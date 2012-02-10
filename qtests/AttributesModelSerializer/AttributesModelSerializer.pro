include( $${TOP_SOURCE_DIR}/qtests/qtests.pri )

SOURCES += main.cpp
RESOURCES = Data.qrc
QT += xml

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
