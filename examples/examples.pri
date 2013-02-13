
# This file is included by all of the examples' *.pro files.

# needed for the export macros.
CONFIG( static ) {
  DEFINES += KDCHART_STATICLIB
} else {
  DEFINES += TESTTOOLS_DLL
}

INCLUDEPATH += \
            $${TOP_SOURCE_DIR}/include \
            $${TOP_SOURCE_DIR}/include/KDChart \
            $${TOP_SOURCE_DIR}/include/KDGantt \
            $${TOP_SOURCE_DIR}/examples/tools
DEPENDPATH += \
            $${TOP_SOURCE_DIR}/include \
            $${TOP_SOURCE_DIR}/examples/tools

unix {
  !macx: LIBEXT = so
  macx: LIBEXT = dylib
  CONFIG( static ) {
    LIBEXT = a
  }
  # qmake insists on passing Qt's -L path first, where there could be a different version of our own
  # libraries. Therefore we pass our own libraries like object files (without -l!) and with full path.
  LIBS += $${TOP_BUILD_DIR}/lib/lib$${KDCHARTLIB}.$${LIBEXT}  $${TOP_BUILD_DIR}/lib/lib$${TESTTOOLSLIB}.$${LIBEXT}
}
# The problem is less important on Windows and also harder to fix, so do it the usual way
win32: LIBS += -L$${TOP_BUILD_DIR}/lib -l$$KDCHARTLIB -l$$TESTTOOLSLIB


# 4. Miscellaneous settings:
QT += svg

contains(QT_VERSION, ^5\\.[0-9]\\..*):QT += printsupport

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  UI_DIR = .ui
}
!unix {
  debug:MODE=debug
  release:MODE=release
  MOC_DIR = $${MODE}/_moc
  OBJECTS_DIR = $${MODE}/_obj
  UI_DIR = $${MODE}/_ui
}
