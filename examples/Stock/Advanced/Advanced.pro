TARGET = StockDiagram

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui
RESOURCES = Resources.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
