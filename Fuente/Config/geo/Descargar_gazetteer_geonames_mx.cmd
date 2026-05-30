@echo off
setlocal EnableExtensions EnableDelayedExpansion
REM -----------------------------------------------------------------------------
REM Tlalpowa · descarga opcional de gazetteer puntual mexicano para pines de
REM ciudades, pueblos, villas y localidades. Este archivo NO sustituye los
REM catálogos curados incluidos; sólo permite ampliar la cobertura con GeoNames
REM sin tocar el código. El renderizador filtra por viewport y LOD, por lo que
REM puede leer MX.txt completo sin reconstruir pines desde municipios.
REM -----------------------------------------------------------------------------
set "ROOT=%~dp0"
set "PS=%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe"
if not exist "%PS%" set "PS=powershell.exe"
"%PS%" -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop';" ^
  "$root=(Resolve-Path '%ROOT%').Path;" ^
  "$tmp=Join-Path $root '_geonames_tmp';" ^
  "New-Item -ItemType Directory -Force -Path $tmp | Out-Null;" ^
  "$jobs=@(@{Url='https://download.geonames.org/export/dump/MX.zip'; Zip='MX.zip'; Txt='MX.txt'},@{Url='https://download.geonames.org/export/dump/cities1000.zip'; Zip='cities1000.zip'; Txt='cities1000.txt'},@{Url='https://download.geonames.org/export/dump/cities500.zip'; Zip='cities500.zip'; Txt='cities500.txt'});" ^
  "foreach($j in $jobs){" ^
  "  $zip=Join-Path $tmp $j.Zip;" ^
  "  Write-Host ('Descargando '+$j.Url);" ^
  "  Invoke-WebRequest -Uri $j.Url -OutFile $zip -UseBasicParsing;" ^
  "  Expand-Archive -Path $zip -DestinationPath $tmp -Force;" ^
  "  $src=Join-Path $tmp $j.Txt;" ^
  "  if(Test-Path $src){ Copy-Item $src (Join-Path $root $j.Txt) -Force; Write-Host ('Instalado '+$j.Txt); }" ^
  "}" ^
  "$stamp=Join-Path $root 'geonames_descarga_leeme.txt';" ^
  "Set-Content -LiteralPath $stamp -Encoding UTF8 -Value ('GeoNames descargado para Tlalpowa: '+(Get-Date -Format s)+[Environment]::NewLine+'Fuentes: https://download.geonames.org/export/dump/ y https://www.geonames.org/export/codes.html'+[Environment]::NewLine+'El cargador acepta feature class P y descarta países no MX.');" ^
  "Remove-Item $tmp -Recurse -Force -ErrorAction SilentlyContinue;"
if errorlevel 1 (
  echo No se pudo descargar o extraer GeoNames. Revisa conexion, permisos o proxy.
  exit /b 1
)
echo Gazetteer GeoNames instalado correctamente en %ROOT%.
exit /b 0
