@echo off
setlocal
rem Run in an x64 MSVC developer prompt. Keep the test out of the OS image.
set "out=%~1"
if not defined out set "out=%~dp0..\..\..\..\..\..\..\OutDir\FatVolume-%VSCMD_ARG_TGT_ARCH%-Debug"
if not exist "%out%" mkdir "%out%" || exit /b 1
cl /nologo /W4 /Od /Z7 /D_DEBUG %CASE_CFLAGS% /GS- /Zl /Fo"%out%\fatflush.obj" /Fe"%out%\fatflush.exe" "%~dp0FatFlush.c" /link /DEBUG /nodefaultlib /entry:entry /subsystem:console,5.02 kernel32.lib user32.lib shell32.lib
exit /b %errorlevel%
