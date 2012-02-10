TARGET = serializeSimple

include( $${TOP_SOURCE_DIR}/kdchartserializer/examples/examples.pri )

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
