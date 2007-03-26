TEMPLATE	= app
TARGET		= Benchmark
KDCHARTLIB = kdchart
CONFIG(debug, debug|release) {
    !unix: KDCHARTLIB = "kdchartd"
}

LIBS		+= -L../../../lib -lkdchart
INCLUDEPATH	+= ../../../include
# INCLUDEPATH	+= ../../../src
SOURCES += main.cpp
