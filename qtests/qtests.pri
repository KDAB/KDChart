# 1. Work around a qmake bug:
#
#    The bug would produce invalid *.exe files when KD Chart is linked
#    statically and examples are build right after building the lib.
staticlib {
  CONFIG -= staticlib
  DEFINES += KDCHART_STATICLIB
}


# 2. Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
#    to avoid name clashes between debug/non-debug versions of the
#    KD Chart library:
KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}


CONFIG+=qtestlib
TEMPLATE = app
INCLUDEPATH +=  ../../examples/tools ../../include
DEPENDPATH += . ../../src ../../lib ../../examples/tools ../../include

test.target = test
test.commands = ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test

LIBS += -L../../lib -l$$KDCHARTLIB -ltesttools

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

