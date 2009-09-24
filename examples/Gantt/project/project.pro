KDCHART_PATH = ../../..

include( $$KDCHART_PATH/examples/examples.pri )

QT+=xml

HEADERS += mainwindow.h projectmodel.h
SOURCES += main.cpp mainwindow.cpp projectmodel.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
