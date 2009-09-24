KDCHART_PATH = ../../..

TEMPLATE = app
TARGET =
DEPENDPATH += . ../../../include
INCLUDEPATH += . ../../../include

LIBS += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB

HEADERS += mainwindow.h \
           entrydialog.h \
           entrydelegate.h

SOURCES += main.cpp \
           mainwindow.cpp \
           entrydialog.cpp \
           entrydelegate.cpp

FORMS += mainwindow.ui \
         entrydialog.ui
