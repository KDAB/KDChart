KDCHART_PATH = ../../..

TARGET = PlotterTimeline

include( $$KDCHART_PATH/examples/examples.pri )

HEADERS += timeaxis.h timechartmodel.h
SOURCES += main.cpp timeaxis.cpp timechartmodel.cpp
