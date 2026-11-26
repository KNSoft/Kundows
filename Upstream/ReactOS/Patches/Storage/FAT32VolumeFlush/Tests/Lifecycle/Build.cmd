@echo off
setlocal
rem Build only. The driver and controller are for a disposable guest.
set "out=%~1"
if not defined out set "out=%~dp0..\..\..\..\..\..\..\OutDir\CcCycle-%VSCMD_ARG_TGT_ARCH%-Debug"
call "%~dp0..\BuildDriver.cmd" "%~dp0CcCycle.c" "%out%\CcCycle.sys" || exit /b 1
cl /nologo /TC /W4 /Od /Z7 /MTd /D_DEBUG %CASE_CFLAGS% /Fo"%out%\CcCycleUser.obj" /Fe"%out%\CcCycle.exe" "%~dp0CcCycleUser.c" /link /DEBUG advapi32.lib
exit /b %errorlevel%
