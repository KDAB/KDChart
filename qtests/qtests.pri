# 1. Work around a qmake bug:
#
#    The bug would produce invalid *.exe files when KD Chart is linked
#    statically and examples are build right after building the lib.
staticlib {
  CONFIG -= staticlib
  DEFINES += KDCHART_STATICLIB
  DEFINES += KDCHARTSERIALIZER_STATICLIB
}


CONFIG+=qtestlib
TEMPLATE = app
INCLUDEPATH +=  ../../examples/tools ../../include \
	../../kdchartserializer/include
DEPENDPATH += . ../../src ../../lib ../../examples/tools ../../include \
	../../kdchartserializer/include \
	../../kdchartserializer/src


test.target = test
test.commands = ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test

LIBS += -L../../lib -l$$KDCHARTSERIALIZERLIB -l$$KDCHARTLIB -l$$TESTTOOLSLIB

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

