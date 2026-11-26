@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" "%~dp0Video.c" win32ss/drivers/videoprt/dispatch.c "NTSTATUS NTAPI IntVideoPortDispatchFdoPnp" %*
exit /b %errorlevel%
