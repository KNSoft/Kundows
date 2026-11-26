@echo off
setlocal
set "out=%~1"
if not defined out set "out=%~dp0..\..\..\..\..\..\OutDir\CcSizes-%VSCMD_ARG_TGT_ARCH%-Debug"
call "%~dp0BuildDriver.cmd" "%~dp0SizeDdi.c" "%out%\SizeDdi.sys" || exit /b 1
cl /nologo /TC /W3 /Od /Z7 /MTd /D_DEBUG %CASE_CFLAGS% /Fo"%out%\FileSizeContract.obj" /Fe"%out%\FileSizeContract.exe" "%~dp0FileSizeContract.c" /link /DEBUG
exit /b %errorlevel%
