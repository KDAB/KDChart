KDCHART_PATH = ../..

TARGET = DataValueTexts

include( $$KDCHART_PATH/examples/examples.pri )

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui
RESOURCES = DataValueTexts.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
