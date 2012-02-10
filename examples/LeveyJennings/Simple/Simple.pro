TARGET = LeveyJenningsSimple

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

QT+=svg
SOURCES= main.cpp

QT += svg

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
