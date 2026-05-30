@echo off
setlocal EnableExtensions DisableDelayedExpansion
chcp 65001 >nul 2>nul
title Tlalpowa - Compilador CMD

for %%I in ("%~dp0.") do set "BASE=%%~fI"
for %%I in ("%~dp0.") do set "BASE_SHORT=%%~sI"
if not defined BASE_SHORT set "BASE_SHORT=%BASE%"
set "LOG=%BASE%\Compilar_Tlalpowa.log"
set "NO_PAUSE=0"
for %%A in (%*) do (
  if /I "%%~A"=="/nopause" set "NO_PAUSE=1"
  if /I "%%~A"=="--nopause" set "NO_PAUSE=1"
)

> "%LOG%" echo ============================================================
>> "%LOG%" echo Tlalpowa - log de compilacion
>> "%LOG%" echo ============================================================
>> "%LOG%" echo Inicio: %DATE% %TIME%
>> "%LOG%" echo CMD: %~f0
>> "%LOG%" echo Base: %BASE%
>> "%LOG%" echo ============================================================
>> "%LOG%" echo.

echo ============================================================
echo  Tlalpowa - compilador unico CMD
echo ============================================================
echo  Log: %LOG%
echo  Producto: %BASE%\Tlalpowa.exe
echo ============================================================
echo.

call :main %*
set "RC=%ERRORLEVEL%"
if not "%RC%"=="0" (
  echo.
  echo [ERROR] Compilacion interrumpida. Ultimas lineas del log:
  echo ------------------------------------------------------------
  call :tail_log 80
  echo ------------------------------------------------------------
)
echo.
echo [LOG] %LOG%
echo [CODIGO DE SALIDA] %RC%
if "%NO_PAUSE%"=="1" exit /b %RC%
echo.
echo La consola queda abierta. Escribe exit y presiona Enter para cerrarla.
%ComSpec% /d /k
exit /b %RC%

:main
setlocal EnableExtensions DisableDelayedExpansion
cd /d "%BASE%" || goto fatal_cd
set "BUILD=%BASE%\Build"
set "STAGE=%BUILD%\Producto"
set "STAGED_EXE=%STAGE%\Tlalpowa.exe"
set "CMAKE_BASE=%BASE_SHORT%"
set "CMAKE_BUILD=%BASE_SHORT%\Build"
set "CMAKE_STAGE=%CMAKE_BUILD%\Producto"
set "SOURCE=%BASE%\Fuente"
set "CONFIG_DIR=%SOURCE%\Config"
set "EXTERNAL=%BASE%\Descargas"
set "INTERNAL=%BASE%\Datos"
set "EXE=%BASE%\Tlalpowa.exe"
set "CONFIG=Release"
set "ENABLE_IMGUI=ON"
set "DO_CLEAN=0"
set "DO_SELFTEST=1"
set "DO_RUN=0"
set "PURGE_06=0"
set "GEN=Ninja"
set "PYTHONUTF8=1"
set "TLALPOWA_UTF8=1"
set "TLALPOWA_BUILD_DIR=%BUILD%"
set "TLALPOWA_SOURCE_DIR=%SOURCE%"
set "TLALPOWA_CONFIG_DIR=%CONFIG_DIR%"
set "TLALPOWA_EXTERNAL_DATA_DIR=%EXTERNAL%"
set "TLALPOWA_INTERNAL_DATA_DIR=%INTERNAL%"
set "DEPS_ROOT=%ProgramData%\Tlalpowa\Dependencias"
set "TLALPOWA_DEPS_ROOT=%DEPS_ROOT%"
set "BUILD_JOBS=2"

:parse_args
if "%~1"=="" goto after_args
if /I "%~1"=="clean" set "DO_CLEAN=1"& shift & goto parse_args
if /I "%~1"=="/clean" set "DO_CLEAN=1"& shift & goto parse_args
if /I "%~1"=="--clean" set "DO_CLEAN=1"& shift & goto parse_args
if /I "%~1"=="noclean" set "DO_CLEAN=0"& shift & goto parse_args
if /I "%~1"=="/noclean" set "DO_CLEAN=0"& shift & goto parse_args
if /I "%~1"=="--noclean" set "DO_CLEAN=0"& shift & goto parse_args
if /I "%~1"=="debug" set "CONFIG=Debug"& shift & goto parse_args
if /I "%~1"=="/debug" set "CONFIG=Debug"& shift & goto parse_args
if /I "%~1"=="--debug" set "CONFIG=Debug"& shift & goto parse_args
if /I "%~1"=="release" set "CONFIG=Release"& shift & goto parse_args
if /I "%~1"=="/release" set "CONFIG=Release"& shift & goto parse_args
if /I "%~1"=="--release" set "CONFIG=Release"& shift & goto parse_args
if /I "%~1"=="core" set "ENABLE_IMGUI=OFF"& shift & goto parse_args
if /I "%~1"=="/core" set "ENABLE_IMGUI=OFF"& shift & goto parse_args
if /I "%~1"=="/noimgui" set "ENABLE_IMGUI=OFF"& shift & goto parse_args
if /I "%~1"=="--noimgui" set "ENABLE_IMGUI=OFF"& shift & goto parse_args
if /I "%~1"=="/noselftest" set "DO_SELFTEST=0"& shift & goto parse_args
if /I "%~1"=="--noselftest" set "DO_SELFTEST=0"& shift & goto parse_args
if /I "%~1"=="run" set "DO_RUN=1"& shift & goto parse_args
if /I "%~1"=="/run" set "DO_RUN=1"& shift & goto parse_args
if /I "%~1"=="--run" set "DO_RUN=1"& shift & goto parse_args
if /I "%~1"=="/purge06" set "PURGE_06=1"& shift & goto parse_args
if /I "%~1"=="--purge06" set "PURGE_06=1"& shift & goto parse_args
if /I "%~1"=="ninja" set "GEN=Ninja"& shift & goto parse_args
if /I "%~1"=="/ninja" set "GEN=Ninja"& shift & goto parse_args
if /I "%~1"=="--ninja" set "GEN=Ninja"& shift & goto parse_args
if /I "%~1"=="nmake" set "GEN=NMake Makefiles"& shift & goto parse_args
if /I "%~1"=="/nmake" set "GEN=NMake Makefiles"& shift & goto parse_args
if /I "%~1"=="--nmake" set "GEN=NMake Makefiles"& shift & goto parse_args
if /I "%~1"=="/nopause" shift & goto parse_args
if /I "%~1"=="--nopause" shift & goto parse_args
if /I "%~1"=="/help" goto help
if /I "%~1"=="--help" goto help
echo [ERROR] Argumento no reconocido: %~1
goto help_error

:after_args
call :progress 5 "Validando proyecto"
echo Base:        %BASE%
echo Fuente:      %SOURCE%
echo Build:       %BUILD%
echo Stage:       %STAGE%
echo Modo:        %CONFIG%
echo ImGui:       %ENABLE_IMGUI%
echo Generador:   %GEN%
echo.
>> "%LOG%" echo Base: %BASE%
>> "%LOG%" echo Build: %BUILD%
>> "%LOG%" echo Stage: %STAGE%
>> "%LOG%" echo Modo: %CONFIG%
>> "%LOG%" echo ImGui: %ENABLE_IMGUI%
>> "%LOG%" echo Generador: %GEN%

if not exist "%BASE%\CMakeLists.txt" goto missing_project
if not exist "%SOURCE%" goto missing_project
call :ensure_config
if errorlevel 1 goto missing_config
if not exist "%EXTERNAL%" mkdir "%EXTERNAL%" || goto mkdir_fail
if not exist "%INTERNAL%" mkdir "%INTERNAL%" || goto mkdir_fail
call :ensure_icon
if errorlevel 1 goto icon_fail

call :progress 15 "Preparando carpetas"
if "%PURGE_06%"=="1" if exist "%BASE%\06_Build" rmdir /s /q "%BASE%\06_Build" || goto purge06_fail
if "%DO_CLEAN%"=="1" if exist "%BUILD%" rmdir /s /q "%BUILD%" || goto clean_fail
if exist "%BUILD%\CMakeCache.txt" (
  >> "%LOG%" echo [INFO] Reutilizo CMakeCache existente; CMake validara generador y rutas.
)
if not exist "%BUILD%" mkdir "%BUILD%" || goto mkdir_fail
if exist "%BUILD%\CMakeCache.txt" (
  call :validate_cmake_cache
  if errorlevel 1 goto cmake_cache_reset_fail
)
if not exist "%BUILD%" mkdir "%BUILD%" || goto mkdir_fail
if not exist "%STAGE%" mkdir "%STAGE%" || goto mkdir_fail

call :progress 25 "Detectando MSVC"
set "VCVARS="
if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
if not defined VCVARS if exist "C:\Program Files\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if not defined VCVARS if exist "C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvars64.bat"
if not defined VCVARS if exist "C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" for /f "usebackq tokens=*" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2^>nul`) do if exist "%%I\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=%%I\VC\Auxiliary\Build\vcvars64.bat"
if not defined VCVARS if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if not defined VCVARS if exist "%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if not defined VCVARS goto msvc_fail
call "%VCVARS%" >> "%LOG%" 2>&1 || goto vcvars_fail
where cl.exe >nul 2>nul || goto cl_fail
where cmake.exe >nul 2>nul || goto cmake_fail
if /I "%GEN%"=="Ninja" where ninja.exe >nul 2>nul || goto ninja_fail
if /I "%GEN%"=="NMake Makefiles" where nmake.exe >nul 2>nul || goto nmake_fail

call :progress 34 "Liberando EXE anterior"
call :unlock_final_exe

call :progress 48 "Configurando CMake"
call :configure_cmake_once
if errorlevel 1 (
  echo [WARN] La configuracion CMake fallo una vez; limpio solo metadatos CMake y reintento sin tocar Datos ni cache_mapa.
  >> "%LOG%" echo [WARN] La configuracion CMake fallo una vez; limpio solo metadatos CMake y reintento sin tocar Datos ni cache_mapa.
  call :reset_cmake_metadata
  if errorlevel 1 goto cmake_cache_reset_fail
  call :configure_cmake_once
  if errorlevel 1 goto configure_fail
)

call :progress 66 "Compilando"
if /I "%GEN%"=="Ninja" (
  call :run_live cmake.exe --build "%CMAKE_BUILD%" --config "%CONFIG%" -- -j %BUILD_JOBS%
) else (
  call :run_live cmake.exe --build "%CMAKE_BUILD%" --config "%CONFIG%"
)
if errorlevel 1 goto build_fail

call :progress 88 "Instalando EXE unico"
if not exist "%STAGED_EXE%" goto exe_missing_stage
call :unlock_final_exe
copy /y "%STAGED_EXE%" "%EXE%" >nul || goto install_fail
if not exist "%EXE%" goto exe_missing
if exist "%STAGE%\Dependencias" (
  if exist "%BASE%\Dependencias" rmdir /s /q "%BASE%\Dependencias" || goto deps_package_fail
  robocopy "%STAGE%\Dependencias" "%BASE%\Dependencias" /MIR /NFL /NDL /NJH /NJS /NP >> "%LOG%" 2>&1
  if errorlevel 8 goto deps_package_fail
  echo [OK] Dependencias portables: %BASE%\Dependencias
  >> "%LOG%" echo [OK] Dependencias portables: %BASE%\Dependencias
)
if exist "%STAGE%" del /f /q "%STAGE%\*.exe" >nul 2>nul

call :progress 95 "Selftest"
if "%DO_SELFTEST%"=="1" call :run_live "%EXE%" selftest
if errorlevel 1 goto selftest_fail
if "%DO_RUN%"=="1" start "" "%EXE%"
call :progress 100 "Terminado"
echo.
echo [OK] Compilacion finalizada. EXE final: %EXE%
echo [OK] Para subirlo usa Publicar_Tlalpowa.cmd; la carpeta base no se convierte en repositorio Git.
>> "%LOG%" echo [OK] Compilacion finalizada. EXE final: %EXE%
exit /b 0

:progress
setlocal EnableDelayedExpansion
set /a P=%~1
set "TXT=%~2"
set /a DONE=P/5
set "BAR="
for /L %%B in (1,1,20) do (
  if %%B LEQ !DONE! (set "BAR=!BAR!#") else (set "BAR=!BAR!-")
)
echo [!BAR!] !P!%%  !TXT!
>> "%LOG%" echo [PROGRESO] !P!%% !TXT!
endlocal & exit /b 0

:run_live
setlocal EnableExtensions DisableDelayedExpansion
set "RUN_OUT=%TEMP%\tlalpowa_run_%RANDOM%%RANDOM%.txt"
>> "%LOG%" echo.
>> "%LOG%" echo [CMD] %*
%* > "%RUN_OUT%" 2>&1
set "CMD_RC=%ERRORLEVEL%"
type "%RUN_OUT%"
type "%RUN_OUT%" >> "%LOG%"
del /f /q "%RUN_OUT%" >nul 2>nul
endlocal & exit /b %CMD_RC%

:tail_log
setlocal EnableDelayedExpansion
set "N=%~1"
if not defined N set "N=60"
set "TMP_TAIL=%TEMP%\tlalpowa_tail_%RANDOM%%RANDOM%.txt"
copy /y "%LOG%" "%TMP_TAIL%" >nul 2>nul
for /f "delims=" %%L in ('find /c /v "" ^< "%TMP_TAIL%"') do set "COUNT=%%L"
set /a START=COUNT-N
if !START! LSS 0 set /a START=0
set /a I=0
for /f "usebackq delims=" %%L in ("%TMP_TAIL%") do (
  if !I! GEQ !START! echo(%%L
  set /a I+=1
)
del /f /q "%TMP_TAIL%" >nul 2>nul
endlocal & exit /b 0

:unlock_final_exe
taskkill /IM "Tlalpowa.exe" /F >nul 2>nul
for /L %%R in (1,1,3) do (
  if exist "%EXE%" (
    attrib -R "%EXE%" >nul 2>nul
    del /f /q "%EXE%" >nul 2>nul
    if exist "%EXE%" timeout /t 1 /nobreak >nul 2>nul
  )
)
exit /b 0

:validate_cmake_cache
setlocal EnableExtensions DisableDelayedExpansion
set "CACHE=%BUILD%\CMakeCache.txt"
if not exist "%CACHE%" endlocal & exit /b 0
set "NEEDS_RESET=0"
findstr /C:"CMAKE_GENERATOR:INTERNAL=%GEN%" "%CACHE%" >nul 2>nul || set "NEEDS_RESET=1"
if "%NEEDS_RESET%"=="0" (
  powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "$cache=$env:CACHE; $base=(Resolve-Path -LiteralPath $env:BASE).ProviderPath; $build=Join-Path $base 'Build'; $cmakeBase=$env:CMAKE_BASE; $cmakeBuild=$env:CMAKE_BUILD; $sourcePattern='(?m)^Tlalpowa_SOURCE_DIR:STATIC=(.+)$'; $binaryPattern='(?m)^Tlalpowa_BINARY_DIR:STATIC=(.+)$'; $text=Get-Content -LiteralPath $cache -Raw -ErrorAction Stop; function SamePath($left, $right) { try { $l=[System.IO.Path]::GetFullPath(($left -replace '/','\')).TrimEnd('\'); $r=[System.IO.Path]::GetFullPath(($right -replace '/','\')).TrimEnd('\'); return [string]::Equals($l,$r,[System.StringComparison]::OrdinalIgnoreCase) } catch { return $false } }; function MatchesAny($left, $paths) { foreach($path in $paths) { if($path -and (SamePath $left $path)) { return $true } }; return $false }; if($text -match $sourcePattern -and -not (MatchesAny $Matches[1].Trim() @($base,$cmakeBase))) { exit 20 }; if($text -match $binaryPattern -and -not (MatchesAny $Matches[1].Trim() @($build,$cmakeBuild))) { exit 21 }; exit 0" >nul 2>> "%LOG%"
  if errorlevel 1 set "NEEDS_RESET=1"
)
if "%NEEDS_RESET%"=="1" (
  echo [INFO] La carpeta Build conserva metadatos CMake de otra ruta o generador; reparo solo CMakeCache/CMakeFiles sin tocar Datos, cache_mapa ni dependencias descargadas.
  >> "%LOG%" echo [INFO] La carpeta Build conserva metadatos CMake de otra ruta o generador; reparo solo CMakeCache/CMakeFiles sin tocar Datos, cache_mapa ni dependencias descargadas.
  endlocal & call :reset_cmake_metadata
  exit /b %ERRORLEVEL%
)
endlocal & exit /b 0

:reset_cmake_metadata
rem ---------------------------------------------------------------------------
rem NO REGRESION: esta limpieza NO equivale a borrar Build. En versiones previas
rem una reparacion de CMake podia terminar eliminando cache_mapa, respaldos IXIPTLAH
rem o fuentes ya descargadas en _deps. Aqui se retiran solamente los artefactos que
rem hacen que CMake/Ninja confundan generador, ruta de fuente o reglas antiguas:
rem CMakeCache, CMakeFiles y archivos de generador en la raiz de Build. Las teselas,
rem respaldos, dependencias y productos ajenos a CMake permanecen intactos.
rem ---------------------------------------------------------------------------
if not exist "%BUILD%" exit /b 0
if exist "%BUILD%\CMakeCache.txt" del /f /q "%BUILD%\CMakeCache.txt" >nul 2>> "%LOG%"
if exist "%BUILD%\CMakeFiles" rmdir /s /q "%BUILD%\CMakeFiles" >nul 2>> "%LOG%"
if exist "%BUILD%\build.ninja" del /f /q "%BUILD%\build.ninja" >nul 2>> "%LOG%"
if exist "%BUILD%\.ninja_deps" del /f /q "%BUILD%\.ninja_deps" >nul 2>> "%LOG%"
if exist "%BUILD%\.ninja_log" del /f /q "%BUILD%\.ninja_log" >nul 2>> "%LOG%"
if exist "%BUILD%\cmake_install.cmake" del /f /q "%BUILD%\cmake_install.cmake" >nul 2>> "%LOG%"
exit /b 0

:configure_cmake_once
call :run_live cmake.exe -S "%CMAKE_BASE%" -B "%CMAKE_BUILD%" -G "%GEN%" "-DCMAKE_BUILD_TYPE:STRING=%CONFIG%" "-DCMAKE_TRY_COMPILE_CONFIGURATION:STRING=Release" "-DCMAKE_HAVE_LIBC_PTHREAD:BOOL=OFF" "-DCMAKE_USE_WIN32_THREADS_INIT:BOOL=ON" "-DCMAKE_USE_PTHREADS_INIT:BOOL=OFF" "-DTHREADS_PREFER_PTHREAD_FLAG:BOOL=OFF" "-DTLALPOWA_ENABLE_IMGUI:BOOL=%ENABLE_IMGUI%" "-DTLALPOWA_ENABLE_LTCG:BOOL=OFF" "-DTLALPOWA_ENABLE_CL_MP:BOOL=OFF" "-DTLALPOWA_DEPS_ROOT:PATH=%DEPS_ROOT%" "-DTLALPOWA_RUNTIME_OUTPUT_DIR:PATH=%CMAKE_STAGE%" "-DCMAKE_PDB_OUTPUT_DIRECTORY:PATH=%CMAKE_BUILD%" "-DCMAKE_COMPILE_PDB_OUTPUT_DIRECTORY:PATH=%CMAKE_BUILD%" "-DCMAKE_SUPPRESS_REGENERATION:BOOL=ON"
exit /b %ERRORLEVEL%

:ensure_icon
if exist "%BASE%\tlalpowa.png" (
  if not exist "%SOURCE%\Tools\Convert-PngToIco.ps1" (
    echo [ERROR] Existe tlalpowa.png, pero falta Fuente\Tools\Convert-PngToIco.ps1 para generar Tlalpowa.ico.
    >> "%LOG%" echo [ERROR] Falta Fuente\Tools\Convert-PngToIco.ps1 para generar Tlalpowa.ico.
    exit /b 1
  )
  where powershell.exe >nul 2>nul || (
    echo [ERROR] PowerShell no esta disponible; no puedo convertir tlalpowa.png a Tlalpowa.ico.
    >> "%LOG%" echo [ERROR] PowerShell no esta disponible para convertir tlalpowa.png a Tlalpowa.ico.
    exit /b 1
  )
  echo [INFO] Actualizando Tlalpowa.ico desde tlalpowa.png...
  >> "%LOG%" echo [INFO] Actualizando Tlalpowa.ico desde tlalpowa.png
  powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%SOURCE%\Tools\Convert-PngToIco.ps1" -InputPng "%BASE%\tlalpowa.png" -OutputIco "%BASE%\Tlalpowa.ico" >> "%LOG%" 2>&1
  if errorlevel 1 exit /b 1
  if not exist "%BASE%\Tlalpowa.ico" (
    echo [ERROR] El convertidor termino sin producir Tlalpowa.ico.
    >> "%LOG%" echo [ERROR] El convertidor termino sin producir Tlalpowa.ico.
    exit /b 1
  )
  copy /y "%BASE%\Tlalpowa.ico" "%SOURCE%\Tlalpowa.ico" >nul 2>nul
  echo [OK] Icono actualizado: %BASE%\Tlalpowa.ico
  >> "%LOG%" echo [OK] Icono actualizado: %BASE%\Tlalpowa.ico
  exit /b 0
)
if not exist "%BASE%\Tlalpowa.ico" (
  echo [WARN] No existe Tlalpowa.ico ni tlalpowa.png en la carpeta base; compilo sin recurso de icono.
  >> "%LOG%" echo [WARN] No existe Tlalpowa.ico ni tlalpowa.png en la carpeta base; compilo sin recurso de icono.
)
exit /b 0

:ensure_config
rem ---------------------------------------------------------------------------
rem NO REGRESION: Fuente\Config\diseases.tsv es una dependencia pequena, pero
rem critica, porque Config.cpp y los flujos de clasificacion epidemiologica
rem esperan que exista desde el arranque. La validacion anterior fallaba de
rem manera dura si el usuario habia aplicado un ZIP incremental que contenia
rem codigo actualizado y documentos de actualizacion, pero no necesariamente la
rem configuracion ligera completa. Ese fallo era correcto como alarma, pero era
rem demasiado fragil operacionalmente: impedía compilar un arbol local que podia
rem repararse de forma determinista sin tocar codigo, sin bajar nada de GitHub y
rem sin sustituir la carpeta local por ningun remoto.
rem
rem Esta rutina mantiene el principio de no dar marcha atras: primero acepta el
rem archivo canonico si ya existe; si no existe, intenta reconstruirlo SOLO desde
rem fuentes locales y controladas por el usuario. El orden es deliberado:
rem   1) Fuente\Config\diseases.tsv, ubicacion canonica actual.
rem   2) Fuente\Fuente\Config\diseases.tsv, residuo historico observado en
rem      paquetes previos por doble anidamiento accidental de Fuente.zip.
rem   3) Fuente.zip en la carpeta base, si el usuario conserva el paquete local.
rem   4) Copia minima interna, suficiente para compilar y arrancar, marcada como
rem      recuperacion de emergencia para que no se confunda con el catalogo
rem      curado completo.
rem
rem La funcion no usa git, no clona, no hace pull, no consulta GitHub y no borra
rem ningun archivo existente. Solo crea Fuente\Config si falta y escribe el TSV
rem cuando la ausencia ya fue comprobada. Esto protege tanto la continuidad del
rem compilador como la autoridad de C:\Tlalpowa sobre cualquier repositorio
rem remoto.
rem ---------------------------------------------------------------------------
if exist "%CONFIG_DIR%\diseases.tsv" (
  call :validate_diseases "%CONFIG_DIR%\diseases.tsv"
  if errorlevel 1 exit /b 1
  exit /b 0
)
if not exist "%CONFIG_DIR%" mkdir "%CONFIG_DIR%" || exit /b 1
set "LEGACY_CONFIG=%SOURCE%\Fuente\Config"
if exist "%LEGACY_CONFIG%\diseases.tsv" (
  copy /y "%LEGACY_CONFIG%\diseases.tsv" "%CONFIG_DIR%\diseases.tsv" >nul || exit /b 1
  echo [OK] Restaure Fuente\Config\diseases.tsv desde Fuente\Fuente\Config.
  >> "%LOG%" echo [OK] Restaure Fuente\Config\diseases.tsv desde Fuente\Fuente\Config.
  call :copy_optional_config_siblings "%LEGACY_CONFIG%" "%CONFIG_DIR%"
  call :validate_diseases "%CONFIG_DIR%\diseases.tsv"
  if errorlevel 1 exit /b 1
  exit /b 0
)
if exist "%BASE%\Fuente.zip" (
  call :restore_config_from_zip "%BASE%\Fuente.zip" "%CONFIG_DIR%"
  if not errorlevel 1 if exist "%CONFIG_DIR%\diseases.tsv" (
    echo [OK] Restaure Fuente\Config desde Fuente.zip local.
    >> "%LOG%" echo [OK] Restaure Fuente\Config desde Fuente.zip local.
    call :validate_diseases "%CONFIG_DIR%\diseases.tsv"
    if errorlevel 1 exit /b 1
    exit /b 0
  )
)
call :write_builtin_diseases "%CONFIG_DIR%\diseases.tsv"
if errorlevel 1 exit /b 1
echo [WARN] Genere Fuente\Config\diseases.tsv minimo de emergencia. Recomiendo conservar el catalogo completo de Fuente.zip.
>> "%LOG%" echo [WARN] Genere Fuente\Config\diseases.tsv minimo de emergencia.
call :validate_diseases "%CONFIG_DIR%\diseases.tsv"
if errorlevel 1 exit /b 1
exit /b 0

:copy_optional_config_siblings
set "CFG_SRC=%~1"
set "CFG_DST=%~2"
rem Copiamos companeros de configuracion sin /MIR y sin borrado: la intencion es
rem completar un arbol parcial, no reemplazar configuracion local mas reciente.
for %%F in ("%CFG_SRC%\*.tsv") do if exist "%%~fF" if not exist "%CFG_DST%\%%~nxF" copy /y "%%~fF" "%CFG_DST%\" >nul 2>> "%LOG%"
for %%F in ("%CFG_SRC%\*.json") do if exist "%%~fF" if not exist "%CFG_DST%\%%~nxF" copy /y "%%~fF" "%CFG_DST%\" >nul 2>> "%LOG%"
for %%F in ("%CFG_SRC%\*.md") do if exist "%%~fF" if not exist "%CFG_DST%\%%~nxF" copy /y "%%~fF" "%CFG_DST%\" >nul 2>> "%LOG%"
exit /b 0

:restore_config_from_zip
set "ZIP_PATH=%~1"
set "CFG_DST=%~2"
where powershell.exe >nul 2>nul || exit /b 1
powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "$ErrorActionPreference='Stop'; Add-Type -AssemblyName System.IO.Compression.FileSystem; $zip=[IO.Compression.ZipFile]::OpenRead($env:ZIP_PATH); try { foreach($e in $zip.Entries){ if($e.FullName -like 'Fuente/Config/*' -and -not [string]::IsNullOrEmpty($e.Name)){ $relative=$e.FullName.Substring('Fuente/Config/'.Length); $target=Join-Path $env:CFG_DST ($relative -replace '/', [IO.Path]::DirectorySeparatorChar); $dir=Split-Path -Parent $target; if($dir -and -not (Test-Path -LiteralPath $dir)){ New-Item -ItemType Directory -Force -Path $dir | Out-Null }; if(-not (Test-Path -LiteralPath $target)){ [IO.Compression.ZipFileExtensions]::ExtractToFile($e,$target,$false) } } } } finally { $zip.Dispose() }" >> "%LOG%" 2>&1
exit /b %ERRORLEVEL%

:write_builtin_diseases
set "DISEASES_OUT=%~1"
> "%DISEASES_OUT%" echo # id	canonical	group	cie10_pipe	aliases_pipe
>> "%DISEASES_OUT%" echo neumococo_invasiva	Enfermedad invasiva por neumococo	prevenibles_vacunacion	A40.3^|G00.1^|J13	Enfermedad invasiva por neumococo^|neumococo
>> "%DISEASES_OUT%" echo rotavirus_enteritis	Enteritis debida a rotavirus	prevenibles_vacunacion	A08.0	Enteritis debida a rotavirus^|rotavirus
>> "%DISEASES_OUT%" echo hepatitis_a	Hepatitis virica A	prevenibles_vacunacion	B15	Hepatitis virica A^|Hepatitis A
>> "%DISEASES_OUT%" echo hepatitis_b	Hepatitis virica B	prevenibles_vacunacion	B16	Hepatitis virica B^|Hepatitis B
>> "%DISEASES_OUT%" echo vph	Infeccion por virus del papiloma humano	transmision_sexual	B97.7	Infeccion por virus del papiloma humano^|papiloma humano
>> "%DISEASES_OUT%" echo haemophilus_influenzae_invasiva	Infecciones invasivas por Haemophilus influenzae	prevenibles_vacunacion	A41.3^|G00.0^|J14	Haemophilus influenzae
>> "%DISEASES_OUT%" echo influenza	Influenza	respiratorias	J09^|J10^|J11	Influenza
>> "%DISEASES_OUT%" echo meningitis_meningococica	Meningitis meningococica	prevenibles_vacunacion	A39.0	Meningitis meningococica
>> "%DISEASES_OUT%" echo meningitis_tuberculosa	Meningitis tuberculosa	prevenibles_vacunacion	A17.0	Meningitis tuberculosa
>> "%DISEASES_OUT%" echo iras	Infecciones respiratorias agudas	respiratorias	J00-J06	Infecciones respiratorias agudas^|IRAS
>> "%DISEASES_OUT%" echo neumonias_bronconeumonias	Neumonias y bronconeumonias	respiratorias	J12-J18	Neumonias y bronconeumonias
>> "%DISEASES_OUT%" echo covid_19	Covid-19	respiratorias	U07.1^|U07.2	Covid-19^|COVID 19
>> "%DISEASES_OUT%" echo asma	Asma y estado asmatico	no_transmisibles	J45^|J46	Asma y estado asmatico
>> "%DISEASES_OUT%" echo hipertension	Hipertension arterial	no_transmisibles	I10-I15	Hipertension arterial
>> "%DISEASES_OUT%" echo diabetes_tipo_2	Diabetes mellitus no insulinodependiente tipo II	no_transmisibles	E11	Diabetes mellitus no insulinodependiente^|Diabetes tipo II
>> "%DISEASES_OUT%" echo efectos_calor_luz	Efectos del calor y de la luz	no_transmisibles	T67	Efectos del calor y de la luz
>> "%DISEASES_OUT%" echo toxoplasmosis	Toxoplasmosis	otras_transmisibles	B58	Toxoplasmosis
exit /b %ERRORLEVEL%

:validate_diseases
set "DISEASES_FILE=%~1"
if not exist "%DISEASES_FILE%" exit /b 1
for %%I in ("%DISEASES_FILE%") do if %%~zI LSS 64 exit /b 1
findstr /B /C:"# id" "%DISEASES_FILE%" >nul 2>nul || exit /b 1
findstr /C:"covid_19" "%DISEASES_FILE%" >nul 2>nul || exit /b 1
exit /b 0

:help
echo Uso:
echo   Compilar_Tlalpowa.cmd [clean] [debug^|release] [core] [run] [/noselftest] [/purge06] [/nmake^|/ninja] [/nopause]
echo.
echo Opciones:
echo   clean       Borra Build antes de configurar.
echo   noclean     Reutiliza Build. Es el valor predeterminado.
echo   debug       Compila Debug.
echo   release     Compila Release. Es el valor predeterminado.
echo   core        Compila sin ImGui/GLFW para diagnosticar el nucleo.
echo   run         Abre el EXE al terminar.
echo   /noselftest Omite la prueba selftest.
echo   /purge06    Borra la antigua carpeta 06_Build si existe.
echo   /nmake      Usa NMake Makefiles.
echo   /ninja      Usa Ninja. Es el valor predeterminado.
echo   /nopause    Sale con codigo de error sin dejar la consola abierta.
echo.
echo Icono:
echo   Si existe tlalpowa.png en la carpeta base, siempre se regenera Tlalpowa.ico antes de compilar.
echo.
echo Configuracion:
echo   Antes de configurar CMake, este compilador valida Fuente\Config\diseases.tsv.
echo   Si falta por una extraccion parcial, intenta restaurarlo desde rutas heredadas,
echo   desde Fuente.zip local, o desde una copia minima interna de emergencia.
echo.
exit /b 0

:help_error
exit /b 2
:missing_project
echo [ERROR] Este CMD debe ejecutarse desde la carpeta base del proyecto, junto a CMakeLists.txt y Fuente.
>> "%LOG%" echo [ERROR] Este CMD debe ejecutarse desde la carpeta base del proyecto, junto a CMakeLists.txt y Fuente.
exit /b 1
:missing_config
echo [ERROR] No pude validar o restaurar Fuente\Config\diseases.tsv.
>> "%LOG%" echo [ERROR] No pude validar o restaurar Fuente\Config\diseases.tsv.
exit /b 1
:mkdir_fail
echo [ERROR] No pude crear una carpeta requerida. Revisa permisos y rutas.
>> "%LOG%" echo [ERROR] No pude crear una carpeta requerida. Revisa permisos y rutas.
exit /b 1
:icon_fail
echo [ERROR] No pude validar Tlalpowa.ico en la carpeta base del proyecto.
>> "%LOG%" echo [ERROR] No pude validar Tlalpowa.ico en la carpeta base del proyecto.
exit /b 1
:purge06_fail
echo [ERROR] No pude borrar 06_Build. Cierra procesos o ventanas abiertas dentro de esa carpeta.
exit /b 1
:clean_fail
echo [ERROR] No pude limpiar Build. Cierra procesos de compilacion o ventanas abiertas dentro de Build.
exit /b 1
:msvc_fail
echo [ERROR] No encontre MSVC x64. Instala Visual Studio Build Tools con Desktop development with C++.
exit /b 1
:vcvars_fail
echo [ERROR] vcvars64.bat fallo al preparar el entorno de compilacion.
exit /b 1
:cl_fail
echo [ERROR] cl.exe no quedo disponible despues de vcvars64.bat.
exit /b 1
:cmake_fail
echo [ERROR] cmake.exe no esta disponible. Instala CMake o agregalo al PATH.
exit /b 1
:nmake_fail
echo [ERROR] No encontre NMake. Repara MSVC o ejecuta con /ninja si Ninja esta disponible.
exit /b 1
:ninja_fail
echo [ERROR] Pediste Ninja, pero ninja.exe no esta disponible en PATH.
exit /b 1
:cmake_cache_reset_fail
echo [ERROR] No pude reparar los metadatos CMake de Build sin tocar datos. Cierra editores o procesos dentro de Build y reintenta.
>> "%LOG%" echo [ERROR] No pude reparar los metadatos CMake de Build sin tocar datos. Cierra editores o procesos dentro de Build y reintenta.
exit /b 1
:configure_fail
echo [ERROR] Fallo la configuracion CMake. Revisa el diagnostico anterior.
>> "%LOG%" echo [ERROR] Fallo la configuracion CMake. Revisa el diagnostico anterior.
exit /b 1
:build_fail
echo [ERROR] Fallo la compilacion. Revisa el primer error de C++/RC/linker mostrado arriba.
>> "%LOG%" echo [ERROR] Fallo la compilacion. Revisa el primer error de C++/RC/linker mostrado arriba.
exit /b 1
:exe_missing_stage
echo [ERROR] CMake termino, pero no encontre el EXE de staging: %STAGED_EXE%
exit /b 1
:install_fail
echo [ERROR] No pude instalar el EXE final en carpeta base. Cierra Tlalpowa, antivirus o exploradores que lo bloqueen y reintenta.
exit /b 1
:exe_missing
echo [ERROR] No encontre el EXE final: %EXE%
exit /b 1
:deps_package_fail
echo [ERROR] No pude empaquetar Dependencias junto al EXE.
exit /b 1
:selftest_fail
echo [ERROR] El ejecutable compilo, pero selftest devolvio error.
>> "%LOG%" echo [ERROR] El ejecutable compilo, pero selftest devolvio error.
exit /b 1
:fatal_cd
echo [ERROR] No pude entrar a la carpeta base del script.
exit /b 1
