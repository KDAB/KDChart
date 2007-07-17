KDCHART_PATH = ../..
include( ../qtests.pri )

SOURCES += CartesianDiagramDataCompressorTests.cpp
INCLUDEPATH += ../../src

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
