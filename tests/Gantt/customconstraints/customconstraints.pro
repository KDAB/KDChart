TARGET = TestCustomConstraints

include( $${TOP_SOURCE_DIR}/examples/examples.pri )

DEPENDPATH += . $${TOP_SOURCE_DIR}/include $${TOP_SOURCE_DIR}/src $${TOP_SOURCE_DIR}/src/Gantt
INCLUDEPATH += . $${TOP_SOURCE_DIR}/include

LIBS+= -L$${TOP_BUILD_DIR}/lib -l$$KDCHARTLIB

greaterThan(QT_MAJOR_VERSION, 4):QT += printsupport

# Input
SOURCES += main.cpp
