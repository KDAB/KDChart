KDCHART_PATH = ../../..

include( $$KDCHART_PATH/examples/examples.pri )

TARGET = LeveyJenningsSimple

SOURCES= main.cpp

QT += svg

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
