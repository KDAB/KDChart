include(../test.pri)

TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../include ../../src
INCLUDEPATH += . ../../include

LIBS+= -L../../lib -l$$KDGANTTLIB

# Input
SOURCES += main.cpp
