TEMPLATE = lib

include(../../variables.pri)

TARGET = testtools
DEFINES += KDCHART_BUILD_TESTTOOLS_LIB
DEPENDPATH += .
INCLUDEPATH += .


# Input
HEADERS += TableModel.h
SOURCES += TableModel.cpp

!static: DEFINES+=QT_SHARED TESTTOOLS_MAKEDLL
