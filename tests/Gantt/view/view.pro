TARGET = TestView

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

DEPENDPATH += . $${TOP_SOURCE_DIR}/include $${TOP_SOURCE_DIR}/src $${TOP_SOURCE_DIR}/src/Gantt
INCLUDEPATH += . $${TOP_SOURCE_DIR}/include
QT += svg

contains(QT_VERSION, ^5\\.[0-9]\\..*):QT += printsupport

# Input
SOURCES += main.cpp
