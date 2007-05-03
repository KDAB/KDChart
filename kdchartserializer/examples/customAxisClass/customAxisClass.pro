KDCHART_PATH = ../../..

TARGET = customAxisClass

include( $$KDCHART_PATH/kdchartserializer/examples/examples.pri )

HEADERS= mainwindow.h AdjustedCartesianAxis.h
SOURCES= main.cpp mainwindow.cpp AdjustedCartesianAxis.cpp
FORMS  = mainwindow.ui

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
