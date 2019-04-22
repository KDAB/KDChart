include( $${TOP_SOURCE_DIR}/examples/examples.pri )

HEADERS= mainwindow.h \
         derivedaddlegenddialog.h
SOURCES= main.cpp \
         mainwindow.cpp \
         derivedaddlegenddialog.cpp
FORMS  = mainwindow.ui \
         addlegenddialog.ui
RESOURCES = Legends.qrc

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
