# PCSX GTE Test Framework Build Script (Windows/MSVC)

$ErrorActionPreference = "Stop"

$FrameworkDir = Split-Path -Parent $MyInvocation.MyScriptName
$IncludeDir = Join-Path $FrameworkDir "include"
$SrcDir = Join-Path $FrameworkDir "src"
$OutputDir = Join-Path $FrameworkDir "bin"

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

# Find Visual Studio vcvarsall.bat
$vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path $vsWhere) {
    $vsPath = & $vsWhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
    if ($vsPath) {
        $vcvarsall = Join-Path $vsPath "VC\Auxiliary\Build\vcvarsall.bat"
        if (Test-Path $vcvarsall) {
            Write-Host "Found Visual Studio at: $vsPath"
        }
    }
}

# Compile with MSVC
$Sources = @(
    (Join-Path $SrcDir "main.cpp"),
    (Join-Path $SrcDir "pcsx_gte_stub.cpp"),
    (Join-Path $SrcDir "pcsx_gte_cpu_state.cpp"),
    (Join-Path $SrcDir "gte.c")
)

$OutputExe = Join-Path $OutputDir "gte-pcsx-runner.exe"

Write-Host "Compiling with MSVC..."
cl /std:c++17 /O2 /I"$IncludeDir" /EHsc /Fe:"$OutputExe" @Sources

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nBuild successful: $OutputExe"
    Write-Host "Running tests..."
    & $OutputExe
} else {
    Write-Host "Build failed!"
    exit 1
}
