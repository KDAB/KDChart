TEMPLATE	= app
TARGET		= SignalCompressorTest
CONFIG		+= qtestlib

KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}

LIBS		+= -L../../lib -lkdchart
INCLUDEPATH	+= ../../src
SOURCES += main.cpp

# FIXME make work on windows (Mirko)
test.target = test
test.commands = LD_LIBRARY_PATH=$$KDCHART_PATH/lib ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test
