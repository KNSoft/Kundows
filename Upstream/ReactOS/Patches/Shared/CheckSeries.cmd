@echo off
setlocal EnableExtensions DisableDelayedExpansion
for %%D in ("%~dp0..\..\..\..") do set "root=%%~fD"
set "patches=%root%\Upstream\ReactOS\Patches"
set "src=%root%\Upstream\ReactOS\Source"
set "pin="
for /f %%H in ('git -C "%root%" rev-parse :Upstream/ReactOS/Source') do set "pin=%%H"
if not defined pin exit /b 1
set "work=%TEMP%\Kundows-Series-%RANDOM%-%RANDOM%"
mkdir "%work%" || exit /b 1
set "GIT_INDEX_FILE=%work%\index"
call :check
set "rc=%errorlevel%"
rmdir /s /q "%work%"
exit /b %rc%

:check
for %%A in (amd64 i386 arm64) do (
    call :queue %%A || exit /b 1
    call :queue %%A --image-fixes || exit /b 1
)
rem These two cache repairs must also work on their own in either order.
set "mm=Storage/MMFlushWriteErrors/MmFlushStatus.patch"
set "cc=Storage/FAT32VolumeFlush/CcBcbOwnership.patch"
>"%work%\list" echo %mm%
>>"%work%\list" echo %cc%
call :roundTrip || exit /b 1
copy /y "%work%\tree" "%work%\first" >nul || exit /b 1
>"%work%\list" echo %cc%
>>"%work%\list" echo %mm%
call :roundTrip || exit /b 1
fc /b "%work%\first" "%work%\tree" >nul || exit /b 1
echo Cache repairs: same tree in either order.
exit /b 0

:queue
call "%root%\Build\ReactOSSeries.cmd" List %* >"%work%\list" || exit /b 1
set "arch="
set "extra="
if "%~1"=="arm64" set "arch=ARM64"
if "%~2"=="--image-fixes" set "extra=ImageFixes"
(for /f "usebackq eol=# tokens=1,2" %%A in ("%patches%\Series.tsv") do for %%C in (All %arch% %extra%) do if "%%A"=="%%C" echo %%B) >"%work%\expected"
fc /b "%work%\list" "%work%\expected" >nul || exit /b 1
call :roundTrip || exit /b 1
echo %*: apply, recognize and reverse OK
type "%work%\tree"
exit /b 0

:roundTrip
git -C "%src%" read-tree %pin% || exit /b 1
set "reverse="
for /f "usebackq delims=" %%P in ("%work%\list") do call :apply "%%P" || exit /b 1
if not defined reverse exit /b 1
git -C "%src%" write-tree >"%work%\tree" || exit /b 1
for %%P in (%reverse%) do git -C "%src%" apply --cached -R --check --whitespace=nowarn "%patches%\%%P" || exit /b 1
for %%P in (%reverse%) do git -C "%src%" apply --cached -R --whitespace=nowarn "%patches%\%%P" || exit /b 1
git -C "%src%" diff-index --cached --quiet %pin% --
exit /b %errorlevel%

:apply
git -C "%src%" apply --cached --whitespace=nowarn "%patches%\%~1" || exit /b 1
set "reverse=%~1 %reverse%"
exit /b 0
