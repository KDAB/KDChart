TARGET = AxisLabels

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

HEADERS= mainwindow.h AdjustedCartesianAxis.h
SOURCES= main.cpp mainwindow.cpp AdjustedCartesianAxis.cpp
FORMS  = mainwindow.ui
RESOURCES = BarChart.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
