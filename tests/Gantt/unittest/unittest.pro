TARGET = TestUnittests

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

DEPENDPATH += . $${TOP_SOURCE_DIR}/include
INCLUDEPATH += . $${TOP_SOURCE_DIR}/include
CONFIG += console
QT += svg
macx:CONFIG -= app_bundle

# Input
SOURCES += main.cpp
