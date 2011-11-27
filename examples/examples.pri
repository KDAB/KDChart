
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
LIBS        += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB -l$$TESTTOOLSLIB


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
