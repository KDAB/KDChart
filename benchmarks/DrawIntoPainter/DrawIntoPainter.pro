# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: .
# Target is an application:  

KDCHARTLIB = kdchart
# CONFIG+ =
TEMPLATE = app
INCLUDEPATH += ../../include ./ ../../examples/tools ../../examples/DrawIntoPainter
DEPENDPATH += . ../../src ../../lib ../../include ../../examples/tools ../../examples/DrawIntoPainter
test.target = test
test.commands = LD_LIBRARY_PATH=../../lib ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test
HEADERS += mainwindow.h framewidget.h DynamicTableModel.h benchmarkwindow.h \
testmainwindow.h
SOURCES += main.cpp framewidget.cpp mainwindow.cpp DynamicTableModel.cpp benchmarkwindow.cpp \
testmainwindow.cpp
LIBS += -L../../lib -l$$KDCHARTLIB -ltesttools
FORMS = benchmarkwindow.ui
RESOURCES = DrawIntoPainter.qrc
CONFIG(debug, debug|release){
    HEADERS +=
    SOURCES +=
    !unix{
        KDCHARTLIB = "kdchartd"
    }
}