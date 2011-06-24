@echo off
rem This file was generated automatically.
rem Please edit generate-configure.sh rather than this file.

set PRODUCT_CAP=KDCHART
set product_low=kdchart
set Product_mix=KDChart
set Product_Space="KD Chart"

set VERSION=2.5.0

set INSTALLATION_SUPPORTED=true
set STATIC_BUILD_SUPPORTED=true

set PACKSCRIPTS_DIR=../admin/packscripts

set shared=yes
set debug=no
set release=no
set designer_plugins_in_debug=no
set prefix=
set unittests=no

if exist %PACKSCRIPTS_DIR% (
    set unittests=yes
    goto :CheckLicenseComplete
)

if exist .license.accepted goto :CheckLicenseComplete

set license_file=

if exist LICENSE.GPL.txt (
    if exist LICENSE.US.txt (
        if exist LICENSE.txt (
            echo.
            echo Please choose your license.
            echo.
            echo Type 1 for the GNU General Public License ^(GPL^).
            echo Type 2 for the %Product_Space% Commercial License for USA/Canada.
            echo Type 3 for the %Product_Space% Commercial License for anywhere outside USA/Canada.
            echo Anything else cancels.
            echo.
            set /p license=Select:
		)
    ) else (
        license=1
    )
) else (
    if exist LICENSE.US.txt (
        license=2
    ) else (
        if exist LICENSE.txt (
            license=3
        ) else (
            echo "Couldn't find license file, aborting"
            exit /B 1
        )
    )
)

if "%license%" == "1" (
    set license_name="GNU General Public License (GPL)"
    set license_file=LICENSE.GPL.txt
	goto :CheckLicense
) else (
    if "%license%" == "2" (
        set license_name="%Product_Space% USA/Canada Commercial License"
        set license_file=LICENSE.US.txt
        goto :CheckLicense
    ) else (
        if "%license%" == "3" (
            set license_name="%Product_Space% Commercial License"
            set license_file=LICENSE.txt
            goto :CheckLicense
        ) else (
            exit /B 1
        )
    )
)

:CheckLicense
echo.
echo License Agreement
echo.
echo You are licensed to use this software under the terms of
echo the %license_name%.
echo.
echo Type '?' to view the %license_name%.
echo Type 'yes' to accept this license offer.
echo Type 'no' to decline this license offer.
echo.
set /p answer=Do you accept the terms of this license?

if "%answer%" == "no" goto :CheckLicenseFailed
if "%answer%" == "yes" (
    echo. > .license.accepted
    goto :CheckLicenseComplete
)
if "%answer%" == "?" more %license_file%
goto :CheckLicense

:CheckLicenseFailed
echo You are not licensed to use this software.
exit /B 1

:CheckLicenseComplete

if "%QTDIR%" == "" (
  rem This is the batch equivalent of QTDIR=`qmake -query QT_INSTALL_PREFIX`...
  for /f "tokens=*" %%V in ('qmake -query QT_INSTALL_PREFIX') do set QTDIR=%%V
)

if "%QTDIR%" == "" (
  echo You need to set QTDIR or add qmake to the PATH
  exit /B 1
)

echo Qt found: %QTDIR%

del /Q /S Makefile* 2> NUL
del /Q /S debug 2> NUL
del /Q /S release 2> NUL
if exist src\src.pro (
    del /Q lib 2> NUL
    del /Q bin 2> NUL
)
del .qmake.cache 2> NUL

echo. > .qmake.cache

:Options
if "%1" == ""          goto :EndOfOptions

if "%1" == "-prefix"   goto :Prefix
if "%1" == "/prefix"   goto :Prefix

if "%1" == "-override-version"  goto :OverrideVersion
if "%1" == "/override-version"  goto :OverrideVersion

if "%1" == "-unittests"    goto :Unittests
if "%1" == "/unittests"    goto :Unittests

if "%1" == "-no-unittests" goto :NoUnittests
if "%1" == "/no-unittests" goto :NoUnittests

if "%1" == "-designer-plugins" goto :DesignerPluginsInDebug
if "%1" == "/designer-plugins" goto :DesignerPluginsInDebug

if "%1" == "-shared"   goto :Shared
if "%1" == "/shared"   goto :Shared

if "%1" == "-static"   goto :Static
if "%1" == "/static"   goto :Static

if "%1" == "-release"  goto :Release
if "%1" == "/release"  goto :Release

if "%1" == "-debug"    goto :Debug
if "%1" == "/debug"    goto :Debug

if "%1" == "-help"     goto :Help
if "%1" == "/help"     goto :Help
if "%1" == "--help"    goto :Help
if "%1" == "/?"        goto :Help

echo Unknown option: %1
goto :usage

:OptionWithArg
shift
:OptionNoArg
shift
goto :Options

:Prefix
    if "%INSTALLATION_SUPPORTED%" == "true" (
      set prefix="%2"
      goto :OptionWithArg
    ) else (
      echo Installation not supported, -prefix option ignored
      goto :OptionWithArg
rem   goto :usage
    )
:OverrideVersion
    set VERSION=%2
    goto :OptionWithArg
:Unittests
    set unittests=yes
    goto :OptionNoArg
:NoUnittests
    set unittests=no
    goto :OptionNoArg
:DesignerPluginsInDebug
    set designer_plugins_in_debug=yes
    goto :OptionNoArg
:Shared
    set shared=yes
    goto :OptionNoArg
:Static
if "%STATIC_BUILD_SUPPORTED%" == "true" (
    set shared=no
    goto :OptionNoArg
) else (
  echo Static build not supported, -static option not allowed
  goto :usage
)
:Release
    set release=yes
    goto :OptionNoArg
:Debug
    set debug=yes
    goto :OptionNoArg
:Help
    goto :usage

:EndOfOptions

if exist %PACKSCRIPTS_DIR% (
    echo.
    echo Creating include directory...
    rem perl %PACKSCRIPTS_DIR%/makeincludes.pl > makeincludes.log 2>&1
    if errorlevel 1 (
	echo Failed to run %PACKSCRIPTS_DIR%/makeincludes.pl
	goto :CleanEnd
    )
    del makeincludes.log
)

if "%release%" == "yes" (
    if "%debug%" == "yes" (
        echo CONFIG += debug_and_release build_all >> .qmake.cache
	set release="yes (combined)"
	set debug="yes (combined)"
    ) else (
        echo CONFIG += release >> .qmake.cache
        echo CONFIG -= debug >> .qmake.cache
	echo CONFIG -= debug_and_release >> .qmake.cache
    )
) else (
    if "%debug%" == "yes" (
        echo CONFIG -= release >> .qmake.cache
	echo CONFIG += debug >> .qmake.cache
	echo CONFIG -= debug_and_release >> .qmake.cache
    ) else (
	echo CONFIG += debug_and_release build_all >> .qmake.cache
	set release="yes (combined)"
	set debug="yes (combined)"
    )
)

if "%shared%" == "yes" (
    echo CONFIG += shared >> .qmake.cache
) else (
    echo CONFIG += static >> .qmake.cache
    rem This is needed too, when Qt is static, otherwise it sets -DQT_DLL and linking fails.
    rem echo CONFIG += qt_static >> .qmake.cache
)

echo CONFIG += warn_on >> .qmake.cache

rem Treat warnings as errors
echo win32-msvc*:QMAKE_CXXFLAGS += -WX >> .qmake.cache
echo win32-g++*:QMAKE_CXXFLAGS += -Wer >> .qmake.cache

if "%unittests%" == "yes" (
    echo CONFIG += unittests >> .qmake.cache
)

if "%designer_plugins_in_debug%" == "yes" (
    echo CONFIG += designer_plugins_in_debug >> .qmake.cache
)

set default_prefix=C:\KDAB\%Product_mix%-%VERSION%

if "%prefix%" == "" (
    set prefix="%default_prefix%"
)
set prefixes=%prefix:\=\\%
echo %PRODUCT_CAP%_INSTALL_PREFIX = %prefixes% >> .qmake.cache


echo VERSION=%VERSION% >> .qmake.cache
echo CONFIG += %product_low%_target >> .qmake.cache

if exist "%QTDIR%\include\Qt\private" (
    echo CONFIG += have_private_qt_headers >> .qmake.cache
    echo INCLUDEPATH += %QTDIR%/include/Qt/private >> .qmake.cache
) else (
    rem echo QTDIR must point to an installation that has private headers installed.
    rem echo Some features will not be available.
)

echo %Product_mix% v%VERSION% configuration:
echo.
echo   Debug...................: %debug% (default: combined)
echo   Release.................: %release% (default: combined)
if "%STATIC_BUILD_SUPPORTED%" == "true" (
  echo   Shared build............: %shared% (default: yes)
)
echo   Compiled-In Unit Tests..: %unittests% (default: no)
echo   Build designer plugins..: %designer_plugins_in_debug% (default: release mode only)
echo.

rem Make a copy so that each run of qmake on $product.pro starts clean
copy .qmake.cache .confqmake.cache

%QTDIR%\bin\qmake %product_low%.pro -recursive "%PRODUCT_CAP%_BASE=%CD%"

if errorlevel 1 (
    echo qmake failed
    goto :CleanEnd
)

echo Ok, now run nmake (for Visual Studio) or mingw32-make (for mingw) to build the framework.
goto :end

:usage
IF "%1" NEQ "" echo %0: unknown option "%1"
echo usage: %0 [options]
echo where options include:
if "%INSTALLATION_SUPPORTED%" == "true" (
  echo.
  echo   -prefix <dir> 
  echo       set installation prefix to <dir>, used by make install
)
echo.
echo   -release / -debug
echo       build in debug/release mode
if "%STATIC_BUILD_SUPPORTED%" == "true" (
  echo.
  echo   -static / -shared
  echo       build static/shared libraries
)
echo.
echo   -unittests / -no-unittests
echo       enable/disable compiled-in unittests
echo.
echo   -designer-plugins
echo 	   Build designer plugins. By default, this is only done for release builds, as the
echo       designer plugins require a Qt Designer built in debug mode.
echo.

:CleanEnd
del .qmake.cache

:end
