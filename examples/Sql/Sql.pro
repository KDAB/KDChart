KDCHART_PATH = ../..

include( $$KDCHART_PATH/examples/examples.pri )

QT+=sql

SOURCES= main.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
