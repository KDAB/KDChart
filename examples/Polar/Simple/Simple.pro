KDCHART_PATH = ../../..

TARGET = PolarSimple

include( $$KDCHART_PATH/examples/examples.pri )

SOURCES= main.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
