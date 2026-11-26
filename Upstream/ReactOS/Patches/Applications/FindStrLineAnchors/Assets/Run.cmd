@echo off
setlocal EnableExtensions DisableDelayedExpansion
set "findstr_exe=%SystemRoot%\System32\findstr.exe"
if not "%~1"=="" set "findstr_exe=%~f1"
if not exist "%findstr_exe%" exit /b 2
set "work=%TEMP%\Kundows-FindStr-%RANDOM%-%RANDOM%"
mkdir "%work%" || exit /b 2
set "result=0"
call :check ChunkStart 1 0 /B red
call :check ChunkEnd 1 0 /E red
call :check Regex 0 5 /R /X r.d
call :check TwoWords 0 11 /X "red blue"
rmdir /s /q "%work%"
exit /b %result%

:check
if not exist "%~dp0Inputs\%~1.txt" (
    echo Missing input: %~1 1>&2
    set "result=1"
    exit /b 1
)
set "findstr_args=%4 %5 %6"
rem The original observations used a pipe, not a filename argument. Bound a hung candidate.
type "%~dp0Inputs\%~1.txt" | powershell.exe -NoProfile -NonInteractive -Command ^
 "$ErrorActionPreference='Stop'; $i=[Diagnostics.ProcessStartInfo]::new($env:findstr_exe,$env:findstr_args); $i.UseShellExecute=$false;" ^
 "$p=[Diagnostics.Process]::Start($i); if(!$p.WaitForExit(5000)){$p.Kill(); $p.WaitForExit(); exit 124}; exit $p.ExitCode" >"%work%\stdout"
set "rc=%errorlevel%"
for %%F in ("%work%\stdout") do (
    echo %~1: exit=%rc% stdout-bytes=%%~zF
    if not "%rc%:%%~zF"=="%~2:%~3" set "result=1"
)
exit /b 0
