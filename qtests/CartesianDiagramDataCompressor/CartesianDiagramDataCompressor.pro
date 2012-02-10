include( $${TOP_SOURCE_DIR}/qtests/qtests.pri )

SOURCES += CartesianDiagramDataCompressorTests.cpp
INCLUDEPATH += $${TOP_SOURCE_DIR}/src
DEPENDPATH += $${TOP_SOURCE_DIR}/src

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
