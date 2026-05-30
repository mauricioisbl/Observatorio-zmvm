# Script PowerShell: Convert-PngToIco.ps1
# Papel técnico: conversión determinista de un PNG maestro a un ICO multiresolución para el ejecutable Windows. La lógica mantiene calidad de remuestreo, transparencia alfa, escritura atómica y validación de tamaños para que CMake pueda regenerar el icono sin intervención manual.

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: param(
param(
    [Parameter(Mandatory = $true)]
    [string] $InputPng,

    [Parameter(Mandatory = $true)]
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: [string] $OutputIco,
    [string] $OutputIco,

<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:17 · bloque 1
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
    [int[]] $Sizes = @(16, 24, 32, 48, 64, 128, 256)
)

$ErrorActionPreference = 'Stop'
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: Set-StrictMode -Version 2.0
Set-StrictMode -Version 2.0

# Función Write-Info: etapa aislada del flujo de conversión; debe conservar manejo explícito de errores y liberación de recursos .NET para evitar archivos corruptos o handles abiertos durante la compilación.
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: function Write-Info([string] $Message) {
function Write-Info([string] $Message) {
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: Write-Host "[ICONO] $Message"
    Write-Host "[ICONO] $Message"
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:35 · bloque 2
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
}

# Función Get-IcoDimensionByte: etapa aislada del flujo de conversión; debe conservar manejo explícito de errores y liberación de recursos .NET para evitar archivos corruptos o handles abiertos durante la compilación.
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: function Get-IcoDimensionByte([int] $Value) {
function Get-IcoDimensionByte([int] $Value) {
    if ($Value -eq 256) { return [byte]0 }
    if ($Value -lt 1 -or $Value -gt 255) { throw "Tamano ICO invalido: $Value" }
    return [byte]$Value
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: }
}

# Función New-ResampledSquarePngBytes: etapa aislada del flujo de conversión; debe conservar manejo explícito de errores y liberación de recursos .NET para evitar archivos corruptos o handles abiertos durante la compilación.
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: function New-ResampledSquarePngBytes($SourceBitmap, [int] $Size) {
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:54 · bloque 3
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
function New-ResampledSquarePngBytes($SourceBitmap, [int] $Size) {
    if ($Size -lt 1 -or $Size -gt 256) { throw "Tamano de icono fuera de rango: $Size" }

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $bitmap = New-Object System.Drawing.Bitmap $Size, $Size, ([System.Drawing.Imaging.PixelFormat]::Format32bpp...
    $bitmap = New-Object System.Drawing.Bitmap $Size, $Size, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
    $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
    try {
        $graphics.Clear([System.Drawing.Color]::Transparent)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:67 · bloque 4
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
        $graphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
        $graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
        $graphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
        $graphics.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
        $graphics.CompositingMode = [System.Drawing.Drawing2D.CompositingMode]::SourceOver

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $sourceWidth = [double]$SourceBitmap.Width
        $sourceWidth = [double]$SourceBitmap.Width
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:80 · bloque 5
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
        $sourceHeight = [double]$SourceBitmap.Height
        if ($sourceWidth -le 0 -or $sourceHeight -le 0) { throw 'La imagen fuente no tiene dimensiones validas.' }

        $scale = [Math]::Min([double]$Size / $sourceWidth, [double]$Size / $sourceHeight)
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $drawWidth = [Math]::Max(1, [int][Math]::Round($sourceWidth * $scale))
        $drawWidth = [Math]::Max(1, [int][Math]::Round($sourceWidth * $scale))
        $drawHeight = [Math]::Max(1, [int][Math]::Round($sourceHeight * $scale))
        $drawX = [int][Math]::Floor(($Size - $drawWidth) / 2.0)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:90 · bloque 6
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
        $drawY = [int][Math]::Floor(($Size - $drawHeight) / 2.0)
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $destRect = New-Object System.Drawing.Rectangle $drawX, $drawY, $drawWidth, $drawHeight
        $destRect = New-Object System.Drawing.Rectangle $drawX, $drawY, $drawWidth, $drawHeight
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $srcRect = New-Object System.Drawing.Rectangle 0, 0, $SourceBitmap.Width, $SourceBitmap.Height
        $srcRect = New-Object System.Drawing.Rectangle 0, 0, $SourceBitmap.Width, $SourceBitmap.Height
        $graphics.DrawImage($SourceBitmap, $destRect, $srcRect, [System.Drawing.GraphicsUnit]::Pixel)

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $stream = New-Object System.IO.MemoryStream
        $stream = New-Object System.IO.MemoryStream
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: try {
        try {
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $bitmap.Save($stream, [System.Drawing.Imaging.ImageFormat]::Png)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:112 · bloque 7
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
            $bitmap.Save($stream, [System.Drawing.Imaging.ImageFormat]::Png)
            return $stream.ToArray()
        } finally {
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $stream.Dispose()
            $stream.Dispose()
        }
    } finally {
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:121 · bloque 8
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
        $graphics.Dispose()
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $bitmap.Dispose()
        $bitmap.Dispose()
    }
}

# Función Write-IcoFile: etapa aislada del flujo de conversión; debe conservar manejo explícito de errores y liberación de recursos .NET para evitar archivos corruptos o handles abiertos durante la compilación.
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: function Write-IcoFile([array] $Entries, [string] $Path) {
function Write-IcoFile([array] $Entries, [string] $Path) {
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $parent = [System.IO.Path]::GetDirectoryName($Path)
    $parent = [System.IO.Path]::GetDirectoryName($Path)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:138 · bloque 9
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
    if ($parent -and -not [System.IO.Directory]::Exists($parent)) {
        [System.IO.Directory]::CreateDirectory($parent) | Out-Null
    }

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $tmp = $Path + '.tmp.' + [System.Diagnostics.Process]::GetCurrentProcess().Id + '.ico'
    $tmp = $Path + '.tmp.' + [System.Diagnostics.Process]::GetCurrentProcess().Id + '.ico'
    if ([System.IO.File]::Exists($tmp)) { [System.IO.File]::Delete($tmp) }

    $fs = [System.IO.File]::Open($tmp, [System.IO.FileMode]::CreateNew, [System.IO.FileAccess]::Write, [System.IO.FileShare]::None)
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $writer = New-Object System.IO.BinaryWriter $fs
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:152 · bloque 10
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
    $writer = New-Object System.IO.BinaryWriter $fs
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: try {
    try {
        $writer.Write([UInt16]0)
        $writer.Write([UInt16]1)
        $writer.Write([UInt16]$Entries.Count)

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $offset = 6 + ($Entries.Count * 16)
        $offset = 6 + ($Entries.Count * 16)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:165 · bloque 11
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
        foreach ($entry in $Entries) {
            $writer.Write((Get-IcoDimensionByte $entry.Size))
            $writer.Write((Get-IcoDimensionByte $entry.Size))
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $writer.Write([byte]0)
            $writer.Write([byte]0)
            $writer.Write([byte]0)
            $writer.Write([UInt16]1)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:174 · bloque 12
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
            $writer.Write([UInt16]32)
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $writer.Write([UInt32]$entry.Bytes.Length)
            $writer.Write([UInt32]$entry.Bytes.Length)
            $writer.Write([UInt32]$offset)
            $offset += $entry.Bytes.Length
        }
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: foreach ($entry in $Entries) {
        foreach ($entry in $Entries) {
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:186 · bloque 13
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
            $writer.Write([byte[]]$entry.Bytes)
        }
    } finally {
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $writer.Dispose()
        $writer.Dispose()
        $fs.Dispose()
    }

<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:196 · bloque 14
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
    if ([System.IO.File]::Exists($Path)) {
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: [System.IO.File]::Delete($Path)
        [System.IO.File]::Delete($Path)
    }
    [System.IO.File]::Move($tmp, $Path)
}

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $inputFull = [System.IO.Path]::GetFullPath($InputPng)
$inputFull = [System.IO.Path]::GetFullPath($InputPng)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:209 · bloque 15
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
$outputFull = [System.IO.Path]::GetFullPath($OutputIco)
if (-not [System.IO.File]::Exists($inputFull)) {
    throw "No existe el PNG fuente: $inputFull"
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: }
}

Add-Type -AssemblyName System.Drawing

$image = $null
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:220 · bloque 16
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
$bitmap = $null
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: try {
try {
    $image = [System.Drawing.Image]::FromFile($inputFull)
    if ($image.Width -lt 1 -or $image.Height -lt 1) { throw 'PNG sin dimensiones validas.' }

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $bitmap = New-Object System.Drawing.Bitmap $image.Width, $image.Height, ([System.Drawing.Imaging.PixelForma...
    $bitmap = New-Object System.Drawing.Bitmap $image.Width, $image.Height, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
    $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:236 · bloque 17
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
    try {
        $graphics.Clear([System.Drawing.Color]::Transparent)
        $graphics.CompositingMode = [System.Drawing.Drawing2D.CompositingMode]::SourceOver
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $graphics.DrawImage($image, 0, 0, $image.Width, $image.Height)
        $graphics.DrawImage($image, 0, 0, $image.Width, $image.Height)
    } finally {
        $graphics.Dispose()
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:245 · bloque 18
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
    }

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $uniqueSizes = @($Sizes | Where-Object { $_ -ge 16 -and $_ -le 256 } | Sort-Object -Unique)
    $uniqueSizes = @($Sizes | Where-Object { $_ -ge 16 -and $_ -le 256 } | Sort-Object -Unique)
    if (-not $uniqueSizes -or $uniqueSizes.Count -eq 0) { throw 'No hay tamanos validos para el ICO.' }

    $entries = @()
    foreach ($size in $uniqueSizes) {
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: $bytes = New-ResampledSquarePngBytes $bitmap $size
        $bytes = New-ResampledSquarePngBytes $bitmap $size
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:259 · bloque 19
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
        $entries += [PSCustomObject]@{ Size = [int]$size; Bytes = $bytes }
    }

# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: Write-IcoFile $entries $outputFull
    Write-IcoFile $entries $outputFull
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: Write-Info ("Convertido: {0} -> {1} ({2} tamanos)" -f $inputFull, $outputFull, $entries.Count)
    Write-Info ("Convertido: {0} -> {1} ({2} tamanos)" -f $inputFull, $outputFull, $entries.Count)
} finally {
    if ($bitmap -ne $null) { $bitmap.Dispose() }
<#
 TLALPOWA_COMENTARIO_TECNICO_V3 · construcción y no regresión arquitectónica · Convert-PngToIco.ps1:272 · bloque 20
 El grafo de construcción debe conservar juntos los módulos que hacen posible importar, normalizar y materializar datos en IXIPTLAH; quitar una dependencia porque una variante mínima compila puede romper rutas CLI o diagnósticas que no pasan por la interfaz gráfica.
 Las dependencias de cabecera o bibliotecas ligeras que sostienen lectura JSON, extracción, conversión de recursos o empaquetado deben declararse cerca del objetivo que las consume para que la relación sea visible al mantener el sistema.
 La salida ejecutable debe conservar rutas reproducibles y recursos empacados; los importadores de sopa de letras necesitan encontrar herramientas externas y configuraciones sin depender de un estado manual del equipo del usuario.
 Ningún ajuste de compilación debe justificar fragmentar IXIPTLAH por dominio; la arquitectura mensual centralizada es una decisión de persistencia, no una consecuencia accidental de CMake.
 Toda optimización debe preservar diagnósticos útiles y evitar que un fallo de dependencia se convierta en silencio funcional dentro del importador.
#>
    if ($image -ne $null) { $image.Dispose() }
# Profundización del script: la instrucción próxima opera fuera del binario y por eso debe fallar de forma explícita, sin dejar artefactos incoherentes en la carpeta fuente.
# La conversión de recursos forma parte del build reproducible; rutas, permisos y objetos COM deben tratarse como dependencias frágiles del entorno Windows.
# Contexto: la herramienta de conversión de icono, utilizada en la etapa de construcción para mantener coherente el PNG fuente y el ICO empacado. Línea próxima: }
}
