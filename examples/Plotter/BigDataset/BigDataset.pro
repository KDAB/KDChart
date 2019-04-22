include( $${TOP_SOURCE_DIR}/examples/examples.pri )

INCLUDEPATH += $${TOP_SOURCE_DIR}/src/KDChart \
               $${TOP_SOURCE_DIR}/src/KDChart/Cartesian

SOURCES += main.cpp MainWidget.cpp Model.cpp

HEADERS += MainWidget.h Model.h

FORMS = Controls.ui

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
