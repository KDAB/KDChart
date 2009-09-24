TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src
INCLUDEPATH += . ../../include

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

LIBS+= -L../../lib -l$$KDGANTTLIB

QT+=xml

# Input
HEADERS += mainwindow.h projectmodel.h
SOURCES += main.cpp mainwindow.cpp projectmodel.cpp
