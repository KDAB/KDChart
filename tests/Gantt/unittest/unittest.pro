KDCHART_PATH = ../../..

TEMPLATE = app
TARGET =
DEPENDPATH += . ../../../include
INCLUDEPATH += . ../../../include
CONFIG += console
QT += svg
macx:CONFIG -= app_bundle

## !win32:LIBS+= -L$$QMAKE_RPATH$$PWD/../../../lib 
LIBS += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB

message($$LIBS)

# Input
SOURCES += main.cpp
