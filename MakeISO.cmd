@echo off
rem Re-enter the same MSVC/RosBE environment for Ninja's remaining prerequisites.
"%~dp0Build.cmd" --bootcd %*
