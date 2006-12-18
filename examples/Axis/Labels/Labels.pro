KDCHART_PATH = ../../..

include( $$KDCHART_PATH/examples/examples.pri )

TARGET = AxisLabels

HEADERS= mainwindow.h
SOURCES= main.cpp mainwindow.cpp
FORMS  = mainwindow.ui
RESOURCES = BarChart.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
