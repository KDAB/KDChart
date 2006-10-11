include( ../variables.pri )

TEMPLATE = lib
TARGET = kdchart1support
DESTDIR	= ../../lib

QT += xml

# use qmake CONFIG+=dll to compile the kdchart compat library as a DLL
dll {
	DEFINES += KDCHART1_DLL
}
DLLDESTDIR = ../../bin


KDCHART_PATH = ../..
INCLUDEPATH+= ../include \
            $(QTDIR)/src/3rdparty/zlib \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools \
            .
DEPENDPATH += ../include \
            $(QTDIR)/src/3rdparty/zlib \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/examples/tools \
            .
LIBS        += -L$$KDCHART_PATH/lib -lkdchart


SOURCES= \
        KDChart1TableBase.cpp \
        KDChart1Enums.cpp \
        KDDrawText1.cpp \
        KDFrame1.cpp \
        KDFrame1ProfileSection.cpp \
        KDChart1CustomBox.cpp \
        KDChart1DataIntern.cpp \
        KDChart1Params.cpp \
        KDChart1Params_io.cpp \
        KDChart1Params_frame.cpp \
        KDChart1PropertySet.cpp \
        KDChart1AxisParams.cpp \
        KDChart1TextPiece.cpp \
        KDChart1VectorTable.cpp \
        KDChart1Widget.cpp

HEADERS= KDDrawText1.h \
        KDChart1Enums.h \
        ../include/KDChart1AxisParams.h \
        ../include/KDChart1CustomBox.h \
        ../include/KDChart1DataIntern.h \
        ../include/KDChart1DataRegion.h \
        ../include/KDChart1NotEnoughSpaceException.h \
        ../include/KDChart1Params.h \
        ../include/KDChart1PropertySet.h \
        ../include/KDChart1Table.h \
        ../include/KDChart1TextPiece.h \
        ../include/KDChart1UnknownTypeException.h \
        ../include/KDChart1Widget.h \
        ../include/KDFrame1.h \
        ../include/KDFrame1ProfileSection.h \
        ../include/KDChart1TableBase.h \
        ../include/KDChart1VectorTable.h \
        ../include/KDChart1Enums.h

KDAB_EVAL {
  HEADERS += ../evaldialog/evaldialog.h
  SOURCES += ../evaldialog/evaldialog.cpp
  DEFINES += KDAB_EVAL
}
