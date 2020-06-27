set(QT_USE_QTTEST TRUE)

include(${QT_USE_FILE})

if(Qt5Core_FOUND)
  find_package(Qt5Widgets REQUIRED)
  list(APPEND QT_LIBRARIES Qt5::Widgets)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

if(${PROJECT_NAME}_STATIC)
  add_definitions(-DKDCHART_STATICLIB)
endif()

# For Qt::WFlag...
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x000000)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

# Tests
add_subdirectory(AttributesModel)
add_subdirectory(AxisOwnership)
add_subdirectory(BarDiagrams)
add_subdirectory(CartesianDiagramDataCompressor)
add_subdirectory(CartesianPlanes)
add_subdirectory(ChartElementOwnership)
add_subdirectory(Cloning)
add_subdirectory(DrawIntoPainter)
add_subdirectory(Legends)
add_subdirectory(LineDiagrams)
add_subdirectory(Measure)
add_subdirectory(Palette)
add_subdirectory(ParamVsParam)
add_subdirectory(PieDiagrams)
add_subdirectory(PolarDiagrams)
add_subdirectory(PolarPlanes)
add_subdirectory(QLayout)
add_subdirectory(RelativePosition)
add_subdirectory(WidgetElementOwnership)
