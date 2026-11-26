@echo off
setlocal
rem Run in an x64 MSVC developer prompt. Keep the test out of the OS image.
set "out=%~dp0..\..\..\..\..\OutDir\ApiTests"
if not exist "%out%" mkdir "%out%" || exit /b 1
cl /nologo /W4 /O1 /GS- /Zl /Fo"%out%\fatflush.obj" /Fe"%out%\fatflush.exe" "%~dp0fatflush.c" /link /nodefaultlib /entry:entry /subsystem:console,5.02 kernel32.lib user32.lib shell32.lib
exit /b %errorlevel%
