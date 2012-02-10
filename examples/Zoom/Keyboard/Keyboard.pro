TARGET = ZoomKeyboard

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

FORMS   = mainwindow.ui
SOURCES = main.cpp mainwindow.cpp zoomwidget.cpp
HEADERS = mainwindow.h zoomwidget.h

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
