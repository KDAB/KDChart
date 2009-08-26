KDCHART_PATH = ../../..

TARGET = LeveyJenningsSimple

include( $$KDCHART_PATH/examples/examples.pri )

QT+=svg
SOURCES= main.cpp

QT += svg

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
