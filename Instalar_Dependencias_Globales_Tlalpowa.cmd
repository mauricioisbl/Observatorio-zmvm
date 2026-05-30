@echo off
setlocal EnableExtensions DisableDelayedExpansion
chcp 65001 >nul 2>nul
title Tlalpowa - Dependencias globales

for %%I in ("%~dp0.") do set "BASE=%%~fI"
set "LOCAL_DEPS=%BASE%\Dependencias"
if not exist "%LOCAL_DEPS%" if exist "%BASE%\05_Dependencias" set "LOCAL_DEPS=%BASE%\05_Dependencias"
set "GLOBAL_DEPS=%ProgramData%\Tlalpowa\Dependencias"
set "PS_DEPS=%TEMP%\tlalpowa_global_deps_%RANDOM%%RANDOM%.ps1"
set "NO_PAUSE=0"
if /I "%~1"=="/nopause" set "NO_PAUSE=1"
if /I "%~1"=="--nopause" set "NO_PAUSE=1"

cd /d "%BASE%" || goto fail_cd

echo ============================================================
echo  Tlalpowa - copiar dependencias a ubicacion global
echo ============================================================
echo  Local:  %LOCAL_DEPS%
echo  Global: %GLOBAL_DEPS%
echo ============================================================
echo.

if not exist "%LOCAL_DEPS%" (
  echo [OK] No existe Dependencias local. Nada que copiar.
  echo [INFO] Ruta global esperada: %GLOBAL_DEPS%
  if not "%NO_PAUSE%"=="1" pause
  exit /b 0
)

where powershell.exe >nul 2>nul || goto no_powershell

> "%PS_DEPS%" echo $ErrorActionPreference = 'Stop'
>> "%PS_DEPS%" echo $local = $env:LOCAL_DEPS
>> "%PS_DEPS%" echo $global = $env:GLOBAL_DEPS
>> "%PS_DEPS%" echo New-Item -ItemType Directory -Path (Split-Path -Parent $global) -Force ^| Out-Null
>> "%PS_DEPS%" echo Get-ChildItem -LiteralPath $local -Force ^| ForEach-Object { Copy-Item -LiteralPath $_.FullName -Destination $global -Recurse -Force }
>> "%PS_DEPS%" echo if ((Split-Path -Leaf $local) -eq '05_Dependencias') { Remove-Item -LiteralPath $local -Recurse -Force }
>> "%PS_DEPS%" echo '[OK] Dependencias globales: ' + $global

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%PS_DEPS%"
set "RC=%ERRORLEVEL%"
del /f /q "%PS_DEPS%" >nul 2>nul
if not "%RC%"=="0" goto move_fail

echo.
echo [OK] Dependencias globales sincronizadas sin romper el paquete portable.
echo [OK] El compilador usara: %GLOBAL_DEPS%
if not "%NO_PAUSE%"=="1" pause
exit /b 0

:fail_cd
echo [ERROR] No pude entrar a la carpeta base.
goto fail

:no_powershell
echo [ERROR] PowerShell no esta disponible.
goto fail

:move_fail
echo [ERROR] No pude mover las dependencias. Puede requerir permisos sobre ProgramData.
goto fail

:fail
echo.
echo [FALLO] Instalacion global interrumpida.
if not "%NO_PAUSE%"=="1" pause
exit /b 1
