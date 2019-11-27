# This is the top-level CMakeLists.txt file for the KDChart project.
#
# Pass the following variables to cmake to control the build:
# (See INSTALL-cmake.txt for more information)
#
# -DKDChart_ENFORCE_QT4_BUILD=[true|false]
#  Force building against Qt4, even if Qt5 is found.
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
# -DDISABLE_PYTHON_BINDINGS=[true|false]
#  Build/Generate python bindings.  Always true for Debug builds
#  Default=false
#
# -DPYTHON_BINDINGS_INSTALL_DIR=[path]
#  alternative install path for python bindings
#  Default=<auto detect>
#
cmake_minimum_required(VERSION 2.8.7)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0025 NEW)

if("${CMAKE_INSTALL_PREFIX}" STREQUAL "")
  set(USE_DEFAULT_INSTALL_LOCATION True)
else()
  set(USE_DEFAULT_INSTALL_LOCATION False)
endif()

project(KDChart CXX)

option(
  ${PROJECT_NAME}_ENFORCE_QT4_BUILD
  "Enable if you want to enforce a build with Qt4"
  OFF
)

if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  endif()
else()
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_MODULE_PATH}")

if(CMAKE_BUILD_TYPE MATCHES "^[Rr]el")
  option(DISABLE_PYTHON_BINDINGS "Build Python bindings" OFF)
else()
  set(DISABLE_PYTHON_BINDINGS ON)
endif()
option(${PROJECT_NAME}_STATIC "Build statically" OFF)
option(${PROJECT_NAME}_TESTS "Build the tests" OFF)
option(${PROJECT_NAME}_EXAMPLES "Build the examples" ON)

set(${PROJECT_NAME}_VERSION_MAJOR 2)
set(${PROJECT_NAME}_VERSION_MINOR 6)
set(${PROJECT_NAME}_VERSION_PATCH 50)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# setup default install locations
include(InstallLocation)

# try Qt5 first, and prefer that (if found), but only if not disabled via option
if(NOT ${PROJECT_NAME}_ENFORCE_QT4_BUILD)
  find_package(Qt5Core QUIET)
endif()

if(Qt5Core_FOUND)
  find_package(Qt5Network REQUIRED)
  find_package(Qt5Widgets CONFIG)
  set(QT_LIBRARIES Qt5::Core)
  set(QT_USE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Qt5Portability.cmake")

  if(Qt5_POSITION_INDEPENDENT_CODE)
    if(CMAKE_VERSION VERSION_LESS 2.8.9)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    elseif(NOT CMAKE_VERSION VERSION_LESS 2.8.11)
      if(Qt5Core_VERSION VERSION_LESS 5.1)
        set_property(TARGET Qt5::Core PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)
      endif()
    else()
      set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    endif()
  endif()

else()
  find_package(Qt4 4.7 QUIET REQUIRED QtCore QtMain)

  # Workaround qt4 moc bug, see https://bugzilla.redhat.com/show_bug.cgi?id=1396755
  add_definitions(-D_SYS_SYSMACROS_H_OUTER)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

add_definitions(-DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_FROM_BYTEARRAY -DQBA_NO_CAST_TO_VOID -DQBA_NO_CAST_TO_ASCII -DQT_NO_STL -DQT_FATAL_ASSERT)
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

  install(FILES LICENSE.GPL.txt LICENSE.US.txt LICENSE.txt README.txt kdchart.pri DESTINATION ${INSTALL_DOC_DIR})

  if(CMAKE_VERSION VERSION_LESS 2.8.8)
    include("${CMAKE_SOURCE_DIR}/cmake/CMakePackageConfigHelpers.cmake")
  else()
    include(CMakePackageConfigHelpers)
  endif()

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
endif()

add_subdirectory(src)

# only build python bindings for Qt5
if(Qt5Core_FOUND AND NOT DISABLE_PYTHON_BINDINGS)
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
