include(../test.pri)

TEMPLATE = app
TARGET =
DEPENDPATH += . ../../include
INCLUDEPATH += . ../../include

LIBS += -L../../lib -l$$KDGANTTLIB

HEADERS += mainwindow.h \
           entrydialog.h \
           entrydelegate.h

SOURCES += main.cpp \
           mainwindow.cpp \
           entrydialog.cpp \
           entrydelegate.cpp

FORMS += mainwindow.ui \
         entrydialog.ui
