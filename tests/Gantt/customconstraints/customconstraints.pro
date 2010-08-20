KDCHART_PATH = ../../..

TARGET = TestCustomConstraints

include( $$KDCHART_PATH/examples/examples.pri )

DEPENDPATH += . ../../../include ../../../src ../../../src/Gantt
INCLUDEPATH += . ../../../include

LIBS+= -L$$KDCHART_PATH/lib -l$$KDCHARTLIB

# Input
SOURCES += main.cpp
