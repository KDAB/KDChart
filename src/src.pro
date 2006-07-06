# Subdir relative project main directory: ./src
# Target is a library:  kdchart



# Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
# to avoid name clashes between debug/non-debug versions of the
# KD Chart library:
TARGET = kdchart
CONFIG(debug, debug|release) {
    !unix: TARGET = "kdchartd"
}



FORMS += KDChartDatasetSelector.ui 
HEADERS += KDChartGlobal.h \
           CartesianCoordinateTransformation.h \
           KDChartAbstractCartesianDiagram.h \
           KDChartAbstractCartesianDiagram_p.h \
           KDChartAbstractCoordinatePlane.h \
           KDChartAbstractCoordinatePlane_p.h \
           KDChartAbstractDiagram.h \
           KDChartAbstractDiagram_p.h \
           KDChartAbstractPieDiagram.h \
           KDChartAbstractPieDiagram_p.h \
           KDChartAbstractPolarDiagram.h \
           KDChartAbstractPolarDiagram_p.h \
           KDChartAbstractProxyModel.h \
           KDChartAbstractGrid.h \
           KDChartCartesianGrid.h \
           KDChartPolarGrid.h \
           KDChartAttributesModel.h \
           KDChartBackgroundAttributes.h \
           KDChartBarAttributes.h \
           KDChartBarDiagram.h \
           KDChartBarDiagram_p.h \
           KDChartCartesianAxis.h \
           KDChartCartesianAxis_p.h \
           KDChartCartesianCoordinatePlane.h \
           KDChartCartesianCoordinatePlane_p.h \
           KDChartChart.h \
           KDChartDatasetProxyModel.h \
           KDChartDatasetSelector.h \
           KDChartDataValueAttributes.h \
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
           KDChartPosition.h \
           KDChartRelativePosition.h \
           KDChartRingDiagram.h \
           KDChartRingDiagram_p.h \
           KDChartTextAttributes.h \
           KDTextDocument.h \
           KDChartAbstractThreeDAttributes.h \
           KDChartAbstractThreeDAttributes_p.h \
           KDChartThreeDBarAttributes.h \
           KDChartThreeDBarAttributes_p.h \
           KDChartThreeDLineAttributes.h \
           KDChartThreeDLineAttributes_p.h \
           KDChartThreeDPieAttributes.h \
           KDChartThreeDPieAttributes_p.h \
           KDChartWidget.h \
           KDChartWidget_p.h \
           KDChartAbstractAreaBase_p.h \
           KDChartAbstractAreaBase.h \
           KDChartAbstractArea.h \
           KDChartAbstractAreaWidget.h \
           KDChartAbstractAxis.h \
           KDChartChart_p.h 
SOURCES += KDChartAbstractCartesianDiagram.cpp \
           KDChartAbstractCoordinatePlane.cpp \
           KDChartAbstractDiagram.cpp \
           KDChartAbstractPieDiagram.cpp \
           KDChartAbstractPolarDiagram.cpp \
           KDChartAbstractProxyModel.cpp \
           KDChartAbstractGrid.cpp \
           KDChartCartesianGrid.cpp \
           KDChartPolarGrid.cpp \
           KDChartAttributesModel.cpp \
           KDChartBackgroundAttributes.cpp \
           KDChartBarAttributes.cpp \
           KDChartBarDiagram.cpp \
           KDChartCartesianAxis.cpp \
           KDChartCartesianCoordinatePlane.cpp \
           KDChartChart.cpp \
           KDChartDatasetProxyModel.cpp \
           KDChartDatasetSelector.cpp \
           KDChartDataValueAttributes.cpp \
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
           KDChartPosition.cpp \
           KDChartRelativePosition.cpp \
           KDChartRingDiagram.cpp \
           KDTextDocument.cpp \
           KDChartTextAttributes.cpp \
           KDChartAbstractThreeDAttributes.cpp \
           KDChartThreeDBarAttributes.cpp \
           KDChartThreeDLineAttributes.cpp \
           KDChartThreeDPieAttributes.cpp \
           KDChartWidget.cpp \
           KDChartAbstractAreaBase.cpp \
           KDChartAbstractArea.cpp \
           KDChartAbstractAreaWidget.cpp \
           KDChartAbstractAxis.cpp
include( ../variables.pri )
DEFINES += KDCHART_BUILD_KDCHART_LIB
TEMPLATE = lib

DESTDIR = ../lib
DEFINES += QT_NO_CAST_TO_ASCII
DEPENDPATH = ../include $(QTDIR)/src/3rdparty/zlib .
INCLUDEPATH = ../include $(QTDIR)/src/3rdparty/zlib .
linux-*{
  version_script{
    QMAKE_LFLAGS += -Wl,--version-script=libkdchart.map
    TARGETDEPS += libkdchart.map
  }
}
qsa{
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
KDAB_EVAL{
  HEADERS += ../evaldialog/evaldialog.h
  SOURCES += ../evaldialog/evaldialog.cpp
  DEFINES += KDAB_EVAL
}
win32{
  DLLDESTDIR = ../bin
  !static{
    DEFINES += KDCHART_MAKEDLL
  }
}
*g++*{
  QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
}

