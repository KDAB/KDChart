TARGET = TestGfxView

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

DEPENDPATH += . $${TOP_SOURCE_DIR}/include $${TOP_SOURCE_DIR}/src/Gantt
INCLUDEPATH += . $${TOP_SOURCE_DIR}/include
QT += svg

QT += printsupport

# Input
SOURCES += main.cpp
