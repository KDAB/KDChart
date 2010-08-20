KDCHART_PATH = ../../..

TARGET = TestHeaders

include( $$KDCHART_PATH/examples/examples.pri )

DEPENDPATH += . ../../../include ../../../src/Gantt
INCLUDEPATH += . ../../../include
QT += svg

# Input
SOURCES += main.cpp
