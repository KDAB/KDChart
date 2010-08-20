KDCHART_PATH = ../../..

TARGET = TestUnittests

include( $$KDCHART_PATH/examples/examples.pri )

DEPENDPATH += . ../../../include
INCLUDEPATH += . ../../../include
CONFIG += console
QT += svg
macx:CONFIG -= app_bundle

# Input
SOURCES += main.cpp
