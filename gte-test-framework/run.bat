@echo off
REM GTE Test Framework - Run Script
REM Usage: run.bat [test_directory]

set TEST_DIR=%~1
if "%TEST_DIR%"=="" set TEST_DIR=.\v1

echo ========================================
echo   GTE Test Framework Runner
echo ========================================
echo.
echo Test directory: %TEST_DIR%
echo.

if not exist gte-runner.exe (
    echo Error: gte-runner.exe not found!
    echo Please run build.bat first.
    pause
    exit /b 1
)

gte-runner -d "%TEST_DIR%"
set RESULT=%errorlevel%

echo.
pause
exit /b %RESULT%
