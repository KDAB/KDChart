KDCHART_PATH = ../../..

include( $$KDCHART_PATH/examples/examples.pri )

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
RESOURCES = ModelView.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
