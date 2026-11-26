@echo off
setlocal
set "run=%~dp0..\..\..\..\..\Shared\Host\Run.cmd"
set "rc=0"
for %%A in (pin retry) do (
    set "KW_CASE_ARG=%%A"
    call "%run%" "%~dp0Contract.c" "%~dp0Contract.ps1" %* || set "rc=1"
)
exit /b %rc%
