@echo off
setlocal
set "host_run=%~dp0..\..\..\Shared\Host\Run.cmd"
set "mm=?static BOOLEAN MiCheckDirtySegment;BOOLEAN NTAPI MmCheckDirtySegment;NTSTATUS NTAPI MmFlushSegment"
set "cc=NTSTATUS CcRosFlushVacb;NTSTATUS CcRosFlushDirtyPages;VOID NTAPI CcFlushCache"
if /i "%~1"=="--mm-only" (
    if not "%~2"=="" exit /b 2
    call "%host_run%" "%~dp0Writeback.c" ntoskrnl/mm/section.c "%mm%" ntoskrnl/cc/view.c "%cc%" --stock-file ntoskrnl/cc/view.c
) else (
    call "%host_run%" "%~dp0Writeback.c" ntoskrnl/mm/section.c "%mm%" ntoskrnl/cc/view.c "%cc%" %* || exit /b 1
    call "%host_run%" "%~dp0SinglePage.c" ntoskrnl/mm/section.c "%mm%" %*
)
exit /b %errorlevel%
