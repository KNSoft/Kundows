@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" ioctl %* || exit /b 1
call "%~dp0..\..\..\Shared\Host\Run.cmd" monitor-list %* || exit /b 1
exit /b 0
