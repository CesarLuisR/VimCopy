@echo off
setlocal EnableDelayedExpansion

REM 1) Variables
set "SRC_DIR=VimoText"
set "EXEC=vimc.exe"
set "TARGET_DIR=%USERPROFILE%\bin"
set "TARGET_PATH=%%USERPROFILE%%\bin"

REM 2) Comprobar carpeta de código
if not exist "%SRC_DIR%" (
  echo Carpeta "%SRC_DIR%" no encontrada.
  exit /b 1
)

REM 3) Compilar
echo Compilando %SRC_DIR%\*.cpp → %EXEC%...
g++ "%SRC_DIR%\*.cpp" -o "%EXEC%"
if errorlevel 1 (
  echo Error en compilación.
  exit /b 1
)

REM 4) Crear carpeta bin si no existe
if not exist "%TARGET_DIR%" (
  echo Creando carpeta "%TARGET_DIR%"...
  mkdir "%TARGET_DIR%"
)

REM 5) Copiar ejecutable
echo Copiando %EXEC% a %TARGET_DIR%...
copy /Y "%EXEC%" "%TARGET_DIR%\" >nul

REM 6) Leer PATH actual de usuario
setlocal DisableDelayedExpansion
for /f "skip=2 tokens=2,*" %%A in (
  'reg query "HKCU\Environment" /v PATH 2^>nul'
) do set "UPATH=%%B"
endlocal & set "UPATH=%UPATH%"
setlocal EnableDelayedExpansion

REM 7) Verificar y actualizar PATH
echo !UPATH! | find /I "%TARGET_PATH%" >nul
if errorlevel 1 (
  echo Agregando "%TARGET_PATH%" al PATH de usuario...
  if "!UPATH!"=="" (
    set "NEWUPATH=%TARGET_PATH%"
  ) else (
    set "NEWUPATH=!UPATH!;%TARGET_PATH%"
  )
  reg add "HKCU\Environment" /v PATH /t REG_EXPAND_SZ /d "!NEWUPATH!" /f >nul
  echo ✔ PATH actualizado.
) else (
  echo ℹ "%TARGET_PATH%" ya está en el PATH de usuario.
)

echo.
echo 🎉 Instalación completada. Cierra y vuelve a abrir la terminal.
pause
