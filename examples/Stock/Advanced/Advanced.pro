KDCHART_PATH = ../../..

TARGET = StockDiagram

include( $$KDCHART_PATH/examples/examples.pri )

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui
RESOURCES = Resources.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
