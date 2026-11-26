@echo off
setlocal
rem Diagnostic EXE only. Do not run on an unmarked or system volume.
set "out=%~1"
if not defined out set "out=%~dp0..\..\..\..\..\..\..\OutDir\FatVolume-%VSCMD_ARG_TGT_ARCH%-Debug"
if not exist "%out%" mkdir "%out%" || exit /b 1
cl /nologo /W4 /WX /Od /Z7 /D_DEBUG %CASE_CFLAGS% /GS- /Zl /Fo"%out%\fatcycle.obj" /Fe"%out%\fatcycle.exe" "%~dp0FatCycle.c" /link /DEBUG /nodefaultlib /entry:entry /subsystem:console,5.02 kernel32.lib user32.lib shell32.lib
exit /b %errorlevel%
