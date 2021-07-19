if(Qt5Core_FOUND)
  find_package(Qt5Widgets REQUIRED)
  list(APPEND QT_LIBRARIES Qt5::Widgets)
  include_directories(${Qt5Widgets_INCLUDE_DIRS})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

# Tests
add_subdirectory(DelayedData)
add_subdirectory(Gantt/apireview)
add_subdirectory(Gantt/customconstraints)
add_subdirectory(Gantt/gfxview)
add_subdirectory(Gantt/headers)
add_subdirectory(Gantt/listview)
add_subdirectory(Gantt/reorder)
add_subdirectory(Gantt/unittest)
add_subdirectory(Gantt/view)
add_subdirectory(RootIndex)
