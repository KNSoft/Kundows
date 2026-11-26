@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" writeback %* || exit /b 1
call "%~dp0..\..\..\Shared\Host\Run.cmd" single-page %* || exit /b 1
exit /b 0
