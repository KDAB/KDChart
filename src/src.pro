include( ../variables.pri )

DEFINES += KDCHART_BUILD_KDCHART_LIB

win32:!static:DEFINES += KDCHART_MAKEDLL

TEMPLATE = lib
TARGET = kdchart
DESTDIR	= ../lib
win32:DLLDESTDIR = ../bin
DEFINES += QT_NO_CAST_TO_ASCII

*g++*:QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

HEADERS = \
           KDChartGlobal.h \
           CartesianCoordinateTransformation.h \
           KDChartAbstractCartesianDiagram.h \
           KDChartAbstractCartesianDiagram_p.h \
           KDChartAbstractDiagram.h \
           KDChartAbstractDiagram_p.h \
           KDChartAbstractPieDiagram.h \
           KDChartAbstractPieDiagram_p.h \
           KDChartAbstractPolarDiagram.h \
           KDChartAbstractPolarDiagram_p.h \
           KDChartArea.h \
           KDChartArea_p.h \
           KDChartAttributesModel.h \
           KDChartAxis.h \
           KDChartAxis_p.h \
           KDChartBackgroundAttributes.h \
           KDChartBarAttributes.h \
           KDChartBarDiagram.h \
           KDChartBarDiagram_p.h \
           KDChartCartesianAxis.h \
           KDChartCartesianAxis_p.h \
           KDChartCartesianCoordinatePlane.h \
           KDChartChart.h \
           KDChartCoordinatePlane.h \
           KDChartDatasetProxyModel.h \
           KDChartDatasetSelector.h \
           KDChartDataValueAttributes.h \
           KDChartDefaultsModel.h \
           KDChartDiagramObserver.h \
           KDChartEnums.h \
           KDChartFrameAttributes.h \
           KDChartGridAttributes.h \
           KDChartHeaderFooter.h \
           KDChartHeaderFooter_p.h \
           KDChartLayoutItems.h \
           KDChartLegend.h \
           KDChartLegend_p.h \
           KDChartLineAttributes.h \
           KDChartLineDiagram.h \
           KDChartLineDiagram_p.h \
           KDChartMarkerAttributes.h \
           KDChartMeasure.h \
           KDChartPaintContext.h \
           KDChartPalette.h \
           KDChartPieDiagram.h \
           KDChartPieDiagram_p.h \
           KDChartPolarCoordinatePlane.h \
           KDChartPolarDiagram.h \
           KDChartPolarDiagram_p.h \
           KDChartRelativePosition.h \
           KDChartRingDiagram.h \
           KDChartRingDiagram_p.h \
           KDChartTextAttributes.h \
           KDTextDocument.h \
           KDChartThreeDAttributes.h \
           KDChartThreeDAttributes_p.h \
           KDChartThreeDBarAttributes.h \
           KDChartThreeDBarAttributes_p.h \
           KDChartThreeDLineAttributes.h \
           KDChartThreeDLineAttributes_p.h \
           KDChartWidget.h \
           KDChartWidget_p.h

SOURCES = \
           KDChartAbstractCartesianDiagram.cpp \
           KDChartAbstractDiagram.cpp \
           KDChartAbstractPieDiagram.cpp \
           KDChartAbstractPolarDiagram.cpp \
           KDChartArea.cpp \
           KDChartAttributesModel.cpp \
           KDChartAxis.cpp \
           KDChartBackgroundAttributes.cpp \
           KDChartBarAttributes.cpp \
           KDChartBarDiagram.cpp \
           KDChartCartesianAxis.cpp \
           KDChartCartesianCoordinatePlane.cpp \
           KDChartChart.cpp \
           KDChartCoordinatePlane.cpp \
           KDChartDatasetProxyModel.cpp \
           KDChartDatasetSelector.cpp \
           KDChartDataValueAttributes.cpp \
           KDChartDefaultsModel.cpp \
           KDChartDiagramObserver.cpp \
           KDChartFrameAttributes.cpp \
           KDChartGridAttributes.cpp \
           KDChartHeaderFooter.cpp \
           KDChartLayoutItems.cpp \
           KDChartLegend.cpp \
           KDChartLineAttributes.cpp \
           KDChartLineDiagram.cpp \
           KDChartMarkerAttributes.cpp \
           KDChartMeasure.cpp \ 
           KDChartPaintContext.cpp \
           KDChartPalette.cpp \
           KDChartPieDiagram.cpp \
           KDChartPolarCoordinatePlane.cpp \
           KDChartPolarDiagram.cpp \
           KDChartRelativePosition.cpp \
           KDChartRingDiagram.cpp \
           KDTextDocument.cpp \
           KDChartTextAttributes.cpp \
           KDChartThreeDAttributes.cpp \
           KDChartThreeDBarAttributes.cpp \
           KDChartThreeDLineAttributes.cpp \
           KDChartWidget.cpp

FORMS += KDChartDatasetSelector.ui

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
