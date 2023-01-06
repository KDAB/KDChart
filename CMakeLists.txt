##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

# This is the top-level CMakeLists.txt file for the KDChart project.
#
# Pass the following variables to cmake to control the build:
# (See INSTALL.txt for more information)
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
# -DKDChart_DOCS=[true|false]
#  Build the API documentation. Enables the 'docs' build target.
#  Default=false
#
# -DKDChart_PYTHON_BINDINGS=[true|false]
#  Build/Generate Python bindings.  Always false for Debug builds.
#  (If your shiboken or pyside is installed in a non-standard locations
#  try passing the SHIBOKEN_CUSTOM_PREFIX and PYSIDE_CUSTOM_PREFIX variables.)
#  Default=false
#
# -DKDChart_PYTHON_BINDINGS_INSTALL_PREFIX=[path]
#  Alternative install path for Python bindings.
#  Default=CMAKE_INSTALL_PREFIX
#

cmake_minimum_required(VERSION 3.12)

# Allow using a non-KDAB install location.
set(KDAB_INSTALL
    True
    CACHE INTERNAL "Install to default KDAB Location"
)
if(DEFINED CMAKE_INSTALL_PREFIX)
    if(NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL "")
        set(KDAB_INSTALL
            False
            CACHE INTERNAL "Install to non-KDAB Location"
        )
    endif()
endif()

project(
    KDChart
    DESCRIPTION "A Qt-based library for creating business and scientific charts"
    HOMEPAGE_URL "https://github.com/KDAB/KDChart"
    LANGUAGES CXX
)

set(${PROJECT_NAME}_VERSION_MAJOR 3)
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(${PROJECT_NAME}_VERSION_PATCH 1)
set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}
)
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION}) #PROJECT_VERSION is needed by some ECM modules
set(${PROJECT_NAME}_SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR})

include(FeatureSummary)

option(${PROJECT_NAME}_QT6 "Build against Qt 6" OFF)
option(${PROJECT_NAME}_STATIC "Build statically" OFF)
option(${PROJECT_NAME}_TESTS "Build the tests" OFF)
option(${PROJECT_NAME}_EXAMPLES "Build the examples" ON)
option(${PROJECT_NAME}_DOCS "Build the API documentation" OFF)
option(${PROJECT_NAME}_PYTHON_BINDINGS "Build python bindings" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ECM/modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/KDAB/modules")

# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type "Debug")
endif()
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to ${default_build_type} as none was specified.")
    set(CMAKE_BUILD_TYPE
        "${default_build_type}"
        CACHE STRING "Choose the type of build." FORCE
    )
    # Set the possible values of build type for cmake-gui
    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
                 "Debug"
                 "Release"
                 "MinSizeRel"
                 "RelWithDebInfo"
    )
endif()

if(${PROJECT_NAME}_QT6)
    set(Qt_VERSION_MAJOR 6)
    set(QT_MIN_VERSION "6.0.0")
    find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets PrintSupport Svg)
    list(
        APPEND
        QT_LIBRARIES
        Qt6::Gui
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Svg
    )
    set(${PROJECT_NAME}_LIBRARY_QTID "-qt6")
else()
    set(Qt_VERSION_MAJOR 5)
    set(QT_MIN_VERSION "5.15.0") # at least Qt::endl is needed
    find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Gui Widgets PrintSupport Svg)
    list(
        APPEND
        QT_LIBRARIES
        Qt5::Gui
        Qt5::Widgets
        Qt5::PrintSupport
        Qt5::Svg
    )
    set(${PROJECT_NAME}_LIBRARY_QTID "")
endif()
include(KDQtInstallPaths) #to set QT_INSTALL_FOO variables

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(TARGET Qt6::Core)
    set(CMAKE_CXX_STANDARD 17)
endif()

# Default to hidden visibility for symbols
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

include(KDCompilerFlags)

add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQBA_NO_CAST_TO_VOID
    -DQBA_NO_CAST_TO_ASCII
    -DQT_NO_STL
)
add_definitions(-DUSE_EXCEPTIONS -DQT_FATAL_ASSERT)
if(MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()
if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(-DNDEBUG)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    add_definitions(-DSUN7)
endif()

#  NOMINMAX avoids conflicting definitions of min and max
#  _USE_MATH_DEFINES makes M_PI defined
if(MSVC)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR /EHsc /wd4251")
endif()

if(${PROJECT_NAME}_STATIC)
    set(${PROJECT_NAME}_LIBRARY_MODE "STATIC")
else()
    set(${PROJECT_NAME}_LIBRARY_MODE "SHARED")
endif()

if(KDAB_INSTALL)
    if(UNIX)
        set(CMAKE_INSTALL_PREFIX
            "/usr/local/KDAB/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}"
            CACHE INTERNAL "Install to default KDAB Location"
        )
    elseif(WIN32)
        set(CMAKE_INSTALL_PREFIX
            "C:\\KDAB\\${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}"
            CACHE INTERNAL "Install to default KDAB Location"
        )
    endif()
endif()

# setup default install locations
include(KDInstallLocation)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(${PROJECT_NAME}_IS_ROOT_PROJECT TRUE)

    message(STATUS "Building ${PROJECT_NAME} ${${PROJECT_NAME}_VERSION} in ${CMAKE_BUILD_TYPE} mode. "
                   "Installing to ${CMAKE_INSTALL_PREFIX}"
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")

    install(FILES README.md DESTINATION ${INSTALL_DOC_DIR})
    if(NOT ${PROJECT_NAME}_QT6)
        install(FILES kdchart.pri DESTINATION ${INSTALL_DOC_DIR})
    endif()
    install(DIRECTORY LICENSES DESTINATION ${INSTALL_DOC_DIR})

    install(
        EXPORT KDChartTargets
        NAMESPACE KDChart::
        DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/KDChart${${PROJECT_NAME}_LIBRARY_QTID}"
    )

    #Install the kdchart features file for qmake users
    add_subdirectory(features)

    # Generate .pri file for qmake users (except when using the VS generator)
    if(NOT CMAKE_CONFIGURATION_TYPES)
        if(Qt_VERSION_MAJOR EQUAL 5 OR (Qt_VERSION_MAJOR EQUAL 6 AND Qt6Core_VERSION VERSION_GREATER "6.2"))
            include(ECMGeneratePriFile) #not available for Qt6.2 or lower
            set(PROJECT_VERSION_STRING ${${PROJECT_NAME}_VERSION})
            ecm_generate_pri_file(
                BASE_NAME
                ${PROJECT_NAME}
                LIB_NAME
                kdchart${${PROJECT_NAME}_LIBRARY_QTID}
                FILENAME_VAR
                pri_filename
                INCLUDE_INSTALL_DIR
                ${INSTALL_INCLUDE_DIR}
            )
            install(FILES ${pri_filename} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
        endif()
    endif()
else()
    #Always disable tests, examples, docs when used as a submodule
    set(${PROJECT_NAME}_IS_ROOT_PROJECT FALSE)
    set(${PROJECT_NAME}_TESTS FALSE)
    set(${PROJECT_NAME}_EXAMPLES FALSE)
    set(${PROJECT_NAME}_DOCS FALSE)
endif()

if(${PROJECT_NAME}_TESTS)
    enable_testing()
endif()

add_subdirectory(src)

# only build python bindings for Qt5.12 or greater
if(${PROJECT_NAME}_PYTHON_BINDINGS)
    if(Qt_VERSION_MAJOR EQUAL 5 AND Qt5Core_VERSION VERSION_LESS 5.12)
        message(WARNING "** Disabling Python Bindings. Qt version is too old and unsupported.")
        set(${PROJECT_NAME}_PYTHON_BINDINGS OFF)
    elseif(CMAKE_BUILD_TYPE MATCHES "^[Dd]eb" OR ${PROJECT_NAME}_STATIC)
        message(FATAL_ERROR "** Python Bindings are disabled in debug or static builds.")
    endif()
endif()
if(${PROJECT_NAME}_PYTHON_BINDINGS)
    add_subdirectory(python)
endif()

if(${PROJECT_NAME}_TESTS OR ${PROJECT_NAME}_EXAMPLES)
    # Shared testtools lib
    add_library(testtools ${KDChart_LIBRARY_MODE} examples/tools/TableModel.cpp)

    target_link_libraries(testtools ${QT_LIBRARIES})
    target_compile_definitions(testtools PRIVATE KDCHART_BUILD_TESTTOOLS_LIB)

    if(NOT ${PROJECT_NAME}_STATIC)
        target_compile_definitions(testtools PRIVATE QT_SHARED TESTTOOLS_MAKEDLL)
    endif()

    target_include_directories(testtools PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/examples/tools>")
endif()

if(${PROJECT_NAME}_TESTS)
    find_package(Qt${Qt_VERSION_MAJOR}Test REQUIRED)
    list(APPEND QT_LIBRARIES Qt${Qt_VERSION_MAJOR}::Test)
    add_subdirectory(qtests)
    add_subdirectory(tests)
endif()

if(${PROJECT_NAME}_EXAMPLES)
    add_subdirectory(examples)
endif()

if(${PROJECT_NAME}_DOCS)
    add_subdirectory(docs) # needs to go last, in case there are build source files
endif()

if(${PROJECT_NAME}_IS_ROOT_PROJECT)
    # Add uninstall target (not for submodules since parent projects typically have uninstall too)
    include(ECMUninstallTarget)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
