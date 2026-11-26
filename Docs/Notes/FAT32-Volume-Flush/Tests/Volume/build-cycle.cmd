@echo off
setlocal
rem Diagnostic EXE only. Do not run on an unmarked or system volume.
set "out=%~dp0..\..\..\..\..\OutDir\ApiTests"
if not exist "%out%" mkdir "%out%" || exit /b 1
cl /nologo /W4 /WX /O1 /GS- /Zl /Fo"%out%\fatcycle.obj" /Fe"%out%\fatcycle.exe" "%~dp0fatcycle.c" /link /nodefaultlib /entry:entry /subsystem:console,5.02 kernel32.lib user32.lib shell32.lib
exit /b %errorlevel%
