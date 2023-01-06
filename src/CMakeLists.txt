##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_definitions(-DQT_NO_KEYWORDS -DQT_NO_CAST_FROM_BYTEARRAY -Demit=)

set(FORMS KDChart/KDChartDatasetSelector.ui)
set(RESOURCES KDChart/KDAB_kdchart_LeveyJennings_resources.qrc)

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/KDChart)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/KDChart)
endif()

include(ECMGenerateHeaders)

# ecm_generate_headers is not able to generate tree-less header files
# which is required by examples and unit tests (KDChart/KDChartCartesianAxis include by example).
# This kind of headers usage are most likely needed because cmake install headers tree-less...
# So we need multiple calls so all files end up in KDChart output folder and make the super common header ourselves.
# The proper fix would be to add a tree-less options to ecm_generate_headers

ecm_generate_headers(
    kdchart_HEADERS
    ORIGINAL
    CAMELCASE
    HEADER_NAMES
    KDChartAbstractArea
    KDChartAbstractAreaBase
    KDChartAbstractAreaWidget
    KDChartAbstractAxis
    KDChartAbstractCoordinatePlane
    KDChartAbstractDiagram
    KDChartAbstractGrid
    KDChartAbstractProxyModel
    KDChartAbstractThreeDAttributes
    KDChartAttributesModel
    KDChartBackgroundAttributes
    KDChartChart
    KDChartDataValueAttributes
    KDChartDatasetProxyModel
    KDChartDatasetSelector
    KDChartDiagramObserver
    KDChartEnums
    KDChartFrameAttributes
    KDChartGlobal
    KDChartGridAttributes
    KDChartHeaderFooter
    KDChartLayoutItems
    KDChartLegend
    KDChartLineAttributes
    KDChartMarkerAttributes
    KDChartMeasure
    KDChartNullPaintDevice
    KDChartPaintContext
    KDChartPalette
    KDChartPosition
    KDChartPrintingParameters
    KDChartRelativePosition
    KDChartRulerAttributes
    KDChartTextArea
    KDChartTextAttributes
    KDChartTextLabelCache
    KDChartThreeDLineAttributes
    KDChartValueTrackerAttributes
    KDChartWidget
    KDChartZoomParameters
    KDTextDocument
    ReverseMapper
    ChartGraphicsItem
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/KDChart
    RELATIVE
    KDChart
)

ecm_generate_headers(
    kdchart_HEADERS
    ORIGINAL
    CAMELCASE
    HEADER_NAMES
    KDChartAbstractCartesianDiagram
    KDChartBarAttributes
    KDChartBarDiagram
    KDChartCartesianAxis
    KDChartCartesianCoordinatePlane
    KDChartCartesianGrid
    KDChartLeveyJenningsAxis
    KDChartLeveyJenningsCoordinatePlane
    KDChartLeveyJenningsDiagram
    KDChartLeveyJenningsGrid
    KDChartLeveyJenningsGridAttributes
    KDChartLineDiagram
    KDChartPlotter
    KDChartPlotterDiagramCompressor
    KDChartStockBarAttributes
    KDChartStockDiagram
    KDChartThreeDBarAttributes
    CartesianCoordinateTransformation
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/KDChart
    RELATIVE
    KDChart/Cartesian
)

ecm_generate_headers(
    kdchart_HEADERS
    ORIGINAL
    CAMELCASE
    HEADER_NAMES
    KDChartAbstractPieDiagram
    KDChartAbstractPolarDiagram
    KDChartPieAttributes
    KDChartPieDiagram
    KDChartPolarCoordinatePlane
    KDChartPolarDiagram
    KDChartPolarGrid
    KDChartRadarCoordinatePlane
    KDChartRadarDiagram
    KDChartRadarGrid
    KDChartRingDiagram
    KDChartThreeDPieAttributes
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/KDChart
    RELATIVE
    KDChart/Polar
)

ecm_generate_headers(
    kdchart_HEADERS
    ORIGINAL
    CAMELCASE
    HEADER_NAMES
    KDChartAbstractTernaryDiagram
    KDChartTernaryAxis
    KDChartTernaryCoordinatePlane
    KDChartTernaryGrid
    KDChartTernaryLineDiagram
    KDChartTernaryPointDiagram
    TernaryConstants
    TernaryPoint
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/KDChart
    RELATIVE
    KDChart/Ternary
)
# Combine required headers into 1 big convenience header
set(COMMON_HEADER ${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChart)
file(WRITE ${COMMON_HEADER} "// convenience header\n")
foreach(_header ${kdchart_HEADERS})
    get_filename_component(_base ${_header} NAME)
    file(APPEND ${COMMON_HEADER} "#include \"${_base}\"\n")
endforeach()
list(APPEND kdchart_HEADERS "${COMMON_HEADER}")
configure_file("${COMMON_HEADER}" "${COMMON_HEADER}.h" COPYONLY)
list(APPEND kdchart_HEADERS "${COMMON_HEADER}.h")

ecm_generate_headers(
    kdchart_HEADERS
    ORIGINAL
    LOWERCASE
    HEADER_NAMES
    KDGanttAbstractGrid
    KDGanttAbstractRowController
    KDGanttConstraint
    KDGanttConstraintGraphicsItem
    KDGanttConstraintModel
    KDGanttConstraintProxy
    KDGanttDateTimeGrid
    KDGanttForwardingProxyModel
    KDGanttGlobal
    KDGanttGraphicsItem
    KDGanttGraphicsScene
    KDGanttGraphicsView
    KDGanttItemDelegate
    KDGanttLegend
    KDGanttListViewRowController
    KDGanttProxyModel
    KDGanttStyleOptionGanttItem
    KDGanttSummaryHandlingProxyModel
    KDGanttTreeViewRowController
    KDGanttView
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/KDChart
    RELATIVE
    KDGantt
    COMMON_HEADER
    KDGantt
)
configure_file("${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDGantt" "${CMAKE_CURRENT_BINARY_DIR}/KDChart/kdgantt.h" COPYONLY)
list(APPEND kdchart_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/KDChart/kdgantt.h")

install(
    FILES ${kdchart_HEADERS}
          KDChart/Cartesian/CartesianCoordinateTransformation.h
          KDChart/Cartesian/KDChartAbstractCartesianDiagram.h
          KDChart/Cartesian/KDChartBarAttributes.h
          KDChart/Cartesian/KDChartBarDiagram.h
          KDChart/Cartesian/KDChartCartesianAxis.h
          KDChart/Cartesian/KDChartCartesianCoordinatePlane.h
          KDChart/Cartesian/KDChartCartesianGrid.h
          KDChart/Cartesian/KDChartLeveyJenningsAxis.h
          KDChart/Cartesian/KDChartLeveyJenningsCoordinatePlane.h
          KDChart/Cartesian/KDChartLeveyJenningsDiagram.h
          KDChart/Cartesian/KDChartLeveyJenningsGrid.h
          KDChart/Cartesian/KDChartLeveyJenningsGridAttributes.h
          KDChart/Cartesian/KDChartLineDiagram.h
          KDChart/Cartesian/KDChartPlotter.h
          KDChart/Cartesian/KDChartPlotterDiagramCompressor.h
          KDChart/Cartesian/KDChartStockBarAttributes.h
          KDChart/Cartesian/KDChartStockDiagram.h
          KDChart/Cartesian/KDChartThreeDBarAttributes.h
          KDChart/ChartGraphicsItem.h
          KDChart/KDChartAbstractArea.h
          KDChart/KDChartAbstractAreaBase.h
          KDChart/KDChartAbstractAreaWidget.h
          KDChart/KDChartAbstractAxis.h
          KDChart/KDChartAbstractCoordinatePlane.h
          KDChart/KDChartAbstractDiagram.h
          KDChart/KDChartAbstractGrid.h
          KDChart/KDChartAbstractProxyModel.h
          KDChart/KDChartAbstractThreeDAttributes.h
          KDChart/KDChartAttributesModel.h
          KDChart/KDChartBackgroundAttributes.h
          KDChart/KDChartChart.h
          KDChart/KDChartDatasetProxyModel.h
          KDChart/KDChartDatasetSelector.h
          KDChart/KDChartDataValueAttributes.h
          KDChart/KDChartDiagramObserver.h
          KDChart/KDChartEnums.h
          KDChart/KDChartFrameAttributes.h
          KDChart/KDChartGlobal.h
          KDChart/KDChartGridAttributes.h
          KDChart/KDChartHeaderFooter.h
          KDChart/KDChartLayoutItems.h
          KDChart/KDChartLegend.h
          KDChart/KDChartLineAttributes.h
          KDChart/KDChartMarkerAttributes.h
          KDChart/KDChartMeasure.h
          KDChart/KDChartNullPaintDevice.h
          KDChart/KDChartPaintContext.h
          KDChart/KDChartPalette.h
          KDChart/KDChartPosition.h
          KDChart/KDChartPrintingParameters.h
          KDChart/KDChartRelativePosition.h
          KDChart/KDChartRulerAttributes.h
          KDChart/KDChartTextArea.h
          KDChart/KDChartTextAttributes.h
          KDChart/KDChartTextLabelCache.h
          KDChart/KDChartThreeDLineAttributes.h
          KDChart/KDChartValueTrackerAttributes.h
          KDChart/KDChartWidget.h
          KDChart/KDChartZoomParameters.h
          KDChart/kdchart_export.h
          KDChart/KDTextDocument.h
          KDChart/Polar/KDChartAbstractPieDiagram.h
          KDChart/Polar/KDChartAbstractPolarDiagram.h
          KDChart/Polar/KDChartPieAttributes.h
          KDChart/Polar/KDChartPieDiagram.h
          KDChart/Polar/KDChartPolarCoordinatePlane.h
          KDChart/Polar/KDChartPolarDiagram.h
          KDChart/Polar/KDChartPolarGrid.h
          KDChart/Polar/KDChartRadarCoordinatePlane.h
          KDChart/Polar/KDChartRadarDiagram.h
          KDChart/Polar/KDChartRadarGrid.h
          KDChart/Polar/KDChartRingDiagram.h
          KDChart/Polar/KDChartThreeDPieAttributes.h
          KDChart/ReverseMapper.h
          KDChart/Ternary/KDChartAbstractTernaryDiagram.h
          KDChart/Ternary/KDChartTernaryAxis.h
          KDChart/Ternary/KDChartTernaryCoordinatePlane.h
          KDChart/Ternary/KDChartTernaryGrid.h
          KDChart/Ternary/KDChartTernaryLineDiagram.h
          KDChart/Ternary/KDChartTernaryPointDiagram.h
          KDChart/Ternary/TernaryConstants.h
          KDChart/Ternary/TernaryPoint.h
          KDGantt/kdganttabstractgrid.h
          KDGantt/kdganttabstractrowcontroller.h
          KDGantt/kdganttconstraint.h
          KDGantt/kdganttconstraintgraphicsitem.h
          KDGantt/kdganttconstraintmodel.h
          KDGantt/kdganttconstraintproxy.h
          KDGantt/kdganttdatetimegrid.h
          KDGantt/kdganttforwardingproxymodel.h
          KDGantt/kdganttglobal.h
          KDGantt/kdganttgraphicsitem.h
          KDGantt/kdganttgraphicsscene.h
          KDGantt/kdganttgraphicsview.h
          KDGantt/kdganttitemdelegate.h
          KDGantt/kdganttlegend.h
          KDGantt/kdganttlistviewrowcontroller.h
          KDGantt/kdganttproxymodel.h
          KDGantt/kdganttstyleoptionganttitem.h
          KDGantt/kdganttsummaryhandlingproxymodel.h
          KDGantt/kdgantttreeviewrowcontroller.h
          KDGantt/kdganttview.h
    DESTINATION ${INSTALL_INCLUDE_DIR}/KDChart${${PROJECT_NAME}_LIBRARY_QTID}
)

set(SOURCES
    # No cpp counter part source files, so automoc need to know them
    KDChart/KDChartZoomParameters.h
    KDChart/KDChartNullPaintDevice.h
    KDChart/KDChartGlobal.h
    KDChart/KDChartEnums.h
    KDChart/Cartesian/CartesianCoordinateTransformation.h
    KDChart/KDChartPainterSaver_p.h
    # Sources
    KDChart/KDChartMeasure.cpp
    KDChart/KDChartAbstractCoordinatePlane.cpp
    KDChart/KDChartChart.cpp
    KDChart/KDChartWidget.cpp
    KDChart/KDChartAbstractDiagram.cpp
    KDChart/KDChartAbstractDiagram_p.cpp
    KDChart/KDChartAbstractAreaBase.cpp
    KDChart/KDChartAbstractArea.cpp
    KDChart/KDChartTextArea.cpp
    KDChart/KDChartAbstractAreaWidget.cpp
    KDChart/KDChartAbstractAxis.cpp
    KDChart/KDChartAbstractProxyModel.cpp
    KDChart/KDChartAbstractGrid.cpp
    KDChart/KDChartAttributesModel.cpp
    KDChart/KDChartBackgroundAttributes.cpp
    KDChart/KDChartDatasetProxyModel.cpp
    KDChart/KDChartDatasetSelector.cpp
    KDChart/KDChartDataValueAttributes.cpp
    KDChart/KDChartDiagramObserver.cpp
    KDChart/KDChartFrameAttributes.cpp
    KDChart/KDChartGridAttributes.cpp
    KDChart/KDChartRulerAttributes.cpp
    KDChart/KDChartHeaderFooter.cpp
    KDChart/KDChartLayoutItems.cpp
    KDChart/KDChartLegend.cpp
    KDChart/KDChartLineAttributes.cpp
    KDChart/KDChartMarkerAttributes.cpp
    KDChart/KDChartPaintContext.cpp
    KDChart/KDChartPalette.cpp
    KDChart/KDChartPosition.cpp
    KDChart/KDChartRelativePosition.cpp
    KDChart/KDTextDocument.cpp
    KDChart/KDChartTextAttributes.cpp
    KDChart/KDChartAbstractThreeDAttributes.cpp
    KDChart/KDChartThreeDLineAttributes.cpp
    KDChart/KDChartTextLabelCache.cpp
    KDChart/ChartGraphicsItem.cpp
    KDChart/ReverseMapper.cpp
    KDChart/KDChartValueTrackerAttributes.cpp
    KDChart/KDChartPrintingParameters.cpp
    KDChart/KDChartModelDataCache_p.cpp
    KDChart/Cartesian/KDChartAbstractCartesianDiagram.cpp
    KDChart/Cartesian/KDChartCartesianCoordinatePlane.cpp
    KDChart/Cartesian/KDChartCartesianAxis.cpp
    KDChart/Cartesian/KDChartCartesianGrid.cpp
    KDChart/Cartesian/KDChartBarDiagram.cpp
    KDChart/Cartesian/KDChartBarDiagram_p.cpp
    KDChart/Cartesian/KDChartThreeDBarAttributes.cpp
    KDChart/Cartesian/KDChartBarAttributes.cpp
    KDChart/Cartesian/KDChartStockBarAttributes.cpp
    KDChart/Cartesian/KDChartStockDiagram.cpp
    KDChart/Cartesian/KDChartStockDiagram_p.cpp
    KDChart/Cartesian/KDChartLineDiagram.cpp
    KDChart/Cartesian/KDChartLineDiagram_p.cpp
    KDChart/Cartesian/KDChartCartesianDiagramDataCompressor_p.cpp
    KDChart/Cartesian/KDChartPlotter.cpp
    KDChart/Cartesian/KDChartPlotter_p.cpp
    KDChart/Cartesian/KDChartPlotterDiagramCompressor.cpp
    KDChart/Cartesian/KDChartLeveyJenningsCoordinatePlane.cpp
    KDChart/Cartesian/KDChartLeveyJenningsDiagram.cpp
    KDChart/Cartesian/KDChartLeveyJenningsDiagram_p.cpp
    KDChart/Cartesian/KDChartLeveyJenningsGrid.cpp
    KDChart/Cartesian/KDChartLeveyJenningsGridAttributes.cpp
    KDChart/Cartesian/KDChartLeveyJenningsAxis.cpp
    KDChart/Cartesian/PaintingHelpers_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartNormalPlotter_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartPercentPlotter_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartStackedLyingBarDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartStackedLineDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartStackedBarDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartPercentBarDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartPercentLyingBarDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartPercentLineDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartNormalBarDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartNormalLyingBarDiagram_p.cpp
    KDChart/Cartesian/DiagramFlavors/KDChartNormalLineDiagram_p.cpp
    KDChart/Polar/KDChartPolarCoordinatePlane.cpp
    KDChart/Polar/KDChartRadarCoordinatePlane.cpp
    KDChart/Polar/KDChartAbstractPieDiagram.cpp
    KDChart/Polar/KDChartAbstractPolarDiagram.cpp
    KDChart/Polar/KDChartPolarGrid.cpp
    KDChart/Polar/KDChartRadarGrid.cpp
    KDChart/Polar/KDChartPieDiagram.cpp
    KDChart/Polar/KDChartPolarDiagram.cpp
    KDChart/Polar/KDChartRadarDiagram.cpp
    KDChart/Polar/KDChartRingDiagram.cpp
    KDChart/Polar/KDChartPieAttributes.cpp
    KDChart/Polar/KDChartThreeDPieAttributes.cpp
    KDChart/Ternary/KDChartTernaryAxis.cpp
    KDChart/Ternary/KDChartTernaryGrid.cpp
    KDChart/Ternary/TernaryPoint.cpp
    KDChart/Ternary/TernaryConstants.cpp
    KDChart/Ternary/KDChartTernaryCoordinatePlane.cpp
    KDChart/Ternary/KDChartAbstractTernaryDiagram.cpp
    KDChart/Ternary/KDChartTernaryPointDiagram.cpp
    KDChart/Ternary/KDChartTernaryLineDiagram.cpp
    KDGantt/kdganttglobal.cpp
    KDGantt/kdganttview.cpp
    KDGantt/kdganttstyleoptionganttitem.cpp
    KDGantt/kdganttgraphicsview.cpp
    KDGantt/kdganttabstractrowcontroller.cpp
    KDGantt/kdgantttreeviewrowcontroller.cpp
    KDGantt/kdganttlistviewrowcontroller.cpp
    KDGantt/kdganttgraphicsscene.cpp
    KDGantt/kdganttgraphicsitem.cpp
    KDGantt/kdganttconstraint.cpp
    KDGantt/kdganttconstraintproxy.cpp
    KDGantt/kdganttconstraintgraphicsitem.cpp
    KDGantt/kdganttitemdelegate.cpp
    KDGantt/kdganttforwardingproxymodel.cpp
    KDGantt/kdganttsummaryhandlingproxymodel.cpp
    KDGantt/kdganttproxymodel.cpp
    KDGantt/kdganttconstraintmodel.cpp
    KDGantt/kdganttabstractgrid.cpp
    KDGantt/kdganttdatetimegrid.cpp
    KDGantt/kdganttlegend.cpp
    KDGantt/unittest/test.cpp
    KDGantt/unittest/testregistry.cpp
)

# Check ld version script support
include(CheckCXXSourceCompiles)
if(NOT CMAKE_REQUIRED_FLAGS) # to make --warn-uninitialized happy
    set(CMAKE_REQUIRED_FLAGS "")
endif()
set(CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})
set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} "-Wl,--version-script=\"${CMAKE_CURRENT_SOURCE_DIR}/libkdchart.map\"")
check_cxx_source_compiles("int main(void) { return 0; }" HAVE_LD_VERSION_SCRIPT)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_SAVE})

add_library(
    kdchart
    ${KDChart_LIBRARY_MODE} ${SOURCES} ${FORMS} ${RESOURCES}
)
add_library(
    KDAB::kdchart ALIAS kdchart
)
set_target_properties(
    kdchart
    PROPERTIES SOVERSION ${${PROJECT_NAME}_SOVERSION}
               VERSION ${${PROJECT_NAME}_VERSION}
               OUTPUT_NAME "kdchart${${PROJECT_NAME}_LIBRARY_QTID}"
)
if(${PROJECT_NAME}_STATIC)
    target_compile_definitions(kdchart PUBLIC KDCHART_STATICLIB)
else()
    target_compile_definitions(kdchart PRIVATE -DKDCHART_BUILD_KDCHART_LIB)
endif()
target_link_libraries(
    kdchart ${QT_LIBRARIES}
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND HAVE_LD_VERSION_SCRIPT)
    set_target_properties(
        kdchart PROPERTIES LINK_FLAGS "-Wl,--version-script,\"${CMAKE_CURRENT_SOURCE_DIR}/libkdchart.map\""
    )
    set_target_properties(kdchart PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libkdchart.map)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    target_link_libraries(kdchart sunmath)
endif()
set_target_properties(kdchart PROPERTIES DEFINE_SYMBOL KDCHART_BUILD_KDCHART_LIB)
target_include_directories(
    kdchart
    INTERFACE "$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}/KDChart${${PROJECT_NAME}_LIBRARY_QTID}>"
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/kdablibfakes/include>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KDGantt>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KDChart>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KDChart/Cartesian>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KDChart/Cartesian/DiagramFlavors>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KDChart/Polar>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/KDChart/Ternary>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/KDChart>"
)

#version libraries on Windows
if(WIN32)
    set(postfix ${${PROJECT_NAME}_SOVERSION})
    string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_BUILD_TYPE)
    if(${UPPER_BUILD_TYPE} MATCHES "^DEBUG")
        string(CONCAT postfix ${postfix} "d")
        set_target_properties(kdchart PROPERTIES DEBUG_POSTFIX ${postfix})
    else()
        set_target_properties(kdchart PROPERTIES ${UPPER_BUILD_TYPE}_POSTFIX ${postfix})
    endif()
endif()

# Generate library version files
include(ECMSetupVersion)
ecm_setup_version(
    ${${PROJECT_NAME}_VERSION}
    VARIABLE_PREFIX
    KDCHART
    VERSION_HEADER
    "${CMAKE_CURRENT_BINARY_DIR}/kdchart_version.h"
    PACKAGE_VERSION_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChart${${PROJECT_NAME}_LIBRARY_QTID}ConfigVersion.cmake"
    SOVERSION
    ${${PROJECT_NAME}_SOVERSION}
    COMPATIBILITY
    AnyNewerVersion
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/kdchart_version.h"
    DESTINATION ${INSTALL_INCLUDE_DIR}/KDChart${${PROJECT_NAME}_LIBRARY_QTID}
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KDChartConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KDChart${${PROJECT_NAME}_LIBRARY_QTID}Config.cmake"
    INSTALL_DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDChart${${PROJECT_NAME}_LIBRARY_QTID}"
    PATH_VARS INSTALL_INCLUDE_DIR
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/KDChart${${PROJECT_NAME}_LIBRARY_QTID}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChart${${PROJECT_NAME}_LIBRARY_QTID}ConfigVersion.cmake"
    DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDChart${${PROJECT_NAME}_LIBRARY_QTID}"
)

export(
    TARGETS kdchart
    NAMESPACE KDChart::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChartTargets.cmake"
)

install(
    TARGETS kdchart
    EXPORT KDChartTargets
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
)
if(MSVC AND NOT ${PROJECT_NAME}_STATIC)
    install(
        FILES "$<TARGET_PDB_FILE_DIR:kdchart>/$<TARGET_PDB_FILE_NAME:kdchart>"
        DESTINATION ${INSTALL_LIBRARY_DIR}
        CONFIGURATIONS Debug RelWithDebInfo
    )
endif()
