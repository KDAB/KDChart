
# This file is included by all of the examples' *.pro files.

# 0. Make sure to use the XML option:
#
QT += xml


# 1. Work around a qmake bug:
#
#    The bug would produce invalid *.exe files when KD Chart is linked
#    statically and examples are build right after building the lib.
CONFIG( static ) {
  CONFIG -= staticlib
  DEFINES += KDCHART_STATICLIB
  DEFINES += KDCHARTSERIALIZER_STATICLIB
}

INCLUDEPATH += \
            $${TOP_SOURCE_DIR}/include \
            $${TOP_SOURCE_DIR}/include/KDChart \
            $${TOP_SOURCE_DIR}/kdchartserializer/include
DEPENDPATH += \
            $${TOP_SOURCE_DIR}/include \
            $${TOP_SOURCE_DIR}/kdchartserializer/include
LIBS        += -L$${TOP_BUILD_DIR}/lib -l$$KDCHARTSERIALIZERLIB -l$$KDCHARTLIB


# 4. Miscellaneous settings:
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
