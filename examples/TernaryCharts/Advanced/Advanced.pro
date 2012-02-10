TARGET = TernaryChartsAdvanced

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

unix:static:POST_TARGETDEPS += $${TOP_BUILD_DIR}/lib/*.a

LIBS        += -L$${TOP_BUILD_DIR}/lib -l$$KDCHARTLIB -l$$TESTTOOLSLIB

HEADERS += mainwindow.h
FORMS   += mainwindow.ui
SOURCES += main.cpp mainwindow.cpp

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
