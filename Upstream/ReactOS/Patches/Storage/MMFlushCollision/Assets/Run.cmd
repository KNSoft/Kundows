@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" collision %* || exit /b 1
exit /b 0
