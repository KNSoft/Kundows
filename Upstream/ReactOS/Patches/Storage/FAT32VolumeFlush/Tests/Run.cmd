@echo off
setlocal
set "run=%~dp0..\..\..\Shared\Host\Run.cmd"
set "host=%~dp0Host"
for %%A in (--lifetime --ownership --volume) do (
    set "KW_CASE_ARG=%%A"
    call "%run%" "%host%\Bcb.c" "%host%\Bcb.ps1" %* || exit /b 1
)
set "KW_CASE_ARG="
call "%run%" "%host%\Fat.c" "%host%\Fat.ps1" %* || exit /b 1
setlocal
set "CASE_CFLAGS=%CASE_CFLAGS% /DKW_FAT_FLUSH_CHAIN"
call "%run%" "%host%\Fat.c" "%host%\Fat.ps1" %* || exit /b 1
endlocal
call "%run%" "%host%\Truncate.c" ^
 ntoskrnl/cc/pin.c "BOOLEAN CcpCanPurgeBcbRange;BOOLEAN CcpTruncateBcbs;VOID CcpFreeBcbList" ^
 ntoskrnl/mm/section.c "BOOLEAN NTAPI MmPurgeSegment" ^
 ntoskrnl/cc/view.c "static VOID CcpSignalUninitializeEvents;static PCACHE_UNINITIALIZE_EVENT CcpReuseFileCache;static NTSTATUS CcRosDeleteFileCache;VOID CcpDereferenceCacheMap;NTSTATUS CcRosUninitializeFileCache" ^
 ntoskrnl/cc/fs.c "static BOOLEAN CcpPurgeCacheSection;BOOLEAN NTAPI CcPurgeCacheSection;static VOID CcpNoteTruncate;static NTSTATUS CcpFinishTruncate;NTSTATUS NTAPI CcSetFileSizesEx;VOID CcpApplyCloseSize;VOID NTAPI CcSetFileSizes;BOOLEAN NTAPI CcUninitializeCacheMap;BOOLEAN NTAPI CcGetFileSizes" %*
exit /b %errorlevel%
