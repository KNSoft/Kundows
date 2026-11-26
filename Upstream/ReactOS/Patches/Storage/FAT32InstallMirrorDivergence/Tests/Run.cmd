@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" "%~dp0ShortWrite.c" "%~dp0ShortWrite.ps1" %*
exit /b %errorlevel%
