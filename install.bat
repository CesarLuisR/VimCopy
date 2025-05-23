@echo off
setlocal EnableDelayedExpansion

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
echo Compilando "%SRC_DIR%\*.cpp" → "%EXEC%"...
g++ "%SRC_DIR%\*.cpp" -o "%EXEC%"
if errorlevel 1 (
    echo Error en compilación.
    exit /b 1
)

REM 4) Crear carpeta de bin si hace falta
if not exist "%TARGET%" (
    echo Creando carpeta "%TARGET%"...
    mkdir "%TARGET%"
)

REM 5) Copiar ejecutable
echo Copiando "%EXEC%" a "%TARGET%\"...
copy /Y "%EXEC%" "%TARGET%\" >nul

REM 6) Leer el PATH de usuario desde el registro
for /f "skip=2 tokens=2,*" %%A in ('
    reg query "HKCU\Environment" /v PATH
') do set "UPATH=%%B"

REM 7) Verificar si ya está en PATH
echo "%UPATH%" | find /I "%TARGET%" >nul
if errorlevel 1 (
    echo Agregando "%TARGET%" al PATH de usuario...
    set "NEWUPATH=!UPATH!;"%TARGET%"
    setx PATH "!NEWUPATH!" >nul
    if errorlevel 0 (
        echo ✔ PATH actualizado.
    ) else (
        echo ⚠ Error al intentar actualizar el PATH. Puede requerir permisos de administrador.
    )
) else (
    echo ℹ "%TARGET%" ya está en el PATH de usuario.
)

echo.
echo 🎉 Instalación completada. Cierra y vuelve a abrir la terminal para usar "vimc".
pause
