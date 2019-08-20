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

# For Qt::WFlag...
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x000000)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

function(ADD_KDCHART_TEST subfolder)
  set(name "${subfolder}-test")
  string(REPLACE "/" "" name "${name}")

  file(GLOB target_sources
    LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/${subfolder}/*.qrc"
    "${CMAKE_CURRENT_SOURCE_DIR}/${subfolder}/*.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/${subfolder}/*.cpp"
  )

  add_executable(${name} ${target_sources})
  target_link_libraries(${name} ${QT_LIBRARIES} kdchart testtools)
  add_test(NAME ${name} COMMAND ${name})
endfunction()

# Tests
ADD_KDCHART_TEST("AttributesModel")
ADD_KDCHART_TEST("AxisOwnership")
ADD_KDCHART_TEST("BarDiagrams")
ADD_KDCHART_TEST("CartesianDiagramDataCompressor")
ADD_KDCHART_TEST("CartesianPlanes")
ADD_KDCHART_TEST("ChartElementOwnership")
ADD_KDCHART_TEST("Cloning")
ADD_KDCHART_TEST("DrawIntoPainter")
ADD_KDCHART_TEST("Legends")
ADD_KDCHART_TEST("LineDiagrams")
ADD_KDCHART_TEST("Measure")
ADD_KDCHART_TEST("Palette")
ADD_KDCHART_TEST("ParamVsParam")
ADD_KDCHART_TEST("PieDiagrams")
ADD_KDCHART_TEST("PolarDiagrams")
ADD_KDCHART_TEST("PolarPlanes")
ADD_KDCHART_TEST("QLayout")
ADD_KDCHART_TEST("RelativePosition")
ADD_KDCHART_TEST("WidgetElementOwnership")
