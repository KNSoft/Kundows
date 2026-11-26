@echo off
rem Compile a user-mode C fixture in the current VS developer prompt.
cl /nologo /TC /W3 /Od /Z7 /MTd /D_DEBUG %CASE_CFLAGS% %* /link /DEBUG
exit /b %errorlevel%
