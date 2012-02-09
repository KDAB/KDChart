
# This file is included by all of the examples' *.pro files.

# Note: KDCHART_PATH is set in the calling .pro file,
#       before examples.pri is included

# needed for the export macros.
CONFIG( static ) {
  DEFINES += KDCHART_STATICLIB
}

# 3. Adjust the paths and LIBS according to KDCHART_PATH.
#    Note: KDCHART_PATH is set in the calling .pro file,
#          before examples.pri is included
INCLUDEPATH += \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/include/KDChart \
            $$KDCHART_PATH/include/KDGantt \
            $$KDCHART_PATH/examples/tools
DEPENDPATH += \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools

unix: {
  !macx: LIBEXT = so
  macx: LIBEXT = dylib
  CONFIG( static ) {
    LIBEXT = a
  }
  # qmake insists on passing Qt's -L path first, where there could be a different version of our own
  # libraries. Therefore we pass our own libraries like object files (without -l!) and with full path.
  LIBS += $${KDCHART_PATH}/lib/lib$${KDCHARTLIB}.$${LIBEXT}  $${KDCHART_PATH}/lib/lib$${TESTTOOLSLIB}.$${LIBEXT}
}
# The problem is less important on Windows and also harder to fix, so do it the usual way
win32: LIBS += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB -l$$TESTTOOLSLIB


# 4. Miscellaneous settings:
QT += svg

unix {
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
  UI_DIR = .ui
}
!unix {
  MOC_DIR = _moc
  OBJECTS_DIR = _obj
  UI_DIR = _ui
}
