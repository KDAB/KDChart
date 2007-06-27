KDCHART_PATH = ../..

include( $$KDCHART_PATH/examples/examples.pri )

TEMPLATE = app

SOURCES += main.cpp \
           kdchartdemo.cpp \
           kdbasicsourcepanel.cpp \
           tablemodel.cpp \

HEADERS += kdchartdemo.h \
           kdbasicsourcepanel.h \
           tablemodel.h \

FORMS = kdchartdemo.ui \
        kdbasicsourcepanel.ui \

CONFIG += warn_on \
          thread \
          qt

QT += gui xml
