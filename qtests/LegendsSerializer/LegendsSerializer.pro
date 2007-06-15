KDCHART_PATH = ../..
include( ../serializertests.pri )

SOURCES += main.cpp
RESOURCES = Data.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
