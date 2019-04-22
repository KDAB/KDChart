TARGET = TestGfxView

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

DEPENDPATH += . $${TOP_SOURCE_DIR}/include $${TOP_SOURCE_DIR}/src/Gantt
INCLUDEPATH += . $${TOP_SOURCE_DIR}/include
QT += svg

greaterThan(QT_MAJOR_VERSION, 4):QT += printsupport

# Input
SOURCES += main.cpp
