include( ../benchmarks.pri )

INCLUDEPATH += ./ ../../examples/DrawIntoPainter
DEPENDPATH += . ../../examples/DrawIntoPainter

HEADERS += mainwindow.h \
           framewidget.h \
           DynamicTableModel.h \
           benchmarkwindow.h \
           testmainwindow.h
SOURCES += main.cpp \
           framewidget.cpp \
           mainwindow.cpp \
           DynamicTableModel.cpp \
           benchmarkwindow.cpp \
           testmainwindow.cpp
FORMS = benchmarkwindow.ui
RESOURCES = DrawIntoPainter.qrc
