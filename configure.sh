#!/bin/bash

PRODUCT=KDCHART
Product=KDChart
product=kdchart

VERSION=2.2.1

default_prefix=/usr/local/KDAB/$Product-$VERSION

hide_symbols=yes
shared=yes
debug=no
release=yes
prefix=
# unittests=no

# [ -d packscripts ] && unittests=yes

function die {
    echo "$1" 1>&2
    exit 1
}

function check_license {

    [ -d packscripts ] && return 0 
    [ -f .license.accepted ] && return 0

    if [ -f LICENSE.GPL ] ; then
	license_name="GNU General Public License (GPL)"
	license_file=LICENSE.GPL

    elif [ -f LICENSE.US -a -f LICENSE ] ; then
	license_name="$Product Commercial License"

	echo
	echo "Please choose your region."
	echo
	echo "Type 1 for North or South America."
	echo "Type 2 for anywhere outside North and South America."
	echo "Anything else cancels."
	echo
	echo -n "Select: "
	read region

	if [ "$region" = "1" ]; then
	    license_file=LICENSE.US
	elif [ "$region" = "2" ]; then
	    license_file=LICENSE
	else 
	    return 1
	fi
	
    elif [ -f LICENSE.US ] ; then
	license_name="$Product Commercial License"
	license_file=LICENSE.US

    elif [ -f LICENSE ] ; then 
	license_name="$Product Commercial License"
	license_file=LICENSE
    else
	die "Couldn't find license file, aborting"
    fi

    while true ; do
	cat <<EOF

License Agreement

You are licensed to use this software under the terms of
the $license_name.

Type '?' to view the $license_name
Type 'yes' to accept this license offer.
Type 'no' to decline this license offer.

Do you accept the terms of this license?
EOF
        read answer

	[ "$answer" = "no" ]  && return 1
	[ "$answer" = "yes" ] && touch .license.accepted && return 0
	[ "$answer" = "?" ]   && more $license_file
    done
}

if ! check_license ; then
    die "You are not licensed to use this software."
fi

function usage {
    [ -z "$1" ] || echo "$0: unknown option \"$1\"" 1>&2
    echo "usage: $0 [options]" 1>&2
    cat <<EOF 1>&2
where options include:

  -prefix <path>
      install $Product into <path>

  -release / -debug
      build in debug/release mode

  -static / -shared
      build static/shared libraries

EOF
    exit 1
}

if [ -z "$QTDIR" ] ;  then
    QTDIR="$(qmake -query QT_INSTALL_PREFIX)"
    if [ $? -ne 0 ] ; then
	QTDIR=
    fi
fi

[ -z "$QTDIR" ] && die "You need QTDIR defined, or qmake in the PATH"


# QTDIR is set, so we can test if shared libs are available.
# Note: This default setting can be overwritten by command line param.
if [ -f "$QTDIR"/lib/libQtGui.so ] ; then
    shared=yes
elif [ -f "$QTDIR"/lib/libQtGui.so.*.debug ] ; then
    shared=yes
elif [ -f "$QTDIR"/lib/QtGui*dylib ] ; then
    shared=yes
else
    echo "No shared QT lib found."
    shared=no
    hide_symbols=no
fi


while [ $# -ne 0 ] ; do
    case "$1" in
	-prefix)
	    shift
            if [ $# -eq 0 ] ; then
		    echo "-prefix needs an argument" 2>&1
		    usage
	    fi
            prefix="$1"
	    ;;
#        -no-hide-symbols)
#            hide_symbols=no
#            ;;
#        -hide-symbols)
#            hide_symbols=yes
#            ;;
#	-no-unittests)
#	    unittests=no
#	    ;;
#	-unittests)
#	    unittests=yes
#	    ;;
        -shared)
            shared=yes
            ;;
        -static)
            shared=no
            ;;
        -debug)
            debug=yes
            release=no
            ;;
        -release)
            debug=no
            release=yes
            ;;
        *)
            usage "$1"
            ;;
    esac
    shift
done

find . -name debug -o -name release -o -name Makefile\* | xargs rm -rf

if [ -f src/src.pro ] ; then
    rm -rf lib bin
fi

if [ -d packscripts ] ; then
    echo
    echo -n "Creating include directory..."
    perl packscripts/makeincludes.pl > makeincludes.log 2>&1 || die "Failed to run packscripts/makeincludes.pl"
    rm makeincludes.log
    echo done
    echo
fi

if [ -z "$prefix" ] ; then
    prefix="$default_prefix"
fi

echo -n > ".qmake.cache"
(
    echo "CONFIG += ${product}_target"

# The following disabled to make debug builds work again:
#    echo '!contains($$list($$[QT_VERSION]), 4.2.*):CONFIG += debug_and_release build_all'
#    [ "$debug" = "yes"   ] && echo "else:CONFIG -=release += debug"
#    [ "$release" = "yes" ] && echo "else:CONFIG -=debug += release"

    if [ "$debug" = "yes"   ] ; then
        echo "CONFIG -=release"
        echo "CONFIG += debug"
    fi
    if [ "$release" = "yes" ] ; then
        echo "CONFIG -=debug"
        echo "CONFIG += release"
    fi

    [ "$hide_symbols" = "yes" ] &&  echo "CONFIG += hide_symbols"
#    [ "$unittests" = "yes" ] && echo "CONFIG += unittests"
    if [ "$shared" = "yes" ] ; then
        echo "CONFIG += shared"
    else
        echo "CONFIG += static staticlib"
    fi

    if [ -d "$QTDIR/include/Qt/private" ] ; then
	echo "CONFIG += have_private_qt_headers"
	echo "INCLUDEPATH += $QTDIR/include/Qt/private"
    #else
        #echo "QTDIR must point to an installation that has private headers installed."
        #echo "Some features will not be available."    
    fi

    echo "${PRODUCT}_INSTALL_PREFIX = $prefix"

    # set KDCHARTDIR to enable making the plug-ins from Autobuild autotools
    if [ "x$KDCHARTDIR" = "x" ] ; then
        echo "KDCHARTDIR=`pwd`"
    fi
) >> ".qmake.cache"

cp ".qmake.cache" ".confqmake.cache"

cat <<EOF 1>&2
$Product v$VERSION configuration:

  Install Prefix.............: $prefix
    (default: $default_prefix)
  Debug......................: $debug (default: no)
  Release....................: $release (default: yes)
  Shared build...............: $shared (default: yes)
  Restricted symbol export
    (shared build only)......: $hide_symbols (default: yes)

EOF

$QTDIR/bin/qmake $product.pro || die "qmake failed"
echo "Ok, now run make, then make install to install into $prefix"
