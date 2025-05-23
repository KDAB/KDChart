# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

# Examples
add_subdirectory(Axis/Chart)
add_subdirectory(Axis/Labels)
add_subdirectory(Axis/Parameters)
add_subdirectory(Axis/Widget)
add_subdirectory(Background)
add_subdirectory(Bars/Advanced)
add_subdirectory(Bars/Parameters)
add_subdirectory(Bars/Simple)
add_subdirectory(DataValueTexts)
add_subdirectory(demo)
add_subdirectory(DrawIntoPainter)
add_subdirectory(EmptyValues)
add_subdirectory(Gantt/legend_example)
add_subdirectory(Gantt/project)
add_subdirectory(Grids/CartesianGrid)
add_subdirectory(Grids/PolarGrid)
add_subdirectory(HeadersFooters/Advanced)
add_subdirectory(HeadersFooters/HeadersFootersParameters)
add_subdirectory(HeadersFooters/HeadersFootersSimple)
add_subdirectory(Legends/LegendAdvanced)
add_subdirectory(Legends/LegendOverDiagram)
add_subdirectory(Legends/LegendParameters)
add_subdirectory(Legends/LegendSimple)
add_subdirectory(LeveyJennings/Simple)
add_subdirectory(Lines/Advanced)
add_subdirectory(Lines/AreaPerCell)
add_subdirectory(Lines/Parameters)
add_subdirectory(Lines/PointChart)
add_subdirectory(Lines/PointWidget)
add_subdirectory(Lines/SimpleLineWidget)
add_subdirectory(ModelView/HidingData)
add_subdirectory(ModelView/TableView)
add_subdirectory(NoValues)
add_subdirectory(Pie/Advanced)
add_subdirectory(Pie/Parameters)
add_subdirectory(Pie/Simple)
add_subdirectory(Plane/AdjustedBoundaries)
add_subdirectory(Plane/OverlayedPlanes)
add_subdirectory(Plotter/Advanced)
add_subdirectory(Plotter/BigDataset)
add_subdirectory(Plotter/BubbleChart)
add_subdirectory(Plotter/Isometric)
add_subdirectory(Plotter/Logarithmic)
add_subdirectory(Plotter/Simple)
add_subdirectory(Plotter/Timeline)
add_subdirectory(Polar/Advanced)
add_subdirectory(Polar/Parameters)
add_subdirectory(Polar/Simple)
add_subdirectory(RealTime)
add_subdirectory(SharedAbscissa/SeparateDiagrams)
if(NOT EMSCRIPTEN)
    find_package(Qt${QT_VERSION_MAJOR}Sql)
    if(TARGET Qt${QT_VERSION_MAJOR}::Sql)
        add_subdirectory(Sql)
    else()
        message(STATUS "QtSql module not found -- skipping the Sql example")
    endif()
endif()
add_subdirectory(Stock/Advanced)
add_subdirectory(TernaryCharts/Advanced)
add_subdirectory(Widget/Advanced)
add_subdirectory(Widget/Parameters)
add_subdirectory(Widget/Simple)
add_subdirectory(Zoom/Keyboard)
add_subdirectory(Zoom/ScrollBars)
