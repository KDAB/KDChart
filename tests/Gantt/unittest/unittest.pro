include(../test.pri)

TEMPLATE = app
TARGET =
DEPENDPATH += . ../../include
INCLUDEPATH += . ../../include
CONFIG += console
macx:CONFIG -= app_bundle

!win32:LIBS+= -L$$QMAKE_RPATH$$PWD/../../lib 
LIBS += -L../../lib -l$$KDGANTTLIB


message($$LIBS)
# Input
SOURCES += main.cpp
