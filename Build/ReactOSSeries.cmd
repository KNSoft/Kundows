@echo off
setlocal EnableExtensions DisableDelayedExpansion
set "mode=%~1"
if /i not "%mode%"=="Apply" if /i not "%mode%"=="List" exit /b 2
if not "%~2"=="amd64" if not "%~2"=="i386" if not "%~2"=="arm64" exit /b 2
if not "%~3"=="" if /i not "%~3"=="--image-fixes" exit /b 2
if not "%~4"=="" exit /b 2
set "arch=%~2"
set "extra=%~3"
for %%D in ("%~dp0..\Upstream\ReactOS") do set "up=%%~fD"
set "base=%up%\Patches"
set "pending="
set "selected="
if not exist "%base%\Series.tsv" exit /b 1
for /f "usebackq eol=# tokens=1,2,*" %%A in ("%base%\Series.tsv") do (
    call :row "%%A" "%%B" "%%C" || exit /b 1
)
if not defined selected exit /b 1
if defined pending git -C "%up%\Source" apply --whitespace=nowarn %pending% || exit /b 1
exit /b 0

:row
if "%~2"=="" goto invalid
if not "%~3"=="" goto invalid
for /f "tokens=1,2,3,* delims=/" %%I in ("%~2") do (
    if "%%J"=="" goto invalid
    if "%%K"=="" goto invalid
    if not "%%L"=="" goto invalid
    if /i "%%I"=="Shared" goto invalid
    if not exist "%base%\%%I\%%J\README.md" goto invalid
)
if /i not "%~x2"==".patch" goto invalid
if not exist "%base%\%~2" goto invalid
if "%~1"=="ARM64" (
    if not "%arch%"=="arm64" exit /b 0
) else if "%~1"=="ImageFixes" (
    if not defined extra exit /b 0
) else if not "%~1"=="All" goto invalid
set "selected=1"
if /i "%mode%"=="List" (
    echo %~2
    exit /b 0
)
git -C "%up%\Source" apply --quiet --reverse --check --whitespace=nowarn "%base%\%~2" && exit /b 0
set "pending=%pending% "%base%\%~2""
exit /b 0

:invalid
echo Invalid Series.tsv entry: "%~1" "%~2" "%~3". 1>&2
exit /b 1
