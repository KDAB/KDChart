TEMPLATE = lib
TARGET = testtools
DESTDIR	= $${TOP_BUILD_DIR}/lib
win32: DLLDESTDIR = $${TOP_BUILD_DIR}/bin
DEFINES += KDCHART_BUILD_TESTTOOLS_LIB
DEPENDPATH += .
INCLUDEPATH += .

#test.target = test
#test.commands = 
#test.depends = $(DESTDIR)/$(TARGET)
#QMAKE_EXTRA_TARGETS += test

# Input
HEADERS += TableModel.h
SOURCES += TableModel.cpp

!static: DEFINES+=QT_SHARED TESTTOOLS_MAKEDLL

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

