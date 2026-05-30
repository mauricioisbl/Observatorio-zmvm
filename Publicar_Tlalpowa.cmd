@echo off
setlocal EnableExtensions DisableDelayedExpansion
chcp 65001 >nul 2>nul
title Tlalpohua - Publicar Tlalpowa en GitHub

rem ============================================================================
rem  TLALPOHUA - PUBLICADOR ROBUSTO DE TLALPOWA EN GITHUB
rem ============================================================================
rem  Este .cmd es deliberadamente un lanzador mínimo y estable. La lógica real se
rem  encuentra debajo del marcador #@@TLALPOHUA_POWERSHELL@@ como PowerShell puro,
rem  porque intentar mantener validación de rutas, tamaños, ramas, remoto, LFS,
rem  limpieza temporal, staging y push seguro en Batch puro aumenta mucho el riesgo
rem  de errores de comillas, de caracteres UTF-8 y de expansión accidental de
rem  variables. El encabezado Batch sólo extrae el bloque PowerShell a TEMP y lo
rem  ejecuta; así el archivo sigue siendo doble-clickeable en Windows y, al mismo
rem  tiempo, el publicador mantiene una implementación auditable y mucho más segura.
rem
rem  Contrato de no regresión:
rem    1) C:\Tlalpowa NO se convierte en repositorio Git y NO recibe .git.
rem    2) La publicación se prepara en carpetas temporales verificadas.
rem    3) NO clona, NO descarga, NO hace checkout, NO hace pull y NO mezcla ningun archivo del remoto.
rem    4) No se usa --force destructivo. El modo normal publica sobre la rama
rem       existente con --force-with-lease exacto, obtenido mediante ls-remote,
rem       porque GitHub es destino y C:\Tlalpowa es la fuente autoritativa.
rem    5) Se valida el manifiesto completo antes de tocar Git.
rem    6) Se bloquean objetos >=100 MiB si no se pide LFS explícitamente.
rem    7) Se genera .gitignore y .gitattributes de publicación para no contaminar el
rem       repositorio con Build, cachés, respaldos, estados locales o artefactos.
rem    8) El autor/committer local se fija como Tlalpohua, salvo correo configurable.
rem    9) El commit publicado es un reemplazo absoluto del arbol remoto: todo
rem       archivo que no exista en este paquete local deja de existir en GitHub
rem       despues del push protegido por lease. No hay merge ni preservacion
rem       silenciosa de archivos remotos ausentes.
rem ============================================================================

where powershell.exe >nul 2>nul
if errorlevel 1 (
  echo [ERROR] PowerShell no esta disponible en PATH.
  pause
  exit /b 1
)

set "TLALPOHUA_SELF=%~f0"
set "TLALPOHUA_PS1=%TEMP%\tlalpohua_publicar_%RANDOM%%RANDOM%.ps1"

powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "$ErrorActionPreference='Stop'; $self=$env:TLALPOHUA_SELF; $out=$env:TLALPOHUA_PS1; $lines=[System.IO.File]::ReadAllLines($self,[System.Text.Encoding]::UTF8); $idx=[Array]::IndexOf($lines,'#@@TLALPOHUA_POWERSHELL@@'); if($idx -lt 0){ throw 'No se encontro el marcador PowerShell interno.' }; $body=$lines[($idx+1)..($lines.Length-1)]; [System.IO.File]::WriteAllLines($out,$body,[System.Text.UTF8Encoding]::new($false))"
if errorlevel 1 (
  echo [ERROR] No pude extraer el bloque PowerShell interno del publicador.
  if exist "%TLALPOHUA_PS1%" del /f /q "%TLALPOHUA_PS1%" >nul 2>nul
  pause
  exit /b 1
)

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%TLALPOHUA_PS1%" %*
set "TLALPOHUA_RC=%ERRORLEVEL%"
del /f /q "%TLALPOHUA_PS1%" >nul 2>nul
exit /b %TLALPOHUA_RC%

#@@TLALPOHUA_POWERSHELL@@
$ErrorActionPreference = 'Stop'
# ---------------------------------------------------------------------------
#  COMPATIBILIDAD ESTRICTA DE WINDOWS POWERSHELL 5.1 / POWERSHELL 7+:
#  El publicador valida manualmente cada condicion critica y por eso NO depende
#  de Set-StrictMode para proteger el flujo de publicacion. StrictMode convierte
#  accesos inocuos como .Count sobre ciertos escalares producidos por tuberias o
#  comandos nativos en errores terminantes, incluso cuando el commit ya fue
#  creado correctamente. Esta desactivacion es local al bloque PowerShell
#  temporal extraido por este .cmd; no modifica perfiles, no modifica la sesion
#  global del usuario, no toca Git global y no reduce las garantias reales del
#  publicador: las comprobaciones de HEAD, commit raiz, arbol, manifiesto local,
#  objetos grandes, lease remoto y push protegido siguen ejecutandose mediante
#  condiciones explicitas. No volver a activar Set-StrictMode aqui sin antes
#  reauditar cada salida escalar de Git y cada tuberia de PowerShell en Windows
#  PowerShell 5.1, porque eso reintroduce la falla `No se encuentra la propiedad
#  Count en este objeto` justo despues de crear el commit.
# ---------------------------------------------------------------------------
Set-StrictMode -Off
$script:TlalpohuaPublicadorVersion = '2026-05-28-countless-strictmode-safe'

# ============================================================================
#  PUBLICADOR CANÓNICO TLALPOHUA - IMPLEMENTACIÓN POWERSHELL
# ============================================================================
#  Este bloque ejecuta la publicación completa. Se conserva dentro del mismo .cmd
#  para no introducir dependencias adicionales ni archivos auxiliares que después
#  puedan extraviarse. El diseño evita modificar el árbol de trabajo real: todo se
#  prepara en TEMP, se valida, se copia a un repositorio temporal vacio y solo al
#  final se empuja la rama. De ese modo, incluso si GitHub rechaza credenciales,
#  objetos grandes o ramas no fast-forward, C:\Tlalpowa queda intacto.
# ============================================================================

function Write-Section([string]$Text) {
    Write-Host ''
    Write-Host '============================================================'
    Write-Host (' ' + $Text)
    Write-Host '============================================================'
}

function Write-Info([string]$Text) { Write-Host ('[OK] ' + $Text) }
function Write-Warn([string]$Text) { Write-Host ('[ADVERTENCIA] ' + $Text) -ForegroundColor Yellow }
function Write-Fail([string]$Text) { Write-Host ('[ERROR] ' + $Text) -ForegroundColor Red }

function ConvertTo-MaskedRemote([string]$Remote) {
    if ([string]::IsNullOrWhiteSpace($Remote)) { return '' }
    return ($Remote -replace '^(https?://)([^/@:]+:)?([^/@]+)@', '$1***@')
}

function Join-RelativePath([string]$Root, [string]$FullName) {
    $rootFull = [System.IO.Path]::GetFullPath($Root).TrimEnd([System.IO.Path]::DirectorySeparatorChar, [System.IO.Path]::AltDirectorySeparatorChar)
    $fileFull = [System.IO.Path]::GetFullPath($FullName)
    if ($fileFull.Length -le $rootFull.Length) { return '' }
    return $fileFull.Substring($rootFull.Length + 1).Replace('\', '/')
}

function Write-Utf8NoBom([string]$Path, [string]$Text) {
    $parent = Split-Path -Parent $Path
    if ($parent -and -not (Test-Path -LiteralPath $parent)) {
        New-Item -ItemType Directory -Path $parent -Force | Out-Null
    }
    [System.IO.File]::WriteAllText($Path, $Text, [System.Text.UTF8Encoding]::new($false))
}

function Invoke-Native {
    param(
        [Parameter(Mandatory=$true)][string]$Exe,
        [Parameter(Mandatory=$true)][string[]]$ArgumentList,
        [string]$WorkingDirectory,
        [switch]$AllowFailure,
        [switch]$Quiet
    )
    $old = (Get-Location).Path
    try {
        if ($WorkingDirectory) { Set-Location -LiteralPath $WorkingDirectory }
        if (-not $Quiet) { Write-Host ('> ' + $Exe + ' ' + ($ArgumentList -join ' ')) }
        & $Exe @ArgumentList
        $code = $LASTEXITCODE
    }
    finally {
        Set-Location -LiteralPath $old
    }
    if ($null -eq $code) { $code = 0 }
    if ($code -ne 0 -and -not $AllowFailure) {
        throw ('Fallo comando externo con codigo ' + $code + ': ' + $Exe + ' ' + ($ArgumentList -join ' '))
    }
    return [int]$code
}

function Invoke-NativeCapture {
    param(
        [Parameter(Mandatory=$true)][string]$Exe,
        [Parameter(Mandatory=$true)][string[]]$ArgumentList,
        [string]$WorkingDirectory
    )
    $old = (Get-Location).Path
    try {
        if ($WorkingDirectory) { Set-Location -LiteralPath $WorkingDirectory }
        $output = & $Exe @ArgumentList 2>&1
        $code = $LASTEXITCODE
    }
    finally {
        Set-Location -LiteralPath $old
    }
    if ($null -eq $code) { $code = 0 }
    [pscustomobject]@{ Code = [int]$code; Output = ($output -join [Environment]::NewLine) }
}

function Invoke-Git {
    param(
        [Parameter(Mandatory=$true)][string[]]$ArgumentList,
        [string]$WorkingDirectory,
        [switch]$AllowFailure,
        [switch]$Quiet
    )
    Invoke-Native -Exe $script:GitExe -ArgumentList $ArgumentList -WorkingDirectory $WorkingDirectory -AllowFailure:$AllowFailure -Quiet:$Quiet | Out-Null
}

function Invoke-GitCapture {
    param(
        [Parameter(Mandatory=$true)][string[]]$ArgumentList,
        [string]$WorkingDirectory
    )
    Invoke-NativeCapture -Exe $script:GitExe -ArgumentList $ArgumentList -WorkingDirectory $WorkingDirectory
}


function New-TlalpohuaShortWritableTempBase {
    param([string[]]$Candidates)

    # ------------------------------------------------------------------------
    #  BLOQUE CRITICO DE NO REGRESION: el publicador usa un repositorio Git
    #  temporal y nunca convierte C:\Tlalpowa en repositorio. Sin embargo, en
    #  Windows el limite historico MAX_PATH puede romper `git add` aunque el
    #  arbol local sea valido, porque el nombre real que recibe Git no es solo
    #  `Fuente/Config/...`, sino `C:\Users\...\Temp\...\repo\Fuente\Config\...`.
    #  Por eso el directorio temporal debe elegirse deliberadamente corto y
    #  escribible. Esta funcion no altera el proyecto, no toca Datos, no toca
    #  Fuente y no crea metadatos en C:\Tlalpowa; solo selecciona una raiz de
    #  trabajo efimera para que el repositorio temporal tenga la menor longitud
    #  absoluta posible. Si algun candidato falla por permisos, se descarta y se
    #  intenta el siguiente, manteniendo la robustez sin exigir privilegios de
    #  administrador. No reemplazar esta logica por GetTempPath() directo: eso
    #  reintroduce rutas como C:\Users\usuario\AppData\Local\Temp\... y vuelve
    #  a exponer `git add` a errores de Filename too long.
    # ------------------------------------------------------------------------
    foreach ($candidate in $Candidates) {
        if ([string]::IsNullOrWhiteSpace($candidate)) { continue }
        try {
            $full = [System.IO.Path]::GetFullPath($candidate)
            if (-not (Test-Path -LiteralPath $full -PathType Container)) {
                New-Item -ItemType Directory -Path $full -Force | Out-Null
            }
            $probe = Join-Path $full ('.tlalpohua_probe_' + $PID + '_' + [System.Guid]::NewGuid().ToString('N'))
            [System.IO.File]::WriteAllText($probe, 'ok', [System.Text.UTF8Encoding]::new($false))
            Remove-Item -LiteralPath $probe -Force -ErrorAction SilentlyContinue
            return (Resolve-Path -LiteralPath $full).Path
        }
        catch {
            continue
        }
    }
    throw 'No encontre una carpeta temporal escribible para preparar la publicacion.'
}

function Write-TlalpohuaPathRiskSummary {
    param(
        [Parameter(Mandatory=$true)][string]$Root,
        [int]$WarningLength = 240,
        [int]$MaxRows = 12
    )

    # ------------------------------------------------------------------------
    #  BLOQUE CRITICO DE NO REGRESION: esta auditoria no borra, no renombra y no
    #  filtra archivos. Su unica funcion es diagnosticar rutas que se acercan al
    #  borde historico de Windows antes de invocar Git. El remedio real se aplica
    #  en dos niveles: (1) raiz temporal corta, (2) core.longpaths=true en el repo
    #  temporal y tambien como `-c core.longpaths=true` en la llamada critica a
    #  git add. Mantener este bloque como observabilidad: si vuelve a aparecer un
    #  nombre patologicamente largo o mojibakeado, el log indicara exactamente que
    #  ruta se esta publicando sin tocar el arbol autoritativo C:\Tlalpowa.
    # ------------------------------------------------------------------------
    $risky = New-Object System.Collections.Generic.List[object]
    $filesForRisk = @(Get-ChildItem -LiteralPath $Root -File -Force -Recurse -ErrorAction SilentlyContinue)
    foreach ($f in $filesForRisk) {
        $rel = Join-RelativePath $Root $f.FullName
        $absoluteLen = ([System.IO.Path]::GetFullPath($f.FullName)).Length
        if ($absoluteLen -ge $WarningLength -or $f.Name.Length -ge 180) {
            $risky.Add([pscustomobject]@{ Relative = $rel; AbsoluteLength = $absoluteLen; NameLength = $f.Name.Length })
        }
    }
    if ($risky.Count -gt 0) {
        Write-Warn ('Rutas largas detectadas en el repositorio temporal: ' + $risky.Count + '. Se usara core.longpaths=true y raiz temporal corta.')
        foreach ($item in ($risky | Sort-Object AbsoluteLength -Descending | Select-Object -First $MaxRows)) {
            Write-Host ('  - len=' + $item.AbsoluteLength + ' nombre=' + $item.NameLength + ' :: ' + $item.Relative)
        }
        if ($risky.Count -gt $MaxRows) {
            Write-Host ('  - ... ' + ($risky.Count - $MaxRows) + ' ruta(s) adicional(es) omitidas del resumen.')
        }
    }
}


function Get-TlalpohuaFirstLsRemoteSha {
    param([string]$Text)
    if ([string]::IsNullOrWhiteSpace($Text)) { return $null }
    $firstLine = ($Text.Trim() -split "`r?`n")[0]
    # NO REGRESION POWERSHELL/STRICTMODE: el resultado de una tuberia con
    # cero o un elemento puede degradarse a escalar bajo Windows PowerShell y,
    # con Set-StrictMode, acceder a .Count sobre un escalar sin esa propiedad
    # rompe la publicacion aunque Git haya creado bien el commit. El operador
    # de subexpresion de arreglo @() es obligatorio en cualquier enumeracion
    # cuyo conteo o indexacion se use despues. No retirar este envoltorio: es
    # la defensa concreta contra fallas como `No se encuentra la propiedad
    # Count en este objeto` cuando ls-remote devuelve una sola ref.
    $parts = @($firstLine -split "`t| " | Where-Object { -not [string]::IsNullOrWhiteSpace($_) })
    if ($parts.Count -ge 2 -and $parts[0] -match '^[0-9a-fA-F]{40,64}$') { return [string]$parts[0] }
    return $null
}

function Assert-TlalpohuaReplacementCommit {
    param(
        [Parameter(Mandatory=$true)][string]$Repo,
        [Parameter(Mandatory=$true)][string]$RemoteBranchRef
    )

    # ------------------------------------------------------------------------
    #  BLOQUE CRITICO DE NO REGRESION: esta funcion valida que el commit que se
    #  va a empujar sea un commit raiz construido exclusivamente desde el stage
    #  local. Esa propiedad es la que garantiza el reemplazo absoluto del arbol
    #  remoto: al empujar este commit raiz con --force-with-lease exacto, la ref
    #  remota deja de apuntar al arbol anterior y pasa a apuntar al arbol local
    #  preparado. Por tanto, los archivos que existian en GitHub pero no existen
    #  en este commit dejan de estar presentes en la rama publicada. Esta funcion
    #  no hace fetch, no hace pull, no hace checkout y no lee arboles remotos.
    #
    #  CORRECCION DE COMPATIBILIDAD 2026-05-28: queda prohibido depender de
    #  propiedades de conteo expuestas por objetos devueltos por tuberias o por
    #  comandos nativos. Windows PowerShell puede devolver un escalar cuando solo
    #  hay una linea, y StrictMode puede convertir eso en una excepcion aunque el
    #  commit sea correcto. Aqui se usan acumuladores enteros y lectura textual
    #  normalizada, de modo que cero, una o muchas lineas se procesan igual. Esto
    #  mantiene la auditoria sin reintroducir la falla posterior al commit.
    # ------------------------------------------------------------------------
    $head = Invoke-GitCapture -ArgumentList @('rev-parse', '--verify', 'HEAD') -WorkingDirectory $Repo
    if ($head.Code -ne 0 -or [string]::IsNullOrWhiteSpace([string]$head.Output)) { throw 'No existe HEAD para publicar.' }

    $parents = Invoke-GitCapture -ArgumentList @('rev-list', '--parents', '-n', '1', 'HEAD') -WorkingDirectory $Repo
    if ($parents.Code -ne 0) { throw 'No pude auditar los padres del commit de publicacion.' }

    $parentLine = ''
    if ($null -ne $parents.Output) { $parentLine = ([string]$parents.Output).Trim() }

    $tokenTotal = 0
    $tokenMatches = [System.Text.RegularExpressions.Regex]::Matches($parentLine, '\S+')
    foreach ($ignoredToken in $tokenMatches) { $tokenTotal = $tokenTotal + 1 }
    if ($tokenTotal -ne 1) {
        throw 'El commit de publicacion no es un commit raiz. Se detiene para no preservar ni mezclar historia remota por accidente.'
    }

    $tree = Invoke-GitCapture -ArgumentList @('rev-parse', 'HEAD^{tree}') -WorkingDirectory $Repo
    if ($tree.Code -ne 0 -or [string]::IsNullOrWhiteSpace([string]$tree.Output)) { throw 'No pude calcular el arbol Git que se va a publicar.' }

    $tracked = Invoke-GitCapture -ArgumentList @('ls-files') -WorkingDirectory $Repo
    if ($tracked.Code -ne 0) { throw 'No pude enumerar los archivos rastreados del commit de publicacion.' }

    $trackedText = ''
    if ($null -ne $tracked.Output) { $trackedText = [string]$tracked.Output }

    $trackedTotal = 0
    $lineParts = [System.Text.RegularExpressions.Regex]::Split($trackedText, "`r?`n")
    foreach ($line in $lineParts) {
        if (-not [string]::IsNullOrWhiteSpace($line)) { $trackedTotal = $trackedTotal + 1 }
    }
    if ($trackedTotal -le 0) { throw 'El commit de publicacion quedaria sin archivos rastreados. Se detiene por seguridad.' }

    $localHead = ([string]$head.Output).Trim()
    $treeSha = ([string]$tree.Output).Trim()
    Write-Info ('Commit raiz de reemplazo absoluto confirmado: ' + $localHead)
    Write-Info ('Arbol local que sustituira ' + $RemoteBranchRef + ': ' + $treeSha)
    Write-Info ('Archivos rastreados en el arbol publicado: ' + $trackedTotal)
    Write-Info 'Cualquier archivo que exista hoy en GitHub pero no exista en este commit dejara de existir en la rama publicada.'
}

function Assert-TlalpohuaRemoteNowPointsToLocalHead {
    param(
        [Parameter(Mandatory=$true)][string]$Repo,
        [Parameter(Mandatory=$true)][string]$Branch,
        [Parameter(Mandatory=$true)][string]$RemoteBranchRef
    )

    # ------------------------------------------------------------------------
    #  BLOQUE CRITICO DE NO REGRESION: despues del push verificamos solo la ref
    #  remota con ls-remote. Esto no clona, no descarga arboles, no hace checkout
    #  y no mezcla contenido remoto; simplemente confirma que GitHub ya apunta al
    #  commit local exacto recien creado. Si la ref remota apunta a ese commit,
    #  entonces el contenido visible de la rama es exactamente el arbol de dicho
    #  commit, sin archivos remotos sobrantes por fusion o preservacion parcial.
    # ------------------------------------------------------------------------
    $local = Invoke-GitCapture -ArgumentList @('rev-parse', '--verify', 'HEAD') -WorkingDirectory $Repo
    if ($local.Code -ne 0) { throw 'No pude leer HEAD despues del push.' }
    $localSha = $local.Output.Trim()

    $probe = Invoke-GitCapture -ArgumentList @('ls-remote', '--heads', 'origin', $Branch) -WorkingDirectory $Repo
    if ($probe.Code -ne 0) { throw 'El push termino, pero no pude verificar la referencia remota con ls-remote.' }
    $remoteSha = Get-TlalpohuaFirstLsRemoteSha -Text $probe.Output
    if ([string]::IsNullOrWhiteSpace($remoteSha)) { throw ('No encontre la rama remota despues del push: ' + $RemoteBranchRef) }
    if ($remoteSha.ToLowerInvariant() -ne $localSha.ToLowerInvariant()) {
        throw ('La rama remota no apunta al commit local publicado. Local=' + $localSha + ' Remoto=' + $remoteSha)
    }
    Write-Info ('Verificacion remota exacta: ' + $RemoteBranchRef + ' @ ' + $remoteSha)
    Write-Info 'GitHub ya apunta al commit de reemplazo; no quedan archivos remotos fuera del arbol local publicado.'
}

function Read-NextArgument {
    param([string[]]$AllArgs, [ref]$Index, [string]$Name)
    if ($Index.Value + 1 -ge $AllArgs.Count) {
        throw ('El argumento ' + $Name + ' requiere un valor inmediatamente despues.')
    }
    $Index.Value++
    return [string]$AllArgs[$Index.Value]
}

function Show-Usage {
    Write-Host 'Uso:'
    Write-Host '  Publicar_Tlalpowa.cmd [opciones]'
    Write-Host ''
    Write-Host 'Opciones principales:'
    Write-Host '  /remote URL                 Remoto GitHub. Default: TLALPOWA_GIT_REMOTE o repositorio canonico.'
    Write-Host '  /branch RAMA                Rama destino. Default: TLALPOWA_GIT_BRANCH o main.'
    Write-Host '  /email CORREO               Correo local del autor Tlalpohua.'
    Write-Host '  /message TEXTO              Mensaje de commit.'
    Write-Host '  /profile-png RUTA           PNG de identidad visual alternativo.'
    Write-Host '  /dry-run                    Prepara y valida el paquete, pero no crea commit ni hace push.'
    Write-Host '  /no-push                    Crea el commit temporal, pero no empuja al remoto.'
    Write-Host '  /keep-temp                  Conserva TEMP para auditoria manual.'
    Write-Host '  /replace-remote             Alias conservado: el modo normal ya publica sobre la rama existente con lease exacto.'
    Write-Host '  /force-with-lease           Alias conservado: el modo normal ya usa lease exacto con ls-remote, no fetch.'
    Write-Host '  /refuse-existing            Modo ultraconservador: se detiene si la rama remota ya existe.'
    Write-Host '  /lfs                        Usa Git LFS para archivos grandes detectados.'
    Write-Host '  /nopause                    No pausa al terminar.'
}

$Remote = $env:TLALPOWA_GIT_REMOTE
if ([string]::IsNullOrWhiteSpace($Remote)) { $Remote = 'https://github.com/mauricioisbl/Tlalpowa.git' }
$Branch = $env:TLALPOWA_GIT_BRANCH
if ([string]::IsNullOrWhiteSpace($Branch)) { $Branch = 'main' }
$PublisherName = 'Tlalpohua'
$PublisherEmail = $env:TLALPOHUA_GIT_EMAIL
if ([string]::IsNullOrWhiteSpace($PublisherEmail)) { $PublisherEmail = $env:TLALPOWA_GIT_EMAIL }
if ([string]::IsNullOrWhiteSpace($PublisherEmail)) { $PublisherEmail = 'tlalpohua@users.noreply.github.com' }
$ProfilePng = $env:TLALPOHUA_PROFILE_PNG
$CommitMessage = $null
$DryRun = $false
$NoPause = $false
$KeepTemp = $false
$NoPush = $false
$ForceWithLease = $true
$RefuseExisting = $false
$UseLfs = $false
$RemoteWasPositional = $false
$PublishRoot = $null
$Stage = $null
$Repo = $null

try {
    $raw = [string[]]$args
    for ($i = 0; $i -lt $raw.Count; $i++) {
        $a = [string]$raw[$i]
        $lower = $a.ToLowerInvariant()
        if ($lower -in @('/?', '-?', '--help', '/help', '-help')) { Show-Usage; exit 0 }
        elseif ($lower -in @('/nopause', '--nopause', '-nopause', '/no-pause', '--no-pause', '-no-pause')) { $NoPause = $true }
        elseif ($lower -in @('/dry-run', '--dry-run', '-dry-run', '/dryrun', '--dryrun', '-dryrun')) { $DryRun = $true }
        elseif ($lower -in @('/keep-temp', '--keep-temp', '-keep-temp', '/keeptemp', '--keeptemp', '-keeptemp')) { $KeepTemp = $true }
        elseif ($lower -in @('/no-push', '--no-push', '-no-push', '/nopush', '--nopush', '-nopush')) { $NoPush = $true }
        elseif ($lower -in @('/replace-remote', '--replace-remote', '-replace-remote', '/replace', '--replace', '-replace', '/force-with-lease', '--force-with-lease', '-force-with-lease')) { $ForceWithLease = $true }
        elseif ($lower -in @('/refuse-existing', '--refuse-existing', '-refuse-existing', '/no-replace', '--no-replace', '-no-replace', '/do-not-replace', '--do-not-replace', '-do-not-replace')) { $RefuseExisting = $true; $ForceWithLease = $false }
        elseif ($lower -in @('/lfs', '--lfs', '-lfs', '/use-lfs', '--use-lfs', '-use-lfs')) { $UseLfs = $true }
        elseif ($lower -in @('/remote', '--remote', '-remote')) { $Remote = Read-NextArgument -AllArgs $raw -Index ([ref]$i) -Name $a }
        elseif ($lower -in @('/branch', '--branch', '-branch')) { $Branch = Read-NextArgument -AllArgs $raw -Index ([ref]$i) -Name $a }
        elseif ($lower -in @('/email', '--email', '-email')) { $PublisherEmail = Read-NextArgument -AllArgs $raw -Index ([ref]$i) -Name $a }
        elseif ($lower -in @('/profile-png', '--profile-png', '-profile-png', '/profilepng', '--profilepng', '-profilepng')) { $ProfilePng = Read-NextArgument -AllArgs $raw -Index ([ref]$i) -Name $a }
        elseif ($lower -in @('/message', '--message', '-message', '/msg', '--msg', '-msg')) { $CommitMessage = Read-NextArgument -AllArgs $raw -Index ([ref]$i) -Name $a }
        elseif (-not $RemoteWasPositional -and $a -match '^(https?://|ssh://|git@)') {
            $Remote = $a
            $RemoteWasPositional = $true
        }
        else {
            throw ('Argumento no reconocido: ' + $a + '. Usa /? para ver opciones.')
        }
    }

    if ([string]::IsNullOrWhiteSpace($Remote)) { throw 'El remoto GitHub esta vacio.' }
    if ([string]::IsNullOrWhiteSpace($Branch)) { throw 'La rama destino esta vacia.' }
    if ([string]::IsNullOrWhiteSpace($PublisherEmail)) { throw 'El correo del autor esta vacio.' }

    $cmdPath = $env:TLALPOHUA_SELF
    if ([string]::IsNullOrWhiteSpace($cmdPath) -or -not (Test-Path -LiteralPath $cmdPath -PathType Leaf)) {
        throw 'No pude localizar el .cmd lanzador original mediante TLALPOHUA_SELF.'
    }
    $Base = (Resolve-Path -LiteralPath (Split-Path -Parent $cmdPath)).Path
    $stamp = Get-Date -Format 'yyyyMMdd_HHmmss'
    $shortStamp = Get-Date -Format 'HHmmss'
    $tempCandidates = New-Object System.Collections.Generic.List[string]
    if (-not [string]::IsNullOrWhiteSpace($env:TLALPOHUA_PUBLICAR_TEMP_ROOT)) { $tempCandidates.Add($env:TLALPOHUA_PUBLICAR_TEMP_ROOT) }
    $tempCandidates.Add((Join-Path ([System.IO.Path]::GetTempPath()) 'TPW'))
    $tempCandidates.Add([System.IO.Path]::GetTempPath())
    $ShortTempBase = New-TlalpohuaShortWritableTempBase -Candidates ([string[]]$tempCandidates.ToArray())
    $PublishRoot = Join-Path $ShortTempBase ('TP' + $shortStamp + '_' + $PID)
    $Stage = Join-Path $PublishRoot 's'
    $Repo = Join-Path $PublishRoot 'r'

    $gitCommand = Get-Command git.exe -ErrorAction Stop
    $script:GitExe = $gitCommand.Source
    $gitVersion = Invoke-GitCapture -ArgumentList @('--version')
    if ($gitVersion.Code -ne 0) { throw 'Git no responde correctamente.' }

    $branchCheck = Invoke-GitCapture -ArgumentList @('check-ref-format', '--branch', $Branch)
    if ($branchCheck.Code -ne 0) { throw ('La rama no es un nombre valido para Git: ' + $Branch) }

    Write-Section 'Tlalpohua - publicar Tlalpowa en GitHub'
    Write-Host ('GitHub:       ' + (ConvertTo-MaskedRemote $Remote))
    Write-Host ('Rama:         ' + $Branch)
    Write-Host ('Autor:        ' + $PublisherName + ' <' + $PublisherEmail + '>')
    Write-Host ('Base:         ' + $Base)
    Write-Host ('Temp:         ' + $PublishRoot)
    Write-Host ('Git:          ' + $gitVersion.Output.Trim())
    Write-Host ('Modo seco:    ' + $DryRun)
    Write-Host ('Sin push:     ' + $NoPush)
    Write-Host ('LFS:          ' + $UseLfs)
    Write-Host ('Reemplazo remoto protegido por lease: ' + $ForceWithLease)
    Write-Host ('Detener si la rama remota ya existe: ' + $RefuseExisting)
    Write-Host 'Politica:     no clone / no fetch / no checkout / no pull / no merge'
    Write-Host 'Reemplazo:    arbol remoto exacto; lo ausente en este commit se elimina de la rama'

    $required = @(
        @{ Rel = 'Tlalpowa.exe'; Kind = 'File' },
        @{ Rel = 'Tlalpowa.ico'; Kind = 'File' },
        @{ Rel = 'Datos'; Kind = 'Directory' },
        @{ Rel = 'Dependencias'; Kind = 'Directory' },
        @{ Rel = 'Fuente'; Kind = 'Directory' },
        @{ Rel = 'main.css'; Kind = 'File' },
        @{ Rel = 'index.html'; Kind = 'File' },
        @{ Rel = 'CMakeLists.txt'; Kind = 'File' },
        @{ Rel = 'main.js'; Kind = 'File' },
        @{ Rel = 'directorio.pyw'; Kind = 'File' },
        @{ Rel = 'Compilar_Tlalpowa.cmd'; Kind = 'File' },
        @{ Rel = 'Instalar_Dependencias_Globales_Tlalpowa.cmd'; Kind = 'File' },
        @{ Rel = 'Publicar_Tlalpowa.cmd'; Kind = 'File' }
    )

    $missing = New-Object System.Collections.Generic.List[string]
    foreach ($item in $required) {
        $src = Join-Path $Base $item.Rel
        if (-not (Test-Path -LiteralPath $src)) {
            $missing.Add($item.Rel)
            continue
        }
        $obj = Get-Item -LiteralPath $src -Force
        if ($item.Kind -eq 'File' -and $obj.PSIsContainer) { $missing.Add($item.Rel + ' [tipo incorrecto: se esperaba archivo]') }
        if ($item.Kind -eq 'Directory' -and -not $obj.PSIsContainer) { $missing.Add($item.Rel + ' [tipo incorrecto: se esperaba carpeta]') }
    }
    if ($missing.Count -gt 0) {
        throw ('Faltan entradas obligatorias para publicar: ' + ($missing -join ', '))
    }

    $profileCandidates = New-Object System.Collections.Generic.List[string]
    if (-not [string]::IsNullOrWhiteSpace($ProfilePng)) { $profileCandidates.Add($ProfilePng) }
    $profileCandidates.Add((Join-Path $Base 'tlalpowa.png'))
    $profileCandidates.Add((Join-Path $Base 'Tlalpowa.png'))
    $profileCandidates.Add((Join-Path $Base 'Fuente\tlalpowa.png'))
    $profileCandidates.Add((Join-Path $Base 'Fuente\Tlalpowa.png'))
    $profileResolved = $null
    foreach ($candidate in $profileCandidates) {
        if ($candidate -and (Test-Path -LiteralPath $candidate -PathType Leaf)) {
            $profileResolved = (Resolve-Path -LiteralPath $candidate).Path
            break
        }
    }
    if (-not $profileResolved) {
        throw 'No encontre el PNG de identidad visual. Coloca tlalpowa.png en la carpeta base o define TLALPOHUA_PROFILE_PNG.'
    }

    Write-Section 'Preparando stage temporal verificado'
    if (Test-Path -LiteralPath $PublishRoot) { Remove-Item -LiteralPath $PublishRoot -Recurse -Force }
    New-Item -ItemType Directory -Path $Stage -Force | Out-Null

    foreach ($item in $required) {
        $src = Join-Path $Base $item.Rel
        $dst = Join-Path $Stage $item.Rel
        $dstParent = Split-Path -Parent $dst
        if ($dstParent -and -not (Test-Path -LiteralPath $dstParent)) { New-Item -ItemType Directory -Path $dstParent -Force | Out-Null }
        Copy-Item -LiteralPath $src -Destination $dst -Recurse -Force
    }
    Copy-Item -LiteralPath $profileResolved -Destination (Join-Path $Stage 'tlalpowa.png') -Force

    # El stage nunca debe transportar repositorios internos ni flujos de CI ajenos.
    # Esta limpieza se hace despues de copiar para neutralizar dependencias o
    # respaldos que traigan carpetas .git/.github anidadas, sin afectar C:\Tlalpowa.
    $repoMetadata = Get-ChildItem -LiteralPath $Stage -Directory -Force -Recurse -ErrorAction SilentlyContinue | Where-Object { $_.Name -eq '.git' -or $_.Name -eq '.github' } | Sort-Object FullName -Descending
    foreach ($m in $repoMetadata) { Remove-Item -LiteralPath $m.FullName -Recurse -Force -ErrorAction SilentlyContinue }

    $gitignore = @'
# Tlalpohua: reglas de publicacion para no contaminar GitHub con artefactos locales.
# La publicacion copia un manifiesto operativo, pero estas reglas blindan commits
# manuales posteriores y reducen el riesgo de subir caches, compilaciones o estados.
Build/
Build*/
_deps/
.cache/
cache/
cache_mapa/
*.obj
*.o
*.pdb
*.ilk
*.ipdb
*.iobj
*.tmp
*.temp
*.log
*.bak
*.zip
*.7z
*.rar
*.tar
*.gz
*.xz
tlalpowa_estado_usuario.json
rutas_directorio.txt
*.user
*.suo
.vs/
.vscode/.ropeproject/
.DS_Store
Thumbs.db
'@
    Write-Utf8NoBom -Path (Join-Path $Stage '.gitignore') -Text ($gitignore.TrimStart() + [Environment]::NewLine)

    $gitattributes = @'
# Tlalpohua: normalizacion conservadora de finales de linea y binarios.
* text=auto
*.cmd text eol=crlf
*.bat text eol=crlf
*.ps1 text eol=crlf
*.sh text eol=lf
*.c text
*.h text
*.cpp text
*.hpp text
*.cmake text
CMakeLists.txt text
*.png binary
*.jpg binary
*.jpeg binary
*.webp binary
*.ico binary
*.exe binary
*.dll binary
*.ixiptlah binary
*.tile binary
*.pdf binary
*.zip binary
*.7z binary
'@
    Write-Utf8NoBom -Path (Join-Path $Stage '.gitattributes') -Text ($gitattributes.TrimStart() + [Environment]::NewLine)

    $allFiles = @(Get-ChildItem -LiteralPath $Stage -File -Force -Recurse)
    $large100 = @($allFiles | Where-Object { $_.Length -ge 100MB })
    $large50 = @($allFiles | Where-Object { $_.Length -ge 50MB })
    if ($large50.Count -gt 0) {
        Write-Warn 'Archivos de 50 MiB o mas detectados:'
        foreach ($f in $large50) {
            Write-Host ('  - ' + (Join-RelativePath $Stage $f.FullName) + ' = ' + ([math]::Round($f.Length / 1MB, 2)) + ' MiB')
        }
    }
    if ($large100.Count -gt 0 -and -not $UseLfs) {
        $detail = ($large100 | ForEach-Object { (Join-RelativePath $Stage $_.FullName) + ' = ' + ([math]::Round($_.Length / 1MB, 2)) + ' MiB' }) -join '; '
        throw ('GitHub bloquea archivos de 100 MiB o mas en Git normal. Ejecuta con /lfs o publica esos archivos como Release asset. Detalle: ' + $detail)
    }

    $bytes = ($allFiles | Measure-Object -Property Length -Sum).Sum
    $dirs = @(Get-ChildItem -LiteralPath $Stage -Directory -Force -Recurse)
    Write-Info ('Stage listo: ' + $Stage)
    Write-Info ('PNG de identidad: ' + $profileResolved)
    Write-Info ('Archivos: ' + $allFiles.Count + '  Carpetas: ' + $dirs.Count + '  Tamano: ' + ([math]::Round($bytes / 1MB, 2)) + ' MiB')

    if ($DryRun) {
        Write-Section 'Modo seco completado'
        Write-Info 'No se creo repositorio temporal, no se hizo commit y no se hizo push.'
        Write-Info ('Stage conservado para auditoria: ' + $Stage)
        $KeepTemp = $true
        if (-not $NoPause) { Pause }
        exit 0
    }

    Write-Section 'Preparando repositorio temporal estrictamente local'
    # ------------------------------------------------------------------------
    #  BLOQUE CRITICO DE NO REGRESION: este publicador NO debe volver a usar
    #  git clone, git fetch, git pull, git checkout contra origin ni cualquier
    #  otra operacion capaz de materializar archivos de GitHub dentro de una
    #  carpeta de trabajo. La ansiedad operativa que origino esta version es
    #  correcta: el directorio local C:\Tlalpowa es la fuente autoritativa y
    #  ninguna copia remota debe poder sustituirlo, contaminarlo o mezclarse con
    #  sus archivos. Por eso se crea un repositorio temporal vacio mediante
    #  `git init`, se le copia exclusivamente el stage ya validado desde el disco
    #  local y, como maximo, se consulta el SHA de la rama remota con ls-remote.
    #  `git ls-remote` no descarga arboles de archivos ni hace checkout; solo
    #  obtiene referencias. Ese SHA se usa SIEMPRE como lease exacto cuando la
    #  rama remota existe: el flujo normal del publicador ya no falla por el solo
    #  hecho de encontrar main en GitHub. GitHub es destino; el paquete local
    #  validado es la fuente. El push se rechaza solamente si la rama remota
    #  cambio entre ls-remote y el envio, o si GitHub protege la rama. Queda estrictamente prohibido
    #  reintroducir clone/fetch/checkout/pull/merge aqui para "preservar" el
    #  repositorio remoto, porque preservar el remoto implicaria volver a traer
    #  su contenido. La garantia de este publicador es inversa: GitHub debe ser
    #  actualizado desde el paquete local verificado, nunca al reves.
    # ------------------------------------------------------------------------
    if (Test-Path -LiteralPath $Repo) { Remove-Item -LiteralPath $Repo -Recurse -Force -ErrorAction SilentlyContinue }
    New-Item -ItemType Directory -Path $Repo -Force | Out-Null
    Invoke-Git -ArgumentList @('init') -WorkingDirectory $Repo
    Invoke-Git -ArgumentList @('remote', 'add', 'origin', $Remote) -WorkingDirectory $Repo

    $RemoteBranchRef = 'refs/heads/' + $Branch
    # No usamos git checkout ni siquiera en modo orphan. En un repositorio recien
    # inicializado basta fijar HEAD hacia la rama destino con symbolic-ref; asi
    # evitamos que aparezca cualquier patron de checkout que en mantenimiento
    # futuro pueda confundirse con una materializacion del remoto.
    Invoke-Git -ArgumentList @('symbolic-ref', 'HEAD', $RemoteBranchRef) -WorkingDirectory $Repo
    $RemoteBranchSha = $null
    $remoteProbe = Invoke-GitCapture -ArgumentList @('ls-remote', '--heads', 'origin', $Branch) -WorkingDirectory $Repo
    if ($remoteProbe.Code -ne 0) {
        throw 'No pude consultar la referencia remota con git ls-remote. No se descargo ningun archivo, pero tampoco puedo verificar el lease de publicacion.'
    }
    $probeText = $remoteProbe.Output.Trim()
    $parsedRemoteSha = Get-TlalpohuaFirstLsRemoteSha -Text $probeText
    if (-not [string]::IsNullOrWhiteSpace($parsedRemoteSha)) {
        $RemoteBranchSha = $parsedRemoteSha
        Write-Warn ('La rama remota ya existe: ' + $RemoteBranchRef + ' @ ' + $RemoteBranchSha)
        if ($RefuseExisting) {
            throw 'La rama remota ya existe y se pidio /refuse-existing. No se clono, no se bajo ningun archivo remoto y no se modifico C:\Tlalpowa.'
        }
        if ($ForceWithLease) {
            Write-Info 'La publicacion continuara: GitHub quedara reemplazado por el paquete LOCAL validado usando --force-with-lease exacto.'
            Write-Info 'Este flujo NO trae archivos desde GitHub: solo envia los archivos copiados desde C:\Tlalpowa al repositorio temporal.'
        }
    }
    else {
        Write-Info ('La rama remota no existe todavia: ' + $RemoteBranchRef)
    }

    Invoke-Git -ArgumentList @('config', 'core.autocrlf', 'false') -WorkingDirectory $Repo
    Invoke-Git -ArgumentList @('config', 'core.safecrlf', 'false') -WorkingDirectory $Repo
    # Git for Windows mantiene desactivado el soporte de rutas largas por compatibilidad.
    # Activarlo localmente aqui es intencional: no modifica la configuracion global del
    # usuario y protege solamente el repositorio temporal de publicacion.
    Invoke-Git -ArgumentList @('config', 'core.longpaths', 'true') -WorkingDirectory $Repo
    Invoke-Git -ArgumentList @('config', 'user.name', $PublisherName) -WorkingDirectory $Repo
    Invoke-Git -ArgumentList @('config', 'user.email', $PublisherEmail) -WorkingDirectory $Repo

    Write-Section 'Sincronizando manifiesto local en el repositorio temporal'
    Get-ChildItem -LiteralPath $Repo -Force -ErrorAction SilentlyContinue | Where-Object { $_.Name -ne '.git' } | Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
    Get-ChildItem -LiteralPath $Stage -Force | ForEach-Object { Copy-Item -LiteralPath $_.FullName -Destination $Repo -Recurse -Force }

    $env:GIT_AUTHOR_NAME = $PublisherName
    $env:GIT_AUTHOR_EMAIL = $PublisherEmail
    $env:GIT_COMMITTER_NAME = $PublisherName
    $env:GIT_COMMITTER_EMAIL = $PublisherEmail

    if ($UseLfs) {
        Write-Section 'Configurando Git LFS para objetos grandes'
        $lfsCheck = Invoke-GitCapture -ArgumentList @('lfs', 'version') -WorkingDirectory $Repo
        if ($lfsCheck.Code -ne 0) { throw 'Se pidio /lfs, pero Git LFS no esta instalado o no esta disponible para git lfs.' }
        Invoke-Git -ArgumentList @('lfs', 'install', '--local') -WorkingDirectory $Repo
        $largeRepoFiles = @(Get-ChildItem -LiteralPath $Repo -File -Force -Recurse | Where-Object { $_.Length -ge 50MB })
        foreach ($f in $largeRepoFiles) {
            $rel = Join-RelativePath $Repo $f.FullName
            if ($rel -and $rel -ne '.gitattributes') {
                Invoke-Git -ArgumentList @('lfs', 'track', '--', $rel) -WorkingDirectory $Repo
            }
        }
        Write-Info ('Patrones LFS revisados para ' + $largeRepoFiles.Count + ' archivo(s) grande(s).')
    }

    Write-TlalpohuaPathRiskSummary -Root $Repo
    Invoke-Git -ArgumentList @('-c', 'core.longpaths=true', 'add', '-A', '--force') -WorkingDirectory $Repo
    $status = Invoke-GitCapture -ArgumentList @('status', '--porcelain=v1') -WorkingDirectory $Repo
    if ([string]::IsNullOrWhiteSpace($status.Output)) {
        Write-Info 'No hay cambios nuevos que confirmar.'
    }
    else {
        if ([string]::IsNullOrWhiteSpace($CommitMessage)) {
            $CommitMessage = 'Publicacion Tlalpohua ' + (Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
        }
        Invoke-Git -ArgumentList @('commit', '--author', ($PublisherName + ' <' + $PublisherEmail + '>'), '-m', $CommitMessage) -WorkingDirectory $Repo
        Write-Info ('Commit creado: ' + $CommitMessage)
    }

    Assert-TlalpohuaReplacementCommit -Repo $Repo -RemoteBranchRef $RemoteBranchRef

    if ($NoPush) {
        Write-Section 'Commit temporal listo sin push'
        Write-Info ('Repositorio temporal conservado: ' + $Repo)
        $KeepTemp = $true
        if (-not $NoPause) { Pause }
        exit 0
    }

    Write-Section 'Subiendo a GitHub'
    $pushArgs = New-Object System.Collections.Generic.List[string]
    $pushArgs.Add('push')
    if ($ForceWithLease) {
        if ($RemoteBranchSha) {
            $pushArgs.Add('--force-with-lease=' + $RemoteBranchRef + ':' + $RemoteBranchSha)
        }
        else {
            # Si la rama no existia en la consulta previa, este lease exige que
            # siga sin existir al momento del push. Asi evitamos pisar una rama
            # creada por otra persona entre ls-remote y el envio.
            $pushArgs.Add('--force-with-lease=' + $RemoteBranchRef + ':')
        }
    }
    $pushArgs.Add('-u')
    $pushArgs.Add('origin')
    $pushArgs.Add('HEAD:' + $RemoteBranchRef)
    Invoke-Git -ArgumentList ([string[]]$pushArgs.ToArray()) -WorkingDirectory $Repo
    Assert-TlalpohuaRemoteNowPointsToLocalHead -Repo $Repo -Branch $Branch -RemoteBranchRef $RemoteBranchRef

    Write-Section 'Publicacion completada'
    Write-Info ('GitHub actualizado: ' + (ConvertTo-MaskedRemote $Remote))
    Write-Info ('Rama publicada: ' + $Branch)
    Write-Info ('Autor local usado en Git: ' + $PublisherName + ' <' + $PublisherEmail + '>')
    Write-Info 'La carpeta base quedo sin .git ni archivos Git creados por publicar.'
    Write-Info 'No se clono, no se bajo ningun archivo de GitHub y no se hizo checkout del remoto.'
    Write-Info 'La rama remota quedo reducida exactamente al manifiesto local publicado.'

    if (-not $KeepTemp -and (Test-Path -LiteralPath $PublishRoot)) {
        Remove-Item -LiteralPath $PublishRoot -Recurse -Force -ErrorAction SilentlyContinue
    }
    elseif ($KeepTemp) {
        Write-Info ('Temporal conservado: ' + $PublishRoot)
    }

    if (-not $NoPause) { Pause }
    exit 0
}
catch {
    Write-Fail $_.Exception.Message
    if ($_.ScriptStackTrace) { Write-Host $_.ScriptStackTrace -ForegroundColor DarkGray }
    if (-not $KeepTemp -and $PublishRoot -and (Test-Path -LiteralPath $PublishRoot)) {
        Remove-Item -LiteralPath $PublishRoot -Recurse -Force -ErrorAction SilentlyContinue
    }
    elseif ($KeepTemp -and $PublishRoot) {
        Write-Warn ('Temporal conservado para diagnostico: ' + $PublishRoot)
    }
    Write-Host ''
    Write-Host '[FALLO] Publicacion interrumpida.' -ForegroundColor Red
    if (-not $NoPause) { Pause }
    exit 1
}
