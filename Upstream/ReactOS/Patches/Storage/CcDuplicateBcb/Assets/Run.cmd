@echo off
setlocal
set "KW_CASE_ARG="
set "bcb=%~dp0..\..\FAT32VolumeFlush\Tests\Host"
call "%~dp0..\..\..\Shared\Host\Run.cmd" "%bcb%\Bcb.c" "%bcb%\Bcb.ps1" %*
exit /b %errorlevel%
