@echo off
REM GTE Test Framework - Windows Build Script
REM Requires Visual Studio (cl.exe) in PATH

echo ========================================
echo   GTE Test Framework - Build Script
echo ========================================
echo.

REM Check if cl.exe is available
where cl >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: MSVC compiler (cl.exe) not found!
    echo.
    echo Please run this script from:
    echo   "Visual Studio Developer Command Prompt"
    echo or
    echo   "Visual Studio x64 Native Tools Command Prompt"
    echo.
    echo Alternatively, install MinGW-w64 and use Makefile.
    echo.
    pause
    exit /b 1
)

echo Found MSVC compiler.
echo.

REM Create build directory
if not exist build mkdir build

echo Compiling...
cl /std:c++17 /O2 /Wall /Iinclude ^
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
