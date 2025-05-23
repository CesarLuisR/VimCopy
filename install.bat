@echo off
setlocal

REM 1) Variables
set "SRC_DIR=VimoText"
set "EXEC=vimc.exe"
set "TARGET=%USERPROFILE%\bin"

REM 2) Comprueba que exista la carpeta de código
if not exist "%SRC_DIR%" (
  echo Carpeta "%SRC_DIR%" no encontrada.
  exit /b 1
)

REM 3) Compilar
echo Compilando %SRC_DIR%\*.cpp en %EXEC%...
g++ "%SRC_DIR%\*.cpp" -o "%EXEC%"
if errorlevel 1 (
  echo Error en compilacion.
  exit /b 1
)

REM 4) Crear carpeta de bin si hace falta
if not exist "%TARGET%" (
  echo Creando carpeta %TARGET%...
  mkdir "%TARGET%"
)

REM 5) Copiar ejecutable
echo Copiando %EXEC% a %TARGET%...
copy /Y "%EXEC%" "%TARGET%\" >nul

REM 6) Añadir al PATH de usuario si no está
echo Revisando PATH de usuario...
echo %PATH% | find /I "%TARGET%" >nul
if errorlevel 1 (
  echo Agregando %TARGET% a PATH de usuario...
  setx PATH "%PATH%;%TARGET%" >nul
  echo ✔ PATH actualizado.
) else (
  echo %TARGET% ya esta en PATH.
)

echo.
echo 🎉 Instalacion completada.
echo Ahora puedes ejecutar "vimc" desde cualquier carpeta.
endlocal
pause
