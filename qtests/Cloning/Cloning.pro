CONFIG+=qtestlib
TEMPLATE = app
INCLUDEPATH +=  ../../examples/tools ../../include
DEPENDPATH += . ../../src ../../lib ../../examples/tools ../../include

test.target = test
test.commands = LD_LIBRARY_PATH=../../lib ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test

# Input
SOURCES += main.cpp
LIBS += -L../../lib -lkdchart -ltesttools
