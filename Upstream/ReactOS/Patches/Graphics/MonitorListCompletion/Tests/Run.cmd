@echo off
setlocal
set "run=%~dp0..\..\..\Shared\Host\Run.cmd"
set "port=win32ss/drivers/videoprt/dispatch.c"
set "device=win32ss/gdi/eng/device.c"
call "%run%" "%~dp0Ioctl.c" %port% "NTSTATUS VideoPortEnumMonitorPdo" %device% "DWORD APIENTRY EngDeviceIoControl" %* || exit /b 1
call "%run%" "%~dp0MonitorList.c" %port% "NTSTATUS VideoPortEnumMonitorPdo" %device% "NTSTATUS NTAPI EngpCompleteTargetRelation;NTSTATUS EngpQueryMonitorList" %*
exit /b %errorlevel%
