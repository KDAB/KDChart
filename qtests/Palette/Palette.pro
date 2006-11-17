# Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
# to avoid name clashes between debug/non-debug versions of the
# KD Chart library:
KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}

CONFIG+=qtestlib
TEMPLATE = app
TARGET = Palette
INCLUDEPATH +=  ../../examples/tools ../../include
DEPENDPATH += . ../../src ../../lib ../../examples/tools ../../include

test.target = test
test.commands = LD_LIBRARY_PATH=../../lib ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test

# Input
SOURCES += main.cpp
LIBS += -L../../lib -l$$KDCHARTLIB -ltesttools
