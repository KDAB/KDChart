KDCHART_PATH = ../../..

TARGET = TernaryChartsAdvanced

include( $$KDCHART_PATH/examples/examples.pri )

unix:static:POST_TARGETDEPS += ../../../lib/*.a

LIBS        += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB -l$$TESTTOOLSLIB

HEADERS += mainwindow.h
FORMS   += mainwindow.ui
SOURCES += main.cpp mainwindow.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
