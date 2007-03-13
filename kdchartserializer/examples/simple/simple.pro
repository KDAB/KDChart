KDCHART_PATH = ../../..

TARGET = serializeSimple

include( $$KDCHART_PATH/kdchartserializer/examples/examples.pri )

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui
RESOURCES = LineChart.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
