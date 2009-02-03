@echo off

set PRODUCT_CAP=KDCHART
set product_low=kdchart
set Product_mix=KDChart

set VERSION=2.2.1

set default_prefix=C:\%Product_mix%-%VERSION%

set shared=yes
set debug=no
set release=no
set prefix=
set unittests=no

if exist packscripts (
    set unittests=yes
    goto :CheckLicenseComplete
)

if exist .license.accepted goto :CheckLicenseComplete

set region=
set license_file=

if exist LICENSE.GPL (
    set license_name="GNU General Public License (GPL)"
    set license_file=LICENSE.GPL
) else (
    if exist LICENSE.US (
	set license_name="%Product_mix% Commercial License"
	if exist LICENSE (
	    echo.
	    echo Please choose your region.
	    echo.
	    echo Type 1 for North or South America.
	    echo Type 2 for anywhere outside North and South America.
	    echo Anything else cancels.
	    echo.
	    set /p region=Select:
	) else (
	    set license_file=LICENSE.US
	)
    ) else (
	if exist LICENSE (
	    set license_name="%Product_mix% Commercial License"
	    set license_file=LICENSE
	) else ( 
	    echo "Couldn't find license file, aborting"
	    exit /B 1
        )
    )
)

if "%license_file%" == "" (
    if "%region%" == "1" (
	set license_file=LICENSE.US
	goto :CheckLicense
    )
    if "%region%" == "2" (
	set license_file=LICENSE
	goto :CheckLicense
    )
    goto :CheckLicenseFailed
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
  echo You need to set QTDIR
  goto :end
)


# QTDIR is set, so we can test if shared libs are available.
# Note: This default setting can be overwritten by command line param.
if exist "%QTDIR%"\lib\QtGui*.dll (
    shared=yes
) else (
    echo "No shared QT lib found."
    shared=no
    hide_symbols=no
)


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

if "%1" == "-unittests"    goto :Unittests
if "%1" == "/unittests"    goto :Unittests

if "%1" == "-no-unittests" goto :NoUnittests
if "%1" == "/no-unittests" goto :NoUnittests

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
    set prefix="%2"
    goto :OptionWithArg
:Unittests
    set unittests=yes
    goto :OptionNoArg
:NoUnittests
    set unittests=no
    goto :OptionNoArg
:Shared
    set shared=yes
    goto :OptionNoArg
:Static
    set shared=no
    goto :OptionNoArg
:Release
    set release=yes
    goto :OptionNoArg
:Debug
    set debug=yes
    goto :OptionNoArg
:Help
    goto :usage

:EndOfOptions

if exist packscripts (
    echo.
    echo Creating include directory...
    perl packscripts/makeincludes.pl > makeincludes.log 2>&1
    if errorlevel 1 (
	echo Failed to run packscripts/makeincludes.pl
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
    echo CONFIG += static staticlib >> .qmake.cache
)

if "%unittests%" == "yes" (
    echo CONFIG += unittests >> .qmake.cache
)

if "%prefix%" == "" (
    set prefix="%default_prefix%"
)
echo %PRODUCT_CAP%_INSTALL_PREFIX = %prefix% >> .qmake.cache

rem set KDCHARTDIR to enable making the plug-ins from Autobuild autotools
if "x%KDCHARTDIR%" == "x" (
    echo KDCHARTDIR="%CD%" >> .qmake.cache
)

echo CONFIG += %product_low%_target >> .qmake.cache

if exist "%QTDIR%\include\Qt\private" (
    echo CONFIG += have_private_qt_headers >> .qmake.cache
    echo INCLUDEPATH += %QTDIR%/include/Qt/private >> .qmake.cache
) else (
    rem echo QTDIR must point to an installation that has private headers installed.
    rem echo Some features will not be available.
)

copy .qmake.cache .confqmake.cache

echo %Product_mix% v%VERSION% configuration:
echo.
echo   Install Prefix..........: %prefix%
echo     (default: %default_prefix%)
echo   Debug...................: %debug% (default: combined)
echo   Release.................: %release% (default: combined)
echo   Shared build............: %shared% (default: yes)
echo   Compiled-In Unit Tests..: %unittests% (default: no)
echo.

%QTDIR%\bin\qmake %product_low%.pro

if errorlevel 1 (
    echo qmake failed
    goto :CleanEnd
)

echo Ok, now run nmake, then nmake install to install into %prefix%
goto :end

:usage
IF "%1" NEQ "" echo %0: unknown option "%1"
echo usage: %0 [options]
echo where options include:
echo. 
echo   -prefix ^<path^>
echo       install $Product_mix% into ^<path^>
echo.
echo   -release / -debug
echo       build in debug/release mode
echo.
echo   -static / -shared
echo       build static/shared libraries
echo.
echo   -unittests / -no-unittests
echo       enable/disable compiled-in unittests
echo.

:CleanEnd
del .qmake.cache

:end
