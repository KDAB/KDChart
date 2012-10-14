@echo off

cd %1
set OLD_PATH=%PATH%
set PATH=..\..\lib;%PATH%

if (%2)==() (
  set TEST_EXE=%1.exe
) else (
  set TEST_EXE=%2\%1.exe
)

echo Running %1 %CD%\%TEST_EXE%
%TEST_EXE%

set RETURNCODE=%ERRORLEVEL%
echo Test returned with %RETURNCODE%

set PATH=%OLD_PATH%
cd ..

exit /B %RETURNCODE%