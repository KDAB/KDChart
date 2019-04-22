TARGET = WidgetAdvanced

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

FORMS   = mainwindow.ui
SOURCES = main.cpp mainwindow.cpp
HEADERS = mainwindow.h

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
