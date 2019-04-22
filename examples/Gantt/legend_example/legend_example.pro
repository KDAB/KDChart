include( $${TOP_SOURCE_DIR}/examples/examples.pri )

HEADERS += mainwindow.h \
           entrydialog.h \
           entrydelegate.h

SOURCES += main.cpp \
           mainwindow.cpp \
           entrydialog.cpp \
           entrydelegate.cpp

FORMS += mainwindow.ui \
         entrydialog.ui

contains(QT_VERSION, ^5\\.[0-9]\\..*):QT += printsupport

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
