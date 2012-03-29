include( $${TOP_SOURCE_DIR}/qtests/qtests.pri )

SOURCES += CartesianDiagramDataCompressorTests.cpp
INCLUDEPATH += $${TOP_SOURCE_DIR}/src/KDChart/Cartesian
DEPENDPATH += $${TOP_SOURCE_DIR}/src/KDChart/Cartesian

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
