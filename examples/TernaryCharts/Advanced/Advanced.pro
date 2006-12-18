KDCHART_PATH = ../../..

TARGET = TernaryChartsAdvanced

include( $$KDCHART_PATH/examples/examples.pri )

HEADERS += mainwindow.h
FORMS   += mainwindow.ui
SOURCES += main.cpp mainwindow.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
