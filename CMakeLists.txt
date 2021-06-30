# This is the top-level CMakeLists.txt file for the KDChart project.
#
# Pass the following variables to cmake to control the build:
# (See INSTALL-cmake.txt for more information)
#
# -DKDChart_QT6=[true|false]
#  Build against Qt6 rather than Qt5
#  Default=false (Qt5 will be used even if Qt6 is available)
#
# -DKDChart_STATIC=[true|false]
#  Build static versions of the libraries
#  Default=false
#
# -DKDChart_TESTS=[true|false]
#  Build the test harness.
#  Default=false
#
# -DKDChart_EXAMPLES=[true|false]
#  Build the examples.
#  Default=true
#
# -DKDChart_PYTHON_BINDINGS=[true|false]
#  Build/Generate Python bindings.  Always false for Debug builds.
#  Default=false
#
# -DKDChart_PYTHON_BINDINGS_INSTALL_PREFIX=[path]
#  Alternative install path for Python bindings.
#  Default=CMAKE_INSTALL_PREFIX
#
cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0025 NEW)

set(KDAB_INSTALL True)
if((NOT DEFINED ${USE_DEFAULT_INSTALL_LOCATION}) OR (NOT ${USE_DEFAULT_INSTALL_LOCATION}))
  if(NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL "")
    set(KDAB_INSTALL False)
  endif()
  set(USE_DEFAULT_INSTALL_LOCATION ${KDAB_INSTALL} CACHE INTERNAL "Install to default KDAB Location" FORCE)
endif()

project(KDChart CXX)

option(${PROJECT_NAME}_QT6 "Build against Qt 6" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_MODULE_PATH}")

option(${PROJECT_NAME}_STATIC "Build statically" OFF)
option(${PROJECT_NAME}_TESTS "Build the tests" OFF)
option(${PROJECT_NAME}_EXAMPLES "Build the examples" ON)
option(${PROJECT_NAME}_PYTHON_BINDINGS "Build python bindings" OFF)
if(${PROJECT_NAME}_PYTHON_BINDINGS AND (CMAKE_BUILD_TYPE MATCHES "^[Dd]eb" OR ${PROJECT_NAME}_STATIC))
  message(FATAL_ERROR "** Python Bindings are disabled in debug or static builds.")
endif()

set(${PROJECT_NAME}_VERSION_MAJOR 2)
set(${PROJECT_NAME}_VERSION_MINOR 99)
set(${PROJECT_NAME}_VERSION_PATCH 50)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})
set(${PROJECT_NAME}_SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# setup default install locations
include(InstallLocation)

# try Qt5 first, and prefer that (if found), but only if not disabled via option
if(${PROJECT_NAME}_QT6)
  set(QT_MIN_VERSION "6.0.0")
  find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets Network PrintSupport Xml Svg Test)
  set(QT_MAJOR_VERSION 6)
  list(APPEND QT_LIBRARIES Qt6::Widgets Qt6::Network Qt6::PrintSupport Qt6::Xml Qt6::Svg Qt6::Test)
  set(${PROJECT_NAME}_LIBRARY_QTID "-qt6")
else()
  set(QT_MIN_VERSION "5.15.0") # at least Qt::endl is needed
  find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets Network PrintSupport Xml Svg Test)
  set(QT_MAJOR_VERSION 5)
  list(APPEND QT_LIBRARIES Qt5::Widgets Qt5::Network Qt5::PrintSupport Qt5::Xml Qt5::Svg Qt5::Test)
  set(${PROJECT_NAME}_LIBRARY_QTID "")
endif()

# extra optional modules
find_package(Qt${QT_MAJOR_VERSION}Sql)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)


add_definitions(-DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII -DQBA_NO_CAST_TO_VOID -DQBA_NO_CAST_TO_ASCII -DQT_NO_STL -DQT_FATAL_ASSERT)
add_definitions(-DUSE_EXCEPTIONS)
if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
  add_definitions(-DSUN7)
endif()
# Note: The following KDCHART_MASTER_CVS definition is used to distinguish
#       between Klaraelvdalens Datakonsult AB Master CVS and other CVS
#       repositories holding copies of the sources that might have to
#       use special statements like include "KDFrame.moc" in the code.
add_definitions(-DKDCHART_VERSION=200 -DKDCHART_MASTER_CVS)
#  NOMINMAX avoids conflicting definitions of min and max
#  _USE_MATH_DEFINES makes M_PI defined
if(MSVC)
  add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR /EHsc /wd4251")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wsuggest-override)
endif()

if(${PROJECT_NAME}_STATIC)
  set(${PROJECT_NAME}_LIBRARY_MODE "STATIC")
else()
  set(${PROJECT_NAME}_LIBRARY_MODE "SHARED")
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(${PROJECT_NAME}_IS_ROOT_PROJECT TRUE)

  if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(-DNDEBUG)
  endif()

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
  endif()

  if(USE_DEFAULT_INSTALL_LOCATION)
    if(UNIX)
      set(CMAKE_INSTALL_PREFIX "/usr/local/KDAB/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}")
    elseif(WIN32)
      set(CMAKE_INSTALL_PREFIX "C:\\KDAB\\${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}")
    endif()
  endif()

  message(STATUS "Building ${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} in ${CMAKE_BUILD_TYPE} mode. Installing to ${CMAKE_INSTALL_PREFIX}")

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")

  install(FILES LICENSE.GPL.txt LICENSE.txt LICENSE.US.txt README.txt README-commercial.txt kdchart.pri DESTINATION ${INSTALL_DOC_DIR})

  include(CMakePackageConfigHelpers)

  write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChartConfigVersion.cmake"
    VERSION ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KDChartConfig-buildtree.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChartConfig.cmake"
    @ONLY
  )

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KDChartConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/install/KDChartConfig.cmake"
    INSTALL_DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDChart"
    PATH_VARS INSTALL_INCLUDE_DIR
  )

  install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/install/KDChartConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChartConfigVersion.cmake"
    DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDChart"
  )
  install(EXPORT KDChartTargets NAMESPACE KDChart::
    DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDChart"
  )

  # Generate .pri file for qmake users
  include(ECMGeneratePriFile)
  set(PROJECT_VERSION_STRING ${${PROJECT_NAME}_VERSION})
  ecm_generate_pri_file(BASE_NAME ${PROJECT_NAME}
                        LIB_NAME kdchart
                        FILENAME_VAR pri_filename)
  install(FILES ${pri_filename} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
endif()

add_subdirectory(src)

# only build python bindings for Qt5.12 or greater
if(${PROJECT_NAME}_PYTHON_BINDINGS AND Qt5Core_VERSION VERSION_LESS 5.12)
  message(WARNING "** Disabling Python Bindings. Qt version is too old and unsupported.")
  set(${PROJECT_NAME}_PYTHON_BINDINGS OFF)
endif()

if(Qt5Core_FOUND AND ${PROJECT_NAME}_PYTHON_BINDINGS)
  add_subdirectory(python)
endif()

if(${PROJECT_NAME}_IS_ROOT_PROJECT)
  export(TARGETS kdchart NAMESPACE KDChart::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/KDChart/KDChartTargets.cmake"
  )

  if(${PROJECT_NAME}_TESTS OR ${PROJECT_NAME}_EXAMPLES)
    # Shared testtools lib
    add_library(testtools ${KDChart_LIBRARY_MODE} examples/tools/TableModel.cpp)

    target_link_libraries(testtools ${QT_LIBRARIES})
    target_compile_definitions(testtools PRIVATE KDCHART_BUILD_TESTTOOLS_LIB)

    if(NOT ${PROJECT_NAME}_STATIC)
      target_compile_definitions(testtools PRIVATE QT_SHARED TESTTOOLS_MAKEDLL)
    endif()

    target_include_directories(testtools
      PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/examples/tools>"
    )
  endif()

  add_subdirectory(features)
  if(${PROJECT_NAME}_TESTS)
    enable_testing()
    add_subdirectory(qtests)
    add_subdirectory(tests)
  endif()
  if(${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(examples)
  endif()

endif()
