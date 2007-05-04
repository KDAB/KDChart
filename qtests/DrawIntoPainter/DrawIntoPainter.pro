CONFIG+=qtestlib
TEMPLATE = app
INCLUDEPATH +=  ../../examples/tools ../../include ./
DEPENDPATH += . ../../src ../../lib ../../examples/tools ../../include

test.target = test
test.commands = ./$(TARGET)
test.depends = $(TARGET)
QMAKE_EXTRA_TARGETS += test

HEADERS += mainwindow.h framewidget.h mainwindow.h
SOURCES += main.cpp framewidget.cpp mainwindow.cpp
LIBS += -L../../lib -l$$KDCHARTLIB -ltesttools
FORMS  = mainwindow.ui
RESOURCES = DrawIntoPainter.qrc