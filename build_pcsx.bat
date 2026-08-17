@echo off
setlocal
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
if errorlevel 1 ( echo VCVARS FAILED & exit /b 1 )

set FW=C:\Work\GTE\gte-pcsx-framework
set INC=%FW%\include
set SRC=%FW%\src
set BIN=%FW%\bin
if not exist "%BIN%" mkdir "%BIN%"

echo === gte.c (C) ===
cl /nologo /TC /O2 /I"%INC%" -Fo:"%BIN%\gte.obj" -c "%SRC%\gte.c"
if errorlevel 1 exit /b 1

echo === pcsx_globals.c (C) ===
cl /nologo /TC /O2 /I"%INC%" -Fo:"%BIN%\pcsx_globals.obj" -c "%SRC%\pcsx_globals.c"
if errorlevel 1 exit /b 1

for %%f in (main.cpp pcsx_gte_stub.cpp pcsx_gte_cpu_state.cpp json_parser.cpp gte_model.cpp test_framework.cpp gte_stub.cpp) do (
    echo === %%f ===
    cl /nologo /std:c++17 /O2 /EHsc /I"%INC%" -Fo:"%BIN%\%%~nf.obj" -c "%SRC%\%%f"
    if errorlevel 1 exit /b 1
)

echo === LINK ===
link /nologo /OUT:"%BIN%\gte-pcsx-runner.exe" ^
  "%BIN%\main.obj" "%BIN%\pcsx_gte_stub.obj" "%BIN%\pcsx_gte_cpu_state.obj" ^
  "%BIN%\json_parser.obj" "%BIN%\gte_model.obj" "%BIN%\test_framework.obj" "%BIN%\gte_stub.obj" ^
  "%BIN%\pcsx_globals.obj" "%BIN%\gte.obj"
if errorlevel 1 exit /b 1

echo BUILD OK: %BIN%\gte-pcsx-runner.exe
endlocal
