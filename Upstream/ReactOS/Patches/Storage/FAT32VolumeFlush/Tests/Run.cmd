@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" bcb-life %* || exit /b 1
call "%~dp0..\..\..\Shared\Host\Run.cmd" bcb-owner %* || exit /b 1
call "%~dp0..\..\..\Shared\Host\Run.cmd" fat %* || exit /b 1
call "%~dp0..\..\..\Shared\Host\Run.cmd" fat-clean-chain %* || exit /b 1
call "%~dp0..\..\..\Shared\Host\Run.cmd" fat-volume %* || exit /b 1
call "%~dp0..\..\..\Shared\Host\Run.cmd" truncate %* || exit /b 1
exit /b 0
