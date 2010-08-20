KDCHART_PATH = ../../..

TARGET = TestView

include( $$KDCHART_PATH/examples/examples.pri )

DEPENDPATH += . ../../../include ../../../src ../../../src/Gantt
INCLUDEPATH += . ../../../include
QT += svg

# Input
SOURCES += main.cpp
