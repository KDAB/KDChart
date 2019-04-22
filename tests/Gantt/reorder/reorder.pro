TARGET = TestReorder

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

DEPENDPATH += . $${TOP_SOURCE_DIR}/include $${TOP_SOURCE_DIR}/src/Gantt
INCLUDEPATH += . $${TOP_SOURCE_DIR}/include
QT += svg

# Input
SOURCES += main.cpp
