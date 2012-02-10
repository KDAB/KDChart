include( $${TOP_SOURCE_DIR}/qtests/qtests.pri )

SOURCES += main.cpp
QT += xml
RESOURCES = Data.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
