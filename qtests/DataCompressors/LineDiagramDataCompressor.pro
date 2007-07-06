KDCHART_PATH = ../..
include( ../qtests.pri )

SOURCES += LineDiagramDataCompressorTests.cpp
INCLUDEPATH += ../../src

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
