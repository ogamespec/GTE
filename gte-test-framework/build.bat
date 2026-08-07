@echo off
REM GTE Test Framework - Windows Build Script
REM Automatically loads Visual Studio environment

echo ========================================
echo   GTE Test Framework - Build Script
echo ========================================
echo.

REM Find and load Visual Studio environment
set VS_PATH=
if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set VS_PATH="C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
)

if "%VS_PATH%"=="" (
    echo Error: Visual Studio environment not found!
    echo.
    echo Please install Visual Studio Build Tools:
    echo   https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo.
    pause
    exit /b 1
)

echo Loading Visual Studio environment...
call %VS_PATH% x64 >nul 2>&1
if %errorlevel% neq 0 (
    echo Warning: Failed to load x64 environment, trying x86...
    call %VS_PATH% x86 >nul 2>&1
)

REM Check if cl.exe is now available
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: cl.exe still not found after loading VS environment.
    echo.
    echo You can also manually run:
    echo   "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    echo.
    pause
    exit /b 1
)

echo Environment loaded successfully.
echo.

REM Create build directory
if not exist build mkdir build

echo Compiling...
cl /std:c++17 /O2 /Iinclude ^
   /Fe:gte-runner.exe ^
   src\main.cpp ^
   src\json_parser.cpp ^
   src\gte_model.cpp ^
   src\dummy_api.cpp ^
   src\test_framework.cpp

if %errorlevel% neq 0 (
    echo.
    echo Build FAILED!
    pause
    exit /b %errorlevel%
)

echo.
echo Build successful!
echo.
echo Running tests from .\v1...
echo.
gte-runner

pause
