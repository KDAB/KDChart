#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T10:02:06
#
#-------------------------------------------------

#include(outdirs.pri)
KDCHART_PATH = ../..

include( $$KDCHART_PATH/examples/examples.pri )


QT       += core gui

TARGET = RealTimeLineChart
TEMPLATE = app


SOURCES += main.cpp\
        Dialog.cpp

HEADERS  += Dialog.h

FORMS    += Dialog.ui

INCLUDEPATH +=  ../IMS_UserInterface/KDChart/include

LIBS += -L../IMS_UserInterface/lib

LIBS += -lkdchart