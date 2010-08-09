KDCHART_PATH = ../../..

TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../../include ../../../src ../../../src/Gantt
INCLUDEPATH += . ../../../include
QT += svg

LIBS+= -L$$KDCHART_PATH/lib -L../../../lib -l$$KDCHARTLIB

# Input
SOURCES += main.cpp
