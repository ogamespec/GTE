# GTE Test Framework - Windows Build Script
# Run in PowerShell: .\build.ps1

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  GTE Test Framework - Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Find vcvarsall.bat
$vcvarsPath = $null
$vcvarsPatterns = @(
    "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat",
    "C:\Program Files (x86)\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
)

foreach ($p in $vcvarsPatterns) {
    if (Test-Path $p) {
        $vcvarsPath = $p
        break
    }
}

if (-not $vcvarsPath) {
    Write-Host "Error: Visual Studio not found!" -ForegroundColor Red
    Write-Host "Please install Visual Studio Build Tools:" -ForegroundColor Yellow
    Write-Host "  https://visualstudio.microsoft.com/visual-cpp-build-tools/" -ForegroundColor Yellow
    pause
    exit 1
}

Write-Host "Loading Visual Studio environment..." -ForegroundColor Yellow
& cmd /c "`"$vcvarsPath`" x64 >nul 2>&1 && set" | ForEach-Object {
    if ($_ -match "^(.*?)=(.*)$") {
        $name = $matches[1]
        $value = $matches[2]
        Set-Item -Force -Path "env:$name" -Value $value
    }
}

# Create build directory
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Write-Host "Compiling..." -ForegroundColor Yellow
& cl /std:c++17 /O2 /Iinclude /Fe:gte-runner.exe src\main.cpp src\json_parser.cpp src\gte_model.cpp src\dummy_api.cpp src\test_framework.cpp

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "Build FAILED!" -ForegroundColor Red
    pause
    exit $LASTEXITCODE
}

Write-Host ""
Write-Host "Build successful!" -ForegroundColor Green
Write-Host ""
Write-Host "Running tests from .\v1..." -ForegroundColor Yellow
Write-Host ""

& .\gte-runner.exe

Write-Host ""
pause
