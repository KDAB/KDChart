KDCHART_PATH = ../../..

TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../../include ../../../src ../../../src/Gantt
INCLUDEPATH += . ../../../include

LIBS+= -L$$KDCHART_PATH/lib -l$$KDCHARTLIB

# Input
SOURCES += main.cpp
