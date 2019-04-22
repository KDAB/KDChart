# # needed for the export macros.
CONFIG( static ) {
  DEFINES += KDCHART_STATICLIB
}

QT += xml svg testlib

CONFIG += testlib

TEMPLATE = app
INCLUDEPATH +=  $${TOP_SOURCE_DIR}/examples/tools $${TOP_SOURCE_DIR}/include \
        $${TOP_SOURCE_DIR}/src/KDChart \
        $${TOP_SOURCE_DIR}/include/KDChart

DEPENDPATH += . $${TOP_SOURCE_DIR}/src $${TOP_SOURCE_DIR}/lib $${TOP_SOURCE_DIR}/examples/tools $${TOP_SOURCE_DIR}/include

test.target = test
test.commands = ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test
win32{
LIB_PATH=$${TOP_BUILD_DIR}/lib;%PATH%
#QMAKE_POST_LINK+=set PATH=$$LIB_PATH && $(TARGET)
}
else {
LIB_PATH=$${TOP_BUILD_DIR}/lib:\$\$LD_LIBRARY_PATH
#QMAKE_POST_LINK+=export LD_LIBRARY_PATH=$$LIB_PATH && ./$(TARGET)
}

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

