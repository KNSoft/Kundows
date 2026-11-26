@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" user %* || exit /b 1
exit /b 0
