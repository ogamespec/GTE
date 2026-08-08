@echo off
setlocal

set FRAMEWORK_DIR=%~dp0
set INCLUDE_DIR=%FRAMEWORK_DIR%include
set SRC_DIR=%FRAMEWORK_DIR%src
set BIN_DIR=%FRAMEWORK_DIR%bin

if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

set GTE_OBJ=%BIN_DIR%\gte.obj
set MAIN_OBJ=%BIN_DIR%\main.obj
set PCSX_GTE_STUB_OBJ=%BIN_DIR%\pcsx_gte_stub.obj
set PCSX_GTE_CPU_STATE_OBJ=%BIN_DIR%\pcsx_gte_cpu_state.obj
set JSON_PARSER_OBJ=%BIN_DIR%\json_parser.obj
set GTE_MODEL_OBJ=%BIN_DIR%\gte_model.obj
set TEST_FRAMEWORK_OBJ=%BIN_DIR%\test_framework.obj
set GTE_STUB_OBJ=%BIN_DIR%\gte_stub.obj
set PCSX_GLOBALS_OBJ=%BIN_DIR%\pcsx_globals.obj
set OUTPUT_EXE=%BIN_DIR%\gte-pcsx-runner.exe

echo Building PCSX GTE Test Framework...
echo.

:: Find vcvarsall.bat
for %%v in ("C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat") do (
    if exist "%%v" (
        call "%%v" x64
        goto :found
    )
)
echo Visual Studio not found!
exit /b 1

:found

:: Compile gte.c as C
echo Compiling gte.c (C)...
cl /TC /O2 /I"%INCLUDE_DIR%" -Fo:"%GTE_OBJ%" "%SRC_DIR%\gte.c"
if errorlevel 1 goto build_failed

:: Compile C++ files
echo Compiling C++ sources...
cl /std:c++17 /O2 /EHsc /I"%INCLUDE_DIR%" -Fo:"%MAIN_OBJ%" -c "%SRC_DIR%\main.cpp"
if errorlevel 1 goto build_failed

cl /std:c++17 /O2 /EHsc /I"%INCLUDE_DIR%" -Fo:"%PCSX_GTE_STUB_OBJ%" -c "%SRC_DIR%\pcsx_gte_stub.cpp"
if errorlevel 1 goto build_failed

cl /std:c++17 /O2 /EHsc /I"%INCLUDE_DIR%" -Fo:"%PCSX_GTE_CPU_STATE_OBJ%" -c "%SRC_DIR%\pcsx_gte_cpu_state.cpp"
if errorlevel 1 goto build_failed

cl /std:c++17 /O2 /EHsc /I"%INCLUDE_DIR%" -Fo:"%JSON_PARSER_OBJ%" -c "%SRC_DIR%\json_parser.cpp"
if errorlevel 1 goto build_failed

cl /std:c++17 /O2 /EHsc /I"%INCLUDE_DIR%" -Fo:"%GTE_MODEL_OBJ%" -c "%SRC_DIR%\gte_model.cpp"
if errorlevel 1 goto build_failed

cl /std:c++17 /O2 /EHsc /I"%INCLUDE_DIR%" -Fo:"%TEST_FRAMEWORK_OBJ%" -c "%SRC_DIR%\test_framework.cpp"
if errorlevel 1 goto build_failed

cl /std:c++17 /O2 /EHsc /I"%INCLUDE_DIR%" -Fo:"%GTE_STUB_OBJ%" -c "%SRC_DIR%\gte_stub.cpp"
if errorlevel 1 goto build_failed

:: Link
echo Linking...
link /OUT:"%OUTPUT_EXE%" "%MAIN_OBJ%" "%PCSX_GTE_STUB_OBJ%" "%PCSX_GTE_CPU_STATE_OBJ%" "%GTE_OBJ%" "%JSON_PARSER_OBJ%" "%GTE_MODEL_OBJ%" "%TEST_FRAMEWORK_OBJ%" "%GTE_STUB_OBJ%" "%PCSX_GLOBALS_OBJ%"
if errorlevel 1 goto build_failed

echo.
echo Build successful: %OUTPUT_EXE%
echo Running tests...
echo.
"%OUTPUT_EXE%" %*
goto :end

:build_failed
echo.
echo Build failed!
exit /b 1

:end
endlocal
