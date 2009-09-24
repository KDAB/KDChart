KDGANTT_PATH = ../..

TEMPLATE = app
TARGET =
DEPENDPATH += . $$KDGANTT_PATH/include
INCLUDEPATH += . $$KDGANTT_PATH/include


KDGANTTLIB=kdgantt
KDGANTT_MAJOR_VERSION=2

CONFIG(debug,debug|release) {
        !unix {
            staticlib {
                KDGANTTLIB=kdganttd
            } else {
                KDGANTTLIB=kdganttd$$KDGANTT_MAJOR_VERSION
            }
        }
}


LIBS += -L$$KDGANTT_PATH/lib -l$$KDGANTTLIB

HEADERS += mainwindow.h \
           entrydialog.h \
           entrydelegate.h

SOURCES += main.cpp \
           mainwindow.cpp \
           entrydialog.cpp \
           entrydelegate.cpp

FORMS += mainwindow.ui \
         entrydialog.ui
