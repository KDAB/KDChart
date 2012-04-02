include( $${TOP_SOURCE_DIR}/examples/examples.pri )

SOURCES += main.cpp MainWidget.cpp Model.cpp

HEADERS += MainWidget.h Model.h

FORMS = Controls.ui

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
