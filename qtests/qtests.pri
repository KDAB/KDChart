# # needed for the export macros.
staticlib {
  DEFINES += KDCHART_STATICLIB
  DEFINES += KDCHARTSERIALIZER_STATICLIB
}

QT += xml svg

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
win32{
LIB_PATH=../../lib:\$\$PATH
QMAKE_POST_LINK+=set PATH=$$LIB_PATH && $(TARGET)
}
else {
LIB_PATH=../../lib:\$\$LD_LIBRARY_PATH
QMAKE_POST_LINK+=export LD_LIBRARY_PATH=$$LIB_PATH && ./$(TARGET)
}

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

