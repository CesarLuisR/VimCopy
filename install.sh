# install.ps1
param(
  [string]$ExecName = "vimc.exe"
)

# 1) Compilar
Write-Host "Compilando *.cpp → $ExecName"
g++ *.cpp -o $ExecName
if ($LASTEXITCODE -ne 0) { throw "Error en compilación" }

# 2) Directorio destino
$target = "$Env:USERPROFILE\bin"
if (!(Test-Path $target)) {
  Write-Host "Creando $target"
  New-Item -ItemType Directory -Path $target | Out-Null
}

# 3) Copiar el ejecutable
Copy-Item $ExecName $target -Force

# 4) Añadir al PATH de usuario (solo si no está)
$paths = [Environment]::GetEnvironmentVariable("PATH","User").Split(";")
if ($paths -notcontains $target) {
  [Environment]::SetEnvironmentVariable(
    "PATH",
    [String]::Join(";", $paths + $target),
    "User"
  )
  Write-Host "Añadido $target al PATH de usuario."
}

Write-Host "✅ Instalación completa. Ejecuta 'vimc' desde cualquier carpeta."

# Asegura permisos de ejecución
chmod +x "$INSTALL_PATH/$EXEC_NAME"

echo
echo "✅ ¡Listo! Ahora puedes ejecutar tu programa desde cualquier ruta con:"
echo "   $EXEC_NAME"
