TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

include (../../g++.pri )

# Input
SOURCES += main.cpp
CONFIG+=qtestlib

test.target = test
test.commands = ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test
