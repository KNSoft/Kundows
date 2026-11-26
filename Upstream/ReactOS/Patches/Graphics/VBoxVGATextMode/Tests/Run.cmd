@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" "%~dp0Display.c" "%~dp0Bind.ps1" %*
exit /b %errorlevel%
