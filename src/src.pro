include( ../variables.pri )

DEFINES += KDCHART_BUILD_KDCHART_LIB

win32:!static:DEFINES += KDCHART_MAKEDLL

TEMPLATE = lib
TARGET = kdchart
DESTDIR	= ../lib
win32:DLLDESTDIR = ../bin
DEFINES += QT_NO_CAST_TO_ASCII

*g++*:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

SOURCES = \

FORMS += KDChartDatasetSelector.ui

HEADERS = \

qsa {
    message(compiling QSA support into KD Chart)
    SOURCES += KDChartObjectFactory.cpp \
               KDChartWrapperFactory.cpp \
               wrappers/KDChartAxisParamsWrapper.cpp \
               wrappers/KDChartParamsWrapper.cpp \
               wrappers/KDChartCustomBoxWrapper.cpp

    HEADERS += KDChartObjectFactory.h \
               KDChartWrapperFactory.h \
               wrappers/KDChartAxisParamsWrapper.h \
               wrappers/KDChartParamsWrapper.h \
               wrappers/KDChartCustomBoxWrapper.h \
               factories/QtFactory.h \
               factories/QFontFactory.h
}

DEPENDPATH = ../include $(QTDIR)/src/3rdparty/zlib .
INCLUDEPATH = ../include $(QTDIR)/src/3rdparty/zlib .

KDAB_EVAL {
  HEADERS += ../evaldialog/evaldialog.h
  SOURCES += ../evaldialog/evaldialog.cpp
  DEFINES += KDAB_EVAL
}
