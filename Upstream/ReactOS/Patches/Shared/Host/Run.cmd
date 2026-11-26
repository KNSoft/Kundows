@echo off
setlocal EnableExtensions DisableDelayedExpansion
if "%~2"=="" goto usage
set "host_tools=%~dp0"
:newdir
set "KW_CASE_DIR=%TEMP%\Kundows-%~n1-%RANDOM%-%RANDOM%"
mkdir "%KW_CASE_DIR%" 2>nul || (
    if exist "%KW_CASE_DIR%\" goto newdir
    exit /b 2
)
powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File "%host_tools%Extract.ps1" %* || goto failed
pushd "%KW_CASE_DIR%" || goto failed
cl /nologo /TC /W3 /Od /Z7 /MTd /D_DEBUG %CASE_CFLAGS% @Case.rsp /Fe:Case.exe /link /DEBUG
if errorlevel 1 (popd & goto failed)
rem Bound the run: an unpatched writeback path can spin or wait indefinitely.
powershell.exe -NoProfile -NonInteractive -Command ^
 "$ErrorActionPreference='Stop'; $i=[Diagnostics.ProcessStartInfo]::new((Resolve-Path .\Case.exe).Path,[string]$env:KW_CASE_ARG); $i.UseShellExecute=$false;" ^
 "$p=[Diagnostics.Process]::Start($i); if(!$p.WaitForExit(20000)){$p.Kill(); $p.WaitForExit(); exit 124}; exit $p.ExitCode"
set "rc=%errorlevel%"
popd
if not "%rc%"=="0" goto retain
rmdir /s /q "%KW_CASE_DIR%"
exit /b 0
:failed
set "rc=2"
:retain
echo Case files: "%KW_CASE_DIR%" 1>&2
exit /b %rc%
:usage
echo Run the issue's Run.cmd from a native VS developer prompt. 1>&2
exit /b 2
