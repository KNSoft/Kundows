@echo off
setlocal
call "%~dp0..\..\..\Shared\Host\Run.cmd" "%~dp0CacheFlushCollisionTests.c" ntoskrnl/mm/section.c "?static BOOLEAN MiCheckDirtySegment;BOOLEAN NTAPI MmCheckDirtySegment;NTSTATUS NTAPI MmFlushSegment" %*
exit /b %errorlevel%
