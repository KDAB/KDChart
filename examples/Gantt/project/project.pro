include( $${TOP_SOURCE_DIR}/examples/examples.pri )

HEADERS += mainwindow.h projectmodel.h
SOURCES += main.cpp mainwindow.cpp projectmodel.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
