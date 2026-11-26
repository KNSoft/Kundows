@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" "%~dp0User.c" win32ss/user/ntuser/ntuser.c "NTSTATUS APIENTRY NtUserInitialize" %*
exit /b %errorlevel%
