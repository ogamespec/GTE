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

# Compile gte.c as C code
$GteObj = Join-Path $OutputDir "gte.obj"
Write-Host "Compiling gte.c (C)..."
& cmd /c "call `"$vcvarsall`" x64 && cl /TC /O2 /I`"$IncludeDir`" -Fo:`"$GteObj`" `"$SrcDir\gte.c`""

if ($LASTEXITCODE -ne 0) {
    Write-Host "gte.c compilation failed!"
    exit 1
}

# Compile C++ files
$Sources = @(
    (Join-Path $SrcDir "main.cpp"),
    (Join-Path $SrcDir "pcsx_gte_stub.cpp"),
    (Join-Path $SrcDir "pcsx_gte_cpu_state.cpp"),
    (Join-Path $SrcDir "json_parser.cpp"),
    (Join-Path $SrcDir "gte_model.cpp"),
    (Join-Path $SrcDir "test_framework.cpp"),
    (Join-Path $SrcDir "gte_stub.cpp")
)

$Objects = @()
foreach ($src in $Sources) {
    $obj = Join-Path $OutputDir (Split-Path $src -Leaf).Replace(".cpp", ".obj")
    $Objects += $obj
    Write-Host "Compiling $(Split-Path $src -Leaf)..."
    & cmd /c "call `"$vcvarsall`" x64 && cl /std:c++17 /O2 /EHsc /I`"$IncludeDir`" -Fo:`"$obj`" -c `"$src`""
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Compilation failed for $(Split-Path $src -Leaf)!"
        exit 1
    }
}

# Link
$OutputExe = Join-Path $OutputDir "gte-pcsx-runner.exe"
$GlobalObj = Join-Path $OutputDir "pcsx_globals.obj"

Write-Host "Linking..."
& cmd /c "call `"$vcvarsall`" x64 && link /OUT:`"$OutputExe`" $($Objects -join ' ') $GlobalObj $GteObj"

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nBuild successful: $OutputExe"
    Write-Host "Running tests..."
    & $OutputExe
} else {
    Write-Host "Linking failed!"
    exit 1
}
