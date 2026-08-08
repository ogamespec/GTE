@echo off
setlocal

set FRAMEWORK_DIR=%~dp0
set INCLUDE_DIR=%FRAMEWORK_DIR%include
set SRC_DIR=%FRAMEWORK_DIR%src
set BIN_DIR=%FRAMEWORK_DIR%bin

if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

set OUTPUT_EXE=%BIN_DIR%\gte-pcsx-runner.exe

echo Building PCSX GTE Test Framework...

cl /std:c++17 /O2 /I"%INCLUDE_DIR%" /EHsc /Fe:"%OUTPUT_EXE%" ^
    "%SRC_DIR%\main.cpp" ^
    "%SRC_DIR%\pcsx_gte_stub.cpp" ^
    "%SRC_DIR%\pcsx_gte_cpu_state.cpp" ^
    "%SRC_DIR%\gte.c"

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build successful: %OUTPUT_EXE%
echo Running tests...
echo.

"%OUTPUT_EXE%" %*
