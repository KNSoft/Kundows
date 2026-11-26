@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" "%~dp0Relation.c" win32ss/gdi/eng/device.c "?NTSTATUS NTAPI EngpCompleteTargetRelation;NTSTATUS EngpPnPTargetRelationRequest" %*
exit /b %errorlevel%
