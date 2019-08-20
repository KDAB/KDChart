include(${QT_USE_FILE})

if(Qt5Core_FOUND)
  find_package(Qt5Widgets REQUIRED)
  list(APPEND QT_LIBRARIES Qt5::Widgets)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

function(ADD_KDCHART_MANUAL_TEST subfolder)
  set(name "${subfolder}-manual-test")
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
  # We don't want them to run from 'make test' as it would stuck the CI
  #add_test(NAME ${name} COMMAND ${name})
endfunction()

# Tests
ADD_KDCHART_MANUAL_TEST("DelayedData")
ADD_KDCHART_MANUAL_TEST("Gantt/apireview")
ADD_KDCHART_MANUAL_TEST("Gantt/customconstraints")
ADD_KDCHART_MANUAL_TEST("Gantt/gfxview")
ADD_KDCHART_MANUAL_TEST("Gantt/headers")
ADD_KDCHART_MANUAL_TEST("Gantt/listview")
ADD_KDCHART_MANUAL_TEST("Gantt/reorder")
ADD_KDCHART_MANUAL_TEST("Gantt/unittest")
ADD_KDCHART_MANUAL_TEST("Gantt/view")
ADD_KDCHART_MANUAL_TEST("RootIndex")
