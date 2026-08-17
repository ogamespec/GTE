@echo off
setlocal
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
set BIN=C:\Work\GTE\gte-pcsx-framework\bin
set INC=C:\Work\GTE\gte-pcsx-framework\include
set SRC=C:\Work\GTE\gte-pcsx-framework\src

echo STEP1 gte.c
cl /nologo /TC /O2 /I"%INC%" -Fo:"%BIN%\gte.obj" "%SRC%\gte.c" >nul 2>&1
echo gte errorlevel=%errorlevel%

echo STEP2 pcsx_globals.c
cl /nologo /TC /O2 /I"%INC%" -Fo:"%BIN%\pcsx_globals.obj" "%SRC%\pcsx_globals.c" >nul 2>&1
echo globals errorlevel=%errorlevel%

echo STEP3 main.cpp
cl /nologo /std:c++17 /O2 /EHsc /I"%INC%" -Fo:"%BIN%\main.obj" -c "%SRC%\main.cpp" >nul 2>&1
echo main errorlevel=%errorlevel%

endlocal
