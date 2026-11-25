@echo off
setlocal EnableExtensions DisableDelayedExpansion
for %%R in ("%~dp0..") do set "kw_root=%%~fR\"
set "kw_stage=%~1"
shift /1
if /i "%~1"=="/?" goto help
if /i "%~1"=="--help" goto help
set "kw_platform=%~1"
if not defined kw_platform set "kw_platform=x64"
if /i "%kw_platform%"=="x64" (
    set "kw_platform=x64"
    set "kw_arch=amd64"
    set "kw_vcarch=x64"
) else if /i "%kw_platform%"=="Win32" (
    set "kw_platform=Win32"
    set "kw_arch=i386"
    set "kw_vcarch=x64_x86"
) else if /i "%kw_platform%"=="ARM64" (
    set "kw_platform=ARM64"
    set "kw_arch=arm64"
    set "kw_vcarch=x64_arm64"
) else goto badargs
set "kw_config=%~2"
if not defined kw_config set "kw_config=Debug"
if /i "%kw_config%"=="Release" (set "kw_config=Release") else if /i "%kw_config%"=="Debug" (set "kw_config=Debug") else goto badargs
if not "%~3"=="" if not "%kw_stage%"=="Configure" goto badargs
if not "%~3"=="" if /i not "%~3"=="--image-fixes" goto badargs
if not "%~4"=="" goto badargs

set "kw_src=%kw_root%Upstream\ReactOS\Source"
set "kw_out=%kw_root%OutDir\%kw_arch%\%kw_config%"
set "kw_iso=%kw_root%OutDir\bootcd-%kw_platform%-%kw_config%.iso"
set "kw_solution=%kw_out%\REACTOS.slnx"
set "kw_generator=Visual Studio 18 2026"
set "kw_export_version=0x600"
set "kw_vs="
if not "%kw_src: =%"=="%kw_src%" goto spaces
if exist "%kw_out%\build.ninja" goto wrongcache
if exist "%kw_out%\CMakeCache.txt" (
    for %%L in ("CMAKE_HOME_DIRECTORY:INTERNAL=%kw_src:\=/%" "CMAKE_TOOLCHAIN_FILE:FILEPATH=%kw_src:\=/%/toolchain-msvc.cmake" "CMAKE_GENERATOR:INTERNAL=%kw_generator%" "CMAKE_GENERATOR_PLATFORM:INTERNAL=%kw_platform%" "ARCH:STRING=%kw_arch%" "CMAKE_CONFIGURATION_TYPES:STRING=%kw_config%" "CMAKE_BUILD_TYPE:STRING=%kw_config%") do (
        findstr /i /l /x /c:"%%~L" "%kw_out%\CMakeCache.txt" >nul || goto wrongcache
    )
    if not "%kw_stage%"=="Configure" (
        findstr /i /l /x /c:"DLL_EXPORT_VERSION:STRING=%kw_export_version%" "%kw_out%\CMakeCache.txt" >nul || goto unprepared
    )
    for /f "tokens=1,* delims==" %%I in ('findstr /b /l /c:"CMAKE_GENERATOR_INSTANCE:" "%kw_out%\CMakeCache.txt"') do set "kw_vs=%%J"
) else if not "%kw_stage%"=="Configure" goto unprepared
if not "%kw_stage%"=="Configure" if not exist "%kw_solution%" goto unprepared

set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
if not defined kw_vs for /f "delims=" %%V in ('vswhere.exe -latest -products * -version "[18.0,19.0)" -requires Microsoft.VisualStudio.Component.CoreEditor Microsoft.VisualStudio.Component.VC.Tools.x86.x64 Microsoft.VisualStudio.Component.VC.CMake.Project -property installationPath') do set "kw_vs=%%V"
if not exist "%kw_vs%\Common7\IDE\devenv.com" (
    echo Install Visual Studio 2026 with the C++ desktop workload, CMake tools and a Windows SDK. 1>&2
    exit /b 1
)
set "kw_cmake=%kw_vs%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
if "%kw_stage%"=="Build" goto buildSolution
if "%kw_stage%"=="MakeISO" goto buildImage
if not "%kw_stage%"=="Configure" goto badargs

rem Use the RosBE host programs, without entering its MinGW compiler shell.
set "kw_be=%ROSBE_ROOT%"
if not defined kw_be set "kw_be=%_ROSBE_BASEDIR%"
if not defined kw_be call :registeredRosBE HKCU
if not defined kw_be call :registeredRosBE HKLM /reg:32
if not defined kw_be call :registeredRosBE HKLM /reg:64
if not defined kw_be for %%R in (RosBE.cmd) do set "kw_be=%%~dp$PATH:R"
if not defined kw_be set "kw_be=%SystemDrive%\RosBE"
for %%R in ("%kw_be%") do set "kw_be=%%~fR"
for %%F in (RosBE.cmd bin\bison.exe bin\flex.exe bin\m4.exe share\bison\skeletons\yacc.c) do if not exist "%kw_be%\%%F" (
    echo Missing RosBE file "%kw_be%\%%F". Set ROSBE_ROOT to the installation directory. 1>&2
    exit /b 1
)

call "%kw_vs%\VC\Auxiliary\Build\vcvarsall.bat" %kw_vcarch% || exit /b 1
for %%R in ("%kw_be%") do (
    set "M4=%%~fsR\bin\m4.exe"
    set "BISON_PKGDATADIR=%%~fsR\share\bison"
    set "kw_be_bin=%%~fsR\bin"
)
set "kw_bison=%kw_be_bin:\=/%/bison.exe"
set "kw_flex=%kw_be_bin:\=/%/flex.exe"

if not exist "%kw_src%\.git" git -C "%kw_root%." submodule update --init -- Upstream/ReactOS/Source || exit /b 1
set "kw_pin="
set "kw_head="
for /f %%R in ('git -C "%kw_root%." rev-parse :Upstream/ReactOS/Source') do set "kw_pin=%%R"
for /f %%R in ('git -C "%kw_src%" rev-parse HEAD') do set "kw_head=%%R"
if not defined kw_pin exit /b 1
if /i not "%kw_pin%"=="%kw_head%" (
    echo ReactOS is at %kw_head%; expected %kw_pin%. Check local edits before updating the submodule. 1>&2
    exit /b 1
)

set "kw_pending="
for %%P in (spec2def-cpp-exports freeldr-target-file generated-header-deps vs-media-deps) do call :queuePatch BuildSystem %%P || exit /b 1
for %%P in (blue-vbox-vbe io-sync-status kd-auto-probe rtc-periodic cc-pinned-dirty) do call :queuePatch Runtime %%P || exit /b 1
for %%P in (cc-duplicate-bcb mm-flush-status fastfat-clean-after-flush user-init-unlock monitor-relation-completion monitor-list-completion sort-eof where-scoped notepad-majority-eol) do call :queuePatch Runtime %%P || exit /b 1
if "%kw_arch%"=="arm64" (
    for %%P in (arm64-ntdll arm64-vs-host-tools) do call :queuePatch ARM64 %%P || exit /b 1
)
if /i "%~3"=="--image-fixes" (
    call :queuePatch Runtime mm-flush-collision || exit /b 1
)
if defined kw_pending (
    git -C "%kw_src%" apply --whitespace=nowarn %kw_pending% || exit /b 1
)

echo ReactOS %kw_head%   %kw_generator%   %kw_platform% %kw_config%
echo RosBE: "%kw_be%"   CMake: "%kw_cmake%"
rem The host-tools ExternalProject is first configured by the IDE, in a fresh environment.
"%kw_cmake%" -S "%kw_src%" -B "%kw_out%" -G "%kw_generator%" -A "%kw_platform%" ^
    "-DCMAKE_GENERATOR_INSTANCE:STRING=%kw_vs:\=/%" "-DCMAKE_TOOLCHAIN_FILE:FILEPATH=%kw_src%\toolchain-msvc.cmake" ^
    "-DARCH:STRING=%kw_arch%" "-DCMAKE_CONFIGURATION_TYPES:STRING=%kw_config%" "-DCMAKE_BUILD_TYPE:STRING=%kw_config%" ^
    "-DDLL_EXPORT_VERSION:STRING=%kw_export_version%" ^
    "-DBISON_EXECUTABLE:FILEPATH=%kw_bison%" "-DFLEX_EXECUTABLE:FILEPATH=%kw_flex%" ^
    "-DCMAKE_HOST_TOOLS_EXTRA_ARGS:STRING=-DBISON_EXECUTABLE:FILEPATH=%kw_bison%;-DFLEX_EXECUTABLE:FILEPATH=%kw_flex%" || exit /b 1
if not exist "%kw_solution%" (
    echo CMake did not generate "%kw_solution%". 1>&2
    exit /b 1
)
echo Solution: "%kw_solution%"
exit /b 0

:buildSolution
"%kw_vs%\Common7\IDE\devenv.com" "%kw_solution%" /Build "%kw_config%|%kw_platform%"
exit /b %errorlevel%

:buildImage
"%kw_cmake%" --build "%kw_out%" --config "%kw_config%" --target bootcd || exit /b 1
if not exist "%kw_out%\bootcd.iso" (
    echo Missing upstream image: "%kw_out%\bootcd.iso" 1>&2
    exit /b 1
)
move /y "%kw_out%\bootcd.iso" "%kw_iso%" >nul || exit /b 1
echo BootCD: "%kw_iso%"
exit /b 0

:registeredRosBE
set "kw_uninstall="
for /f "tokens=1,2,*" %%A in ('reg query "%~1\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\ReactOS Build Environment" /v UninstallString %~2 2^>nul') do if /i "%%A"=="UninstallString" set "kw_uninstall=%%C"
if not defined kw_uninstall exit /b 0
set "kw_uninstall=%kw_uninstall:"=%"
for %%U in ("%kw_uninstall%") do if exist "%%~dpURosBE.cmd" set "kw_be=%%~dpU."
exit /b 0

:queuePatch
set "kw_patch=%kw_root%Upstream\ReactOS\Patches\%~1\ReactOS-%~2.patch"
if not exist "%kw_patch%" (
    echo Missing patch "%kw_patch%". 1>&2
    exit /b 1
)
git -C "%kw_src%" apply --quiet --reverse --check --whitespace=nowarn "%kw_patch%" && exit /b 0
set "kw_pending=%kw_pending% "%kw_patch%""
exit /b 0

:wrongcache
echo Incompatible build tree in "%kw_out%". Move it aside and run Configure.cmd again. 1>&2
exit /b 1
:unprepared
echo Run Configure.cmd %kw_platform% %kw_config% first to generate "%kw_solution%". 1>&2
exit /b 1
:spaces
echo ReactOS requires a checkout path without spaces. 1>&2
exit /b 1
:badargs
call :help
exit /b 2
:help
echo Usage: Configure.cmd [x64^|Win32^|ARM64] [Debug^|Release] [--image-fixes]
echo        Build.cmd     [x64^|Win32^|ARM64] [Debug^|Release]
echo        MakeISO.cmd   [x64^|Win32^|ARM64] [Debug^|Release]
echo Defaults: x64 Debug. Build tree: OutDir\amd64\Debug. BootCD: OutDir\bootcd-x64-Debug.iso
exit /b 0
