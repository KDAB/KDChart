KDCHART_PATH = ../../..

TARGET = TestApiReview

include( $$KDCHART_PATH/examples/examples.pri )

DEPENDPATH += . ../../../include
INCLUDEPATH += . ../../../include
QT += svg

HEADERS += mainwindow.h \
           entrydialog.h \
           entrydelegate.h

SOURCES += main.cpp \
           mainwindow.cpp \
           entrydialog.cpp \
           entrydelegate.cpp

FORMS += mainwindow.ui \
         entrydialog.ui
