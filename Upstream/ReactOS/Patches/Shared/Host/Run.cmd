@echo off
setlocal EnableExtensions DisableDelayedExpansion
if "%~1"=="" goto help
if /i "%~1"=="--help" goto help
set "shared=%~dp0"
set "case_dir=%TEMP%\Kundows-Case-%RANDOM%-%RANDOM%"
mkdir "%case_dir%" || exit /b 1
python -B "%~dp0PrepareCase.py" "%case_dir%" %* || goto failed
pushd "%case_dir%" || goto failed
call "%shared%Compile.cmd" @Case.rsp /Fe:Case.exe
if errorlevel 1 (popd & goto failed)
set "args="
if "%~1"=="bcb-life" set "args=--lifetime"
if "%~1"=="bcb-owner" set "args=--ownership"
if "%~1"=="fat-volume" set "args=--volume"
rem Handoff/wait regressions can deadlock. Preserve the original execution deadline.
python -c "import subprocess,sys; sys.exit(subprocess.run(sys.argv[1:],timeout=20).returncode)" .\Case.exe %args%
set "rc=%errorlevel%"
popd
if not "%rc%"=="0" goto retain
rmdir /s /q "%case_dir%"
exit /b 0
:failed
set "rc=1"
:retain
echo Case files: "%case_dir%" 1>&2
exit /b %rc%
:help
echo Usage: Run.cmd suite [--stock] [--source tree] [--overlay tree] [--mm-only]
echo Use a native VS developer prompt. Set CASE_CFLAGS to override compiler defaults.
python -B "%~dp0PrepareCase.py" --help
exit /b %errorlevel%
