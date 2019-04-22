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

greaterThan(QT_MAJOR_VERSION, 4):QT += printsupport

message( "Building ''$$TARGET'' using LIBS ''$$LIBS''" )
