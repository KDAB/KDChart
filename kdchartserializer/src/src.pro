include( ../kdchartserializer.pri )

TEMPLATE = lib
TARGET = kdchartserializer
DESTDIR	= ../../lib

QT += xml

# use qmake CONFIG+=dll to compile
# the kdchart serializer library as a DLL
dll {
	DEFINES += KDCHARTSERIALIZER_DLL
}
DLLDESTDIR = ../../bin


KDCHART_PATH = ../..
INCLUDEPATH+= ../include \
            $(QTDIR)/src/3rdparty/zlib \
            $$KDCHART_PATH/include \
            .
DEPENDPATH += ../include \
            $(QTDIR)/src/3rdparty/zlib \
            $$KDCHART_PATH/include \
            .
LIBS        += -L$$KDCHART_PATH/lib -lkdchart


SOURCES= \
        KDChartSerializer.cpp \
        KDChartAttributesSerializer.cpp \
        KDChartCoordPlanesSerializer.cpp \
        KDChartDiagramsSerializer.cpp \
        KDXMLTools.cpp

HEADERS= \
        KDChartSerializer.h \
        KDChartAttributesSerializer.h \
        KDChartCoordPlanesSerializer.h \
        KDChartDiagramsSerializer.h \
        KDXMLTools.cpp
