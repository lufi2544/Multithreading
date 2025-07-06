
@echo off

:: Set variables
set SRC_DIR=../source
set MAYORANA=%SRC_DIR%/MayoranaFramework/source
set BIN_DIR=bin
set CFLAGS=/std:c17 /Zi /EHsc
set SRC_FILES=main.cpp
if not exist %BIN_DIR% mkdir %BIN_DIR%

cd %BIN_DIR%

:: Create the bin directory if it doesn't exist

:: Set up MSVC environment (ensure it's the correct script)
call "../setup_cl_x64.bat"

:: Compile with MSVC
:: Using Fe: in stead of OUT, since we are using unity build.
cl /Fe:Multithreading %CFLAGS% -I%SRC_DIR% -I%MAYORANA% %SRC_DIR%\%SRC_FILES% /nologo %LDFLAGS% /link /SUBSYSTEM:CONSOLE


