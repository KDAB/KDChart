KDCHART_PATH = ../../..

TARGET = TestGfxView

include( $$KDCHART_PATH/examples/examples.pri )

DEPENDPATH += . ../../../include ../../../src/Gantt
INCLUDEPATH += . ../../../include
QT += svg

# Input
SOURCES += main.cpp
