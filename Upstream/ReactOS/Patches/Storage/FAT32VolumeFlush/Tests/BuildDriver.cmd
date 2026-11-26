@echo off
setlocal EnableExtensions DisableDelayedExpansion
rem Internal to the two guest-only cases: BuildDriver.cmd source.c output.sys
set "kit=%WindowsSdkDir%Include\%WindowsSDKVersion%km"
set "libdir=%WindowsSdkDir%Lib\%WindowsSDKVersion%km\%VSCMD_ARG_TGT_ARCH%"
if not exist "%kit%\ntifs.h" goto tools
if not exist "%libdir%\ntoskrnl.lib" goto tools
set "cpu="
if "%VSCMD_ARG_TGT_ARCH%"=="x64" set "cpu=/D_AMD64_ /DAMD64"
if "%VSCMD_ARG_TGT_ARCH%"=="x86" set "cpu=/D_X86_ /Di386 /Gz"
if "%VSCMD_ARG_TGT_ARCH%"=="arm64" set "cpu=/D_ARM64_ /DARM64"
if not defined cpu goto tools
if not exist "%~dp2." mkdir "%~dp2." || exit /b 1
cl /nologo /c /TC /kernel /GS /W3 /Od /Z7 /D_DEBUG /DDBG=1 %cpu% /D_WIN32_WINNT=0x0601 /DNTDDI_VERSION=0x06010000 /I"%kit%" %CASE_CFLAGS% /Fo"%~dpn2.obj" "%~f1" || exit /b 1
link /nologo /driver /debug /nodefaultlib /subsystem:native /entry:GsDriverEntry /out:"%~f2" "%~dpn2.obj" /libpath:"%libdir%" ntoskrnl.lib hal.lib BufferOverflowK.lib
exit /b %errorlevel%
:tools
echo This case needs the matching WDK headers and kernel libraries for the current VS developer prompt. 1>&2
exit /b 1
