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
            $$KDCHART_PATH/src \
            .
DEPENDPATH += ../include \
            $(QTDIR)/src/3rdparty/zlib \
            $$KDCHART_PATH/include \
            .
LIBS        += -L$$KDCHART_PATH/lib -lkdchart


SOURCES= \
        KDChartSerializer.cpp \
        KDChartSerializeCollector.cpp \
        KDChartIdMapper.cpp \
        KDChartAttributesSerializer.cpp \
        KDChartAttributesModelSerializer.cpp \
        KDChartAbstractAreaBaseSerializer.cpp \
        KDChartCoordPlanesSerializer.cpp \
        KDChartDiagramsSerializer.cpp \
        KDChartAxesSerializer.cpp \
        KDXMLTools.cpp

HEADERS= \
        KDChartSerializer.h \
        KDChartSerializeCollector.h \
        KDChartIdMapper.h \
        KDChartAttributesSerializer.h \
        KDChartAttributesModelSerializer.h \
        KDChartAbstractAreaBaseSerializer.h \
        KDChartCoordPlanesSerializer.h \
        KDChartDiagramsSerializer.h \
        KDChartAxesSerializer.h \
        KDXMLTools.cpp
