@echo off
setlocal
echo running test %1 in %2 mode
cd %1
set PATH=..\..\lib;%PATH%
%2\%1.exe
set RETCODE=%ERRORLEVEL%
cd ..
exit /B %RETCODE%
