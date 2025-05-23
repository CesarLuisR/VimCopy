# install.ps1
param(
  [string]$ExecName = "vimc.exe",
  [string]$SourceDir = "VimoText"   # Carpeta donde están los .cpp
)

# 1) Compilar todos los .cpp dentro de VimoText
if (!(Test-Path $SourceDir)) {
    throw "Carpeta '$SourceDir' no encontrada."
}
Write-Host "📦 Compilando $SourceDir\*.cpp → $ExecName"
$cppFiles = Get-ChildItem -Path $SourceDir -Filter *.cpp | ForEach-Object { $_.FullName }
g++ $cppFiles -o $ExecName
if ($LASTEXITCODE -ne 0) {
    throw "❌ Error en compilación."
}

# 2) Directorio destino en el perfil del usuario
$target = "$Env:USERPROFILE\bin"
if (!(Test-Path $target)) {
    Write-Host "📂 Creando directorio $target"
    New-Item -ItemType Directory -Path $target | Out-Null
}

# 3) Copiar el ejecutable
Write-Host "🚚 Copiando $ExecName → $target"
Copy-Item -Path $ExecName -Destination $target -Force

# 4) Añadir al PATH de usuario si no está ya
$paths = [Environment]::GetEnvironmentVariable("PATH","User").Split(";")
if ($paths -notcontains $target) {
    [Environment]::SetEnvironmentVariable(
        "PATH",
        [String]::Join(";", $paths + $target),
        "User"
    )
    Write-Host "✅ Añadido '$target' al PATH de usuario."
} else {
    Write-Host "ℹ️ '$target' ya está en el PATH."
}

Write-Host "`n🎉 Instalación completa. Ejecuta 'vimc' desde cualquier carpeta."
