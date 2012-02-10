# Subdir relative project main directory: ./src

# Target is a library:  kdchart
TEMPLATE = lib
# Use the filename "kdchartd.dll" (or "kdchartd.lib") on Windows
# to avoid name clashes between debug/non-debug versions of the
# KD Chart library:
TARGET = kdchart
CONFIG(debug, debug|release) {
    !unix: TARGET = kdchartd
}

include( ../variables.pri )

DEFINES += KDCHART_BUILD_KDCHART_LIB

RESOURCES += KDChart/KDAB_kdchart_LeveyJennings_resources.qrc

QT += svg

FORMS += KDChart/KDChartDatasetSelector.ui

INSTALLHEADERS_INCLUDE = \
    KDChart/KDChartAbstractAxis \
    KDChart/KDChartAbstractCartesianDiagram \
    KDChart/KDChartAbstractCoordinatePlane \
    KDChart/KDChartAbstractDiagram \
    KDChart/KDChartAbstractPieDiagram \
    KDChart/KDChartAbstractPolarDiagram \
    KDChart/KDChartAbstractProxyModel \
    KDChart/KDChartAbstractTernaryDiagram \
    KDChart/KDChartAbstractThreeDAttributes \
    KDChart/KDChartAttributesModel \
    KDChart/KDChartBackgroundAttributes \
    KDChart/KDChartBarAttributes \
    KDChart/KDChartStockBarAttributes \
    KDChart/KDChartStockDiagram \
    KDChart/KDChartBarDiagram \
    KDChart/KDChartCartesianAxis \
    KDChart/KDChartCartesianCoordinatePlane \
    KDChart/KDChartChart \
    KDChart/KDChartDatasetProxyModel \
    KDChart/KDChartDatasetSelector \
    KDChart/KDChartDataValueAttributes \
    KDChart/KDChartDiagramObserver \
    KDChart/KDChartEnums \
    KDChart/KDChartFrameAttributes \
    KDChart/KDChartGlobal \
    KDChart/KDChartGridAttributes \
    KDChart/KDChartHeaderFooter \
    KDChart/KDChartLegend \
    KDChart/KDChartLeveyJenningsAxis \
    KDChart/KDChartLeveyJenningsCoordinatePlane \
    KDChart/KDChartLeveyJenningsDiagram \
    KDChart/KDChartLeveyJenningsGrid \
    KDChart/KDChartLeveyJenningsGridAttributes \
    KDChart/KDChartLineAttributes \
    KDChart/KDChartLineDiagram \
    KDChart/KDChartMarkerAttributes \
    KDChart/KDChartMeasure \
    KDChart/KDChartPaintContext \
    KDChart/KDChartPalette \
    KDChart/KDChartPieAttributes \
    KDChart/KDChartPieDiagram \
    KDChart/KDChartPlotter \
    KDChart/KDChartPolarCoordinatePlane \
    KDChart/KDChartPolarDiagram \
    KDChart/KDChartRadarCoordinatePlane \
    KDChart/KDChartRadarDiagram \
    KDChart/KDChartPosition \
    KDChart/KDChartRelativePosition \
    KDChart/KDChartRingDiagram \
    KDChart/KDChartTernaryAxis \
    KDChart/KDChartTernaryCoordinatePlane \
    KDChart/KDChartTernaryLineDiagram \
    KDChart/KDChartTernaryPointDiagram \
    KDChart/KDChartTextAttributes \
    KDChart/KDChartThreeDBarAttributes \
    KDChart/KDChartThreeDLineAttributes \
    KDChart/KDChartThreeDPieAttributes \
    KDChart/KDChartValueTrackerAttributes \
    KDChart/KDChartWidget \
    KDChart/KDChartZoomParameters \
    KDChart/KDTextDocument \
    KDChart/KDGanttAbstractGrid \
    KDChart/KDGanttAbstractRowController \
    KDGanttConstraint \
    KDGanttConstraintGraphicsItem \
    KDGanttConstraintModel \
    KDGanttDateTimeGrid \
    KDGanttForwardingProxyModel \
    KDGanttGlobal \
    KDGanttGraphicsItem \
    KDGanttGraphicsView \
    KDGanttGraphicsScene \
    KDGanttItemDelegate \
    KDGanttLegend \
    KDGanttListViewRowController \
    KDGanttStyleOptionGanttItem \
    KDGanttSummaryHandlingProxyModel \
    KDGanttTreeViewRowController \
    KDGanttView \
    KDGanttProxyModel \

INSTALLHEADERS_SRC = \
	   kdchart_export.h \
           KDChartGlobal.h \
           KDChartMeasure.h \
           CartesianCoordinateTransformation.h \
           KDChartAbstractCartesianDiagram.h \
           KDChartAbstractCoordinatePlane.h \
           KDChartCartesianCoordinatePlane.h \
           KDChartPolarCoordinatePlane.h \
           KDChartChart.h \
           KDChartWidget.h \
           KDChartAbstractDiagram.h \
           KDChartAbstractPieDiagram.h \
           KDChartAbstractPolarDiagram.h \
           KDChartAbstractAreaBase.h \
           KDChartAbstractArea.h \
           KDChartTextArea.h \
           KDChartAbstractAreaWidget.h \
           KDChartAbstractAxis.h \
           KDChartAbstractProxyModel.h \
           KDChartAbstractGrid.h \
           KDChartCartesianGrid.h \
           KDChartPolarGrid.h \
           KDChartAttributesModel.h \
           KDChartBackgroundAttributes.h \
           KDChartBarAttributes.h \
           KDChartStockBarAttributes.h \
           KDChartStockDiagram.h \
           KDChartBarDiagram.h \
           KDChartCartesianAxis.h \
           KDChartDatasetProxyModel.h \
           KDChartDatasetSelector.h \
           KDChartDataValueAttributes.h \
           KDChartDiagramObserver.h \
           KDChartEnums.h \
           KDChartFrameAttributes.h \
           KDChartGridAttributes.h \
           KDChartRulerAttributes.h \
           KDChartHeaderFooter.h \
           KDChartLayoutItems.h \
           KDChartLegend.h \
           KDChartLineAttributes.h \
           KDChartLineDiagram.h \
           KDChartCartesianDiagramDataCompressor.h \
           KDChartMarkerAttributes.h \
           KDChartPaintContext.h \
           KDChartPalette.h \
           KDChartPieDiagram.h \
           KDChartPolarDiagram.h \
           KDChartRadarDiagram.h \
           KDChartPosition.h \
           KDChartRelativePosition.h \
           KDChartRingDiagram.h \
           KDChartTextAttributes.h \
           KDTextDocument.h \
           KDChartPieAttributes.h \
           KDChartAbstractThreeDAttributes.h \
           KDChartThreeDBarAttributes.h \
           KDChartThreeDLineAttributes.h \
           KDChartThreeDPieAttributes.h \
           PrerenderedElements/KDChartTextLabelCache.h \
# unused so far: KDChartSignalCompressor.h \
           Ternary/TernaryPoint.h \
           Ternary/TernaryConstants.h \
           Ternary/KDChartTernaryGrid.h \
           Ternary/KDChartTernaryCoordinatePlane.h \
           Ternary/KDChartTernaryAxis.h \
           Ternary/KDChartAbstractTernaryDiagram.h \
           Ternary/KDChartTernaryPointDiagram.h \
           Ternary/KDChartTernaryLineDiagram.h \
           KDChartValueTrackerAttributes.h \
           KDChartPlotter.h \
           KDChartPrintingParameters.h \
           LeveyJennings/KDChartLeveyJenningsCoordinatePlane.h \
           LeveyJennings/KDChartLeveyJenningsDiagram.h \
           LeveyJennings/KDChartLeveyJenningsGrid.h \
           LeveyJennings/KDChartLeveyJenningsGridAttributes.h \
           LeveyJennings/KDChartLeveyJenningsAxis.h \

INSTALLHEADERS_GANTT_SRC = \
           KDGantt/kdgantt_export.h \
           KDGantt/kdganttglobal.h \
           KDGantt/kdganttabstractrowcontroller.h \
           KDGantt/kdgantttreeviewrowcontroller.h \
           KDGantt/kdganttlistviewrowcontroller.h \
           KDGantt/kdganttview.h \
           KDGantt/kdganttstyleoptionganttitem.h \
           KDGantt/kdganttgraphicsview.h \
           KDGantt/kdganttgraphicsscene.h \
           KDGantt/kdganttgraphicsitem.h \
           KDGantt/kdganttconstraint.h \
           KDGantt/kdganttconstraintproxy.h \
           KDGantt/kdganttconstraintgraphicsitem.h \
           KDGantt/kdganttitemdelegate.h \
           KDGantt/kdganttforwardingproxymodel.h \
           KDGantt/kdganttsummaryhandlingproxymodel.h \
           KDGantt/kdganttproxymodel.h \
           KDGantt/kdganttconstraintmodel.h \
           KDGantt/kdganttabstractgrid.h \
           KDGantt/kdganttdatetimegrid.h \
           KDGantt/kdganttlegend.h \

# installation targets:
headers_include.files = $$INSTALLHEADERS_INCLUDE
headers_include.path = $$INSTALL_PREFIX/include
headers_src.files = $$INSTALLHEADERS_SRC
headers_src.path = $$INSTALL_PREFIX/src
headers_gantt_src.files = $$INSTALLHEADERS_GANTT_SRC
headers_gantt_src.path = $$INSTALL_PREFIX/src/Gantt
#INSTALLS += headers_include headers_src headers_gantt_src

# install target to install the src code for license holders:
lib.files = $${DESTDIR}
lib.path = $$INSTALL_PREFIX/
INSTALLS += lib



HEADERS += KDChart/KDChartGlobal.h \
           KDChart/KDChartMeasure.h \
           KDChart/CartesianCoordinateTransformation.h \
           KDChart/KDChartAbstractCartesianDiagram.h \
           KDChart/KDChartAbstractCartesianDiagram_p.h \
           KDChart/KDChartAbstractCoordinatePlane.h \
           KDChart/KDChartAbstractCoordinatePlane_p.h \
           KDChart/KDChartCartesianCoordinatePlane.h \
           KDChart/KDChartCartesianCoordinatePlane_p.h \
           KDChart/KDChartPolarCoordinatePlane.h \
           KDChart/KDChartPolarCoordinatePlane_p.h \
           KDChart/KDChartRadarCoordinatePlane.h \
           KDChart/KDChartRadarCoordinatePlane_p.h \
           KDChart/KDChartChart.h \
           KDChart/KDChartChart_p.h \
           KDChart/KDChartWidget.h \
           KDChart/KDChartWidget_p.h \
           KDChart/KDChartAbstractDiagram.h \
           KDChart/KDChartAbstractDiagram_p.h \
           KDChart/KDChartAbstractPieDiagram.h \
           KDChart/KDChartAbstractPieDiagram_p.h \
           KDChart/KDChartAbstractPolarDiagram.h \
           KDChart/KDChartAbstractPolarDiagram_p.h \
           KDChart/KDChartAbstractAreaBase.h \
           KDChart/KDChartAbstractAreaBase_p.h \
           KDChart/KDChartAbstractArea.h \
           KDChart/KDChartAbstractArea_p.h \
           KDChart/KDChartTextArea.h \
           KDChart/KDChartTextArea_p.h \
           KDChart/KDChartAbstractAreaWidget.h \
           KDChart/KDChartAbstractAreaWidget_p.h \
           KDChart/KDChartAbstractAxis.h \
           KDChart/KDChartAbstractProxyModel.h \
           KDChart/KDChartAbstractGrid.h \
           KDChart/KDChartCartesianGrid.h \
           KDChart/KDChartPolarGrid.h \
           KDChart/KDChartRadarGrid.h \
           KDChart/KDChartAttributesModel.h \
           KDChart/KDChartBackgroundAttributes.h \
           KDChart/KDChartBarAttributes.h \
           KDChart/KDChartStockBarAttributes.h \
           KDChart/KDChartStockDiagram.h \
           KDChart/KDChartStockDiagram_p.h \
           KDChart/KDChartBarDiagram.h \
           KDChart/KDChartBarDiagram_p.h \
           KDChart/KDChartCartesianAxis.h \
           KDChart/KDChartCartesianAxis_p.h \
           KDChart/KDChartDatasetProxyModel.h \
           KDChart/KDChartDatasetSelector.h \
           KDChart/KDChartDataValueAttributes.h \
           KDChart/KDChartDiagramObserver.h \
           KDChart/KDChartEnums.h \
           KDChart/KDChartFrameAttributes.h \
           KDChart/KDChartGridAttributes.h \
           KDChart/KDChartRulerAttributes.h \
           KDChart/KDChartHeaderFooter.h \
           KDChart/KDChartHeaderFooter_p.h \
           KDChart/KDChartLayoutItems.h \
           KDChart/KDChartLegend.h \
           KDChart/KDChartLegend_p.h \
           KDChart/KDChartLineAttributes.h \
           KDChart/KDChartLineDiagram.h \
           KDChart/KDChartLineDiagram_p.h \
           KDChart/KDChartCartesianDiagramDataCompressor_p.h \
           KDChart/KDChartNormalBarDiagram_p.h \
           KDChart/KDChartNormalLyingBarDiagram_p.h \
           KDChart/KDChartNormalLineDiagram_p.h \
           KDChart/KDChartStackedBarDiagram_p.h \
           KDChart/KDChartStackedLyingBarDiagram_p.h \
           KDChart/KDChartStackedLineDiagram_p.h \
           KDChart/KDChartPercentBarDiagram_p.h \
           KDChart/KDChartPercentLyingBarDiagram_p.h \
           KDChart/KDChartPercentLineDiagram_p.h \
           KDChart/KDChartMarkerAttributes.h \
           KDChart/KDChartPaintContext.h \
           KDChart/KDChartPalette.h \
           KDChart/KDChartPieDiagram.h \
           KDChart/KDChartPieDiagram_p.h \
           KDChart/KDChartPolarDiagram.h \
           KDChart/KDChartPolarDiagram_p.h \
           KDChart/KDChartRadarDiagram.h \
           KDChart/KDChartRadarDiagram_p.h \
           KDChart/KDChartPosition.h \
           KDChart/KDChartRelativePosition.h \
           KDChart/KDChartRingDiagram.h \
           KDChart/KDChartRingDiagram_p.h \
           KDChart/KDChartTextAttributes.h \
           KDChart/KDTextDocument.h \
           KDChart/KDChartPieAttributes.h \
           KDChart/KDChartPieAttributes_p.h \
           KDChart/KDChartAbstractThreeDAttributes.h \
           KDChart/KDChartAbstractThreeDAttributes_p.h \
           KDChart/KDChartThreeDBarAttributes.h \
           KDChart/KDChartThreeDBarAttributes_p.h \
           KDChart/KDChartThreeDLineAttributes.h \
           KDChart/KDChartThreeDLineAttributes_p.h \
           KDChart/KDChartThreeDPieAttributes.h \
           KDChart/KDChartThreeDPieAttributes_p.h \ 
           KDChart/KDChartTextLabelCache.h \
           # unused so far: KDChartSignalCompressor.h \
           KDChart/TernaryPoint.h \
           KDChart/TernaryConstants.h \
           KDChart/KDChartTernaryGrid.h \
           KDChart/KDChartTernaryCoordinatePlane.h \
           KDChart/KDChartTernaryCoordinatePlane_p.h \
           KDChart/KDChartTernaryAxis.h \
           KDChart/KDChartAbstractTernaryDiagram.h \
           KDChart/KDChartAbstractTernaryDiagram_p.h \
           KDChart/KDChartTernaryPointDiagram.h \
           KDChart/KDChartTernaryLineDiagram.h \
           KDChart/ChartGraphicsItem.h \
           KDChart/ReverseMapper.h \
           KDChart/KDChartValueTrackerAttributes.h \
           KDChart/KDChartPlotter.h \
           KDChart/KDChartPlotter_p.h \
           KDChart/KDChartNormalPlotter_p.h \
           KDChart/KDChartPercentPlotter_p.h \
           KDChart/KDChartPrintingParameters.h \
           KDChart/KDChartModelDataCache_p.h \
           KDChart/KDChartPlotterDiagramCompressor.h \
           KDChart/KDChartPlotterDiagramCompressor_p.h \
           KDChart/KDChartLeveyJenningsCoordinatePlane.h \
           KDChart/KDChartLeveyJenningsCoordinatePlane_p.h \
           KDChart/KDChartLeveyJenningsDiagram.h \
           KDChart/KDChartLeveyJenningsDiagram_p.h \
           KDChart/KDChartLeveyJenningsGrid.h \
           KDChart/KDChartLeveyJenningsGridAttributes.h \
           KDChart/KDChartLeveyJenningsAxis.h \
           KDChart/KDChartLeveyJenningsAxis_p.h \
           KDGantt/kdganttglobal.h \
           KDGantt/kdganttview.h \
           KDGantt/kdganttstyleoptionganttitem.h \
           KDGantt/kdganttgraphicsview.h \
           KDGantt/kdganttabstractrowcontroller.h \
           KDGantt/kdgantttreeviewrowcontroller.h \
           KDGantt/kdganttlistviewrowcontroller.h \
           KDGantt/kdganttgraphicsscene.h \
           KDGantt/kdganttgraphicsitem.h \
           KDGantt/kdganttconstraint.h \
           KDGantt/kdganttconstraintproxy.h \
           KDGantt/kdganttconstraintgraphicsitem.h \
           KDGantt/kdganttitemdelegate.h \
           KDGantt/kdganttforwardingproxymodel.h \
           KDGantt/kdganttsummaryhandlingproxymodel.h \
           KDGantt/kdganttproxymodel.h \
           KDGantt/kdganttconstraintmodel.h \
           KDGantt/kdganttabstractgrid.h \
           KDGantt/kdganttdatetimegrid.h \
           KDGantt/kdganttlegend.h \
           KDGantt/kdgantttreeviewrowcontroller_p.h \
           KDGantt/kdganttlistviewrowcontroller_p.h \
           KDGantt/kdganttview_p.h \
           KDGantt/kdganttgraphicsview_p.h \
           KDGantt/kdganttgraphicsscene_p.h \
           KDGantt/kdganttconstraint_p.h \
           KDGantt/kdganttitemdelegate_p.h \
           KDGantt/kdganttsummaryhandlingproxymodel_p.h \
           KDGantt/kdganttproxymodel_p.h \
           KDGantt/kdganttconstraintmodel_p.h \
           KDGantt/kdganttabstractgrid_p.h \
           KDGantt/kdganttdatetimegrid_p.h \
           KDGantt/kdganttlegend_p.h \
           KDGantt/unittest/test.h \
           KDGantt/unittest/testregistry.h \
           KDGantt/unittest/libutil.h

SOURCES += \
           KDChart/KDChartMeasure.cpp \
           KDChart/KDChartAbstractCartesianDiagram.cpp \
           KDChart/KDChartAbstractCoordinatePlane.cpp \
           KDChart/KDChartCartesianCoordinatePlane.cpp \
           KDChart/KDChartPolarCoordinatePlane.cpp \
           KDChart/KDChartRadarCoordinatePlane.cpp \
           KDChart/KDChartChart.cpp \
           KDChart/KDChartWidget.cpp \
           KDChart/KDChartAbstractDiagram.cpp \
           KDChart/KDChartAbstractDiagram_p.cpp \
           KDChart/KDChartAbstractPieDiagram.cpp \
           KDChart/KDChartAbstractPolarDiagram.cpp \
           KDChart/KDChartAbstractAreaBase.cpp \
           KDChart/KDChartAbstractArea.cpp \
           KDChart/KDChartTextArea.cpp \
           KDChart/KDChartAbstractAreaWidget.cpp \
           KDChart/KDChartAbstractAxis.cpp \
           KDChart/KDChartAbstractProxyModel.cpp \
           KDChart/KDChartAbstractGrid.cpp \
           KDChart/KDChartCartesianGrid.cpp \
           KDChart/KDChartPolarGrid.cpp \
           KDChart/KDChartRadarGrid.cpp \
           KDChart/KDChartAttributesModel.cpp \
           KDChart/KDChartBackgroundAttributes.cpp \
           KDChart/KDChartBarAttributes.cpp \
           KDChart/KDChartStockBarAttributes.cpp \
           KDChart/KDChartStockDiagram.cpp \
           KDChart/KDChartStockDiagram_p.cpp \
           KDChart/KDChartBarDiagram.cpp \
           KDChart/KDChartBarDiagram_p.cpp \
           KDChart/KDChartCartesianAxis.cpp \
           KDChart/KDChartDatasetProxyModel.cpp \
           KDChart/KDChartDatasetSelector.cpp \
           KDChart/KDChartDataValueAttributes.cpp \
           KDChart/KDChartDiagramObserver.cpp \
           KDChart/KDChartFrameAttributes.cpp \
           KDChart/KDChartGridAttributes.cpp \
           KDChart/KDChartRulerAttributes.cpp \
           KDChart/KDChartHeaderFooter.cpp \
           KDChart/KDChartLayoutItems.cpp \
           KDChart/KDChartLegend.cpp \
           KDChart/KDChartLineAttributes.cpp \
           KDChart/KDChartLineDiagram.cpp \
           KDChart/KDChartLineDiagram_p.cpp \
           KDChart/KDChartCartesianDiagramDataCompressor_p.cpp \
           KDChart/KDChartNormalBarDiagram_p.cpp \
           KDChart/KDChartNormalLyingBarDiagram_p.cpp \
           KDChart/KDChartNormalLineDiagram_p.cpp \
           KDChart/KDChartStackedBarDiagram_p.cpp \
           KDChart/KDChartStackedLyingBarDiagram_p.cpp \
           KDChart/KDChartStackedLineDiagram_p.cpp \
           KDChart/KDChartPercentBarDiagram_p.cpp \
           KDChart/KDChartPercentLyingBarDiagram_p.cpp \
           KDChart/KDChartPercentLineDiagram_p.cpp \
           KDChart/KDChartMarkerAttributes.cpp \
           KDChart/KDChartPaintContext.cpp \
           KDChart/KDChartPalette.cpp \
           KDChart/KDChartPieDiagram.cpp \
           KDChart/KDChartPolarDiagram.cpp \
           KDChart/KDChartRadarDiagram.cpp \
           KDChart/KDChartPosition.cpp \
           KDChart/KDChartRelativePosition.cpp \
           KDChart/KDChartRingDiagram.cpp \
           KDChart/KDTextDocument.cpp \
           KDChart/KDChartTextAttributes.cpp \
           KDChart/KDChartPieAttributes.cpp \
           KDChart/KDChartAbstractThreeDAttributes.cpp \
           KDChart/KDChartThreeDBarAttributes.cpp \
           KDChart/KDChartThreeDLineAttributes.cpp \
           KDChart/KDChartThreeDPieAttributes.cpp \
           KDChart/KDChartTextLabelCache.cpp \
           # unused so far: KDChart/KDChartSignalCompressor.cpp \
           KDChart/KDChartTernaryAxis.cpp \
           KDChart/KDChartTernaryGrid.cpp \
           KDChart/TernaryPoint.cpp \
           KDChart/TernaryConstants.cpp \
           KDChart/KDChartTernaryCoordinatePlane.cpp \
           KDChart/KDChartAbstractTernaryDiagram.cpp \
           KDChart/KDChartTernaryPointDiagram.cpp \
           KDChart/KDChartTernaryLineDiagram.cpp \
           KDChart/ChartGraphicsItem.cpp \
           KDChart/ReverseMapper.cpp \
           KDChart/KDChartValueTrackerAttributes.cpp \
           KDChart/KDChartPlotter.cpp \
           KDChart/KDChartPlotter_p.cpp \
           KDChart/KDChartNormalPlotter_p.cpp \
           KDChart/KDChartPercentPlotter_p.cpp \
           KDChart/KDChartPrintingParameters.cpp \
           KDChart/KDChartModelDataCache_p.cpp \
           KDChart/KDChartPlotterDiagramCompressor.cpp \
           KDChart/KDChartLeveyJenningsCoordinatePlane.cpp \
           KDChart/KDChartLeveyJenningsDiagram.cpp \
           KDChart/KDChartLeveyJenningsDiagram_p.cpp \
           KDChart/KDChartLeveyJenningsGrid.cpp \
           KDChart/KDChartLeveyJenningsGridAttributes.cpp \
           KDChart/KDChartLeveyJenningsAxis.cpp \
           KDGantt/kdganttglobal.cpp \
           KDGantt/kdganttview.cpp \
           KDGantt/kdganttstyleoptionganttitem.cpp \
           KDGantt/kdganttgraphicsview.cpp \
           KDGantt/kdganttabstractrowcontroller.cpp \
           KDGantt/kdgantttreeviewrowcontroller.cpp \
           KDGantt/kdganttlistviewrowcontroller.cpp \
           KDGantt/kdganttgraphicsscene.cpp \
           KDGantt/kdganttgraphicsitem.cpp \
           KDGantt/kdganttconstraint.cpp \
           KDGantt/kdganttconstraintproxy.cpp \
           KDGantt/kdganttconstraintgraphicsitem.cpp \
           KDGantt/kdganttitemdelegate.cpp \
           KDGantt/kdganttforwardingproxymodel.cpp \
           KDGantt/kdganttsummaryhandlingproxymodel.cpp \
           KDGantt/kdganttproxymodel.cpp \
           KDGantt/kdganttconstraintmodel.cpp \
           KDGantt/kdganttabstractgrid.cpp \
           KDGantt/kdganttdatetimegrid.cpp \
           KDGantt/kdganttlegend.cpp \
           KDGantt/unittest/test.cpp \
           KDGantt/unittest/testregistry.cpp \

CONFIG += warn-on

# We want users of kdchart to be able to use the lib without interference with Qt-specific keywords, e.g. "signals" that collides with Boost's Signals
DEFINES += QT_NO_KEYWORDS
DEFINES += emit=""

LIBFAKES_PATH = ../kdablibfakes

DEPENDPATH = ../include \
            $$LIBFAKES_PATH/include \
            .
INCLUDEPATH = $$LIBFAKES_PATH/include \
              KDGantt \
              KDChart \
              . \
              ../include

linux-*{
  version_script{
    QMAKE_LFLAGS += -Wl,--version-script=libkdchart.map
    TARGETDEPS += libkdchart.map
  }
}

solaris-*{
LIBS *= -lsunmath
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

#*g++*{
#  QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter
#}

