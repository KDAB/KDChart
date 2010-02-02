include( ../kdchartserializer.pri )

TEMPLATE = lib
TARGET = kdchartserializer
DESTDIR	= ../../lib

# Use the filename "kdchartserializerd.dll" (or "kdchartserializerd.lib") on Windows
# to avoid name clashes between debug/non-debug versions of the
# KD Chart library:
TARGET = kdchartserializer
CONFIG(debug, debug|release) {
    !unix: TARGET = kdchartserializerd
}

QT += xml

# use qmake CONFIG+=dll to compile
# the kdchart serializer library as a DLL
dll {
	DEFINES += KDCHARTSERIALIZER_DLL
}
DLLDESTDIR = ../../bin

DEFINES += KDCHART_BUILD_KDCHARTSERIALIZER_LIB

KDCHART_PATH = ../..
INCLUDEPATH+= ../include \
            $$KDCHART_PATH/include \
            $$KDCHART_PATH/src \
            .
DEPENDPATH += ../include \
            $$KDCHART_PATH/include \
            .
LIBS        += -L$$KDCHART_PATH/lib -l$$KDCHARTLIB


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
        KDChartTextAreaSerializer.cpp \
        KDChartLegendsSerializer.cpp \
        KDXMLTools.cpp \
        KDChartDiagramSerializerFactory.cpp \
        KDChartLegendSerializerFactory.cpp \
        KDChartTextAreaSerializerFactory.cpp \
        KDChartAxisSerializerFactory.cpp \
        KDChartCoordPlaneSerializerFactory.cpp \
        KDChartAbstractSerializerFactory.cpp \

HEADERS= \
        kdchartserializer_export.h \
        KDChartSerializer.h \
        KDChartSerializer_p.h \
        KDChartSerializeCollector.h \
        KDChartSerializeCollector_p.h \
        KDChartIdMapper.h \
        KDChartIdMapper_p.h \
        KDChartAttributesSerializer.h \
        KDChartAttributesSerializer_p.h \
        KDChartAttributesModelSerializer.h \
        KDChartAttributesModelSerializer_p.h \
        KDChartAbstractAreaBaseSerializer.h \
        KDChartAbstractAreaBaseSerializer_p.h \
        KDChartCoordPlanesSerializer.h \
        KDChartCoordPlanesSerializer_p.h \
        KDChartDiagramsSerializer.h \
        KDChartDiagramsSerializer_p.h \
        KDChartAxesSerializer.h \
        KDChartAxesSerializer_p.h \
        KDChartTextAreaSerializer.h \
        KDChartTextAreaSerializer_p.h \
        KDChartLegendsSerializer.h \
        KDChartLegendsSerializer_p.h \
        KDXMLTools.h \
        KDChartDiagramSerializerFactory.h \
        KDChartDiagramSerializerFactory_p.h \
        KDChartLegendSerializerFactory.h \
        KDChartLegendSerializerFactory_p.h \
        KDChartTextAreaSerializerFactory.h \
        KDChartTextAreaSerializerFactory_p.h \
        KDChartAxisSerializerFactory.h \
        KDChartAxisSerializerFactory_p.h \
        KDChartCoordPlaneSerializerFactory.h \
        KDChartCoordPlaneSerializerFactory_p.h \
        KDChartAbstractSerializerFactory.h \
        KDChartSerializerRegistrator.h \

INSTALLHEADERS = \
    KDChartAbstractAreaBaseSerializer \
    KDChartAttributesModelSerializer \
    KDChartCoordPlanesSerializer \
    KDChartLegendsSerializer \
    KDChartSerializerRegistrator \
    KDChartAbstractSerializer \
    KDChartAttributesSerializer \
    KDChartDiagramsSerializer \
    KDChartSerializeCollector \
    KDChartTextAreaSerializer \
    KDChartAbstractSerializerFactory \
    KDChartAxesSerializer \
    KDChartIdMapper \
    KDChartSerializer \
    KDXMLTools

# installation targets:
serializer_headers_include.files = $$INSTALLHEADERS
serializer_headers_include.path = $$INSTALL_PREFIX/include
serializer_headers_src.files = $$HEADERS
serializer_headers_src.path = $$INSTALL_PREFIX/src
INSTALLS += serializer_headers_include serializer_headers_src
