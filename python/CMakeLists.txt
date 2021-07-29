## This file is part of the KD Chart library.
##
## SPDX-FileCopyrightText: 2019-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
##
## SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
##
## Licensees holding valid commercial KD Chart licenses may use this file in
## accordance with the KD Chart Commercial License Agreement provided with
## the Software.
##
## Contact info@kdab.com if any conditions of this licensing are not
## clear to you.
##

set(PYTHON_BINDING_NAMESPACE "PyKDChart")

if(${PROJECT_NAME}_QT6)
  set(PYSIDE_MAJOR_VERSION "6")
  set(QtWidgets_VERSION ${Qt6Widgets_VERSION})
else()
  set(PYSIDE_MAJOR_VERSION "2")
  set(QtWidgets_VERSION ${Qt5Widgets_VERSION})
endif()

find_package(Python3 3.6 REQUIRED COMPONENTS Interpreter Development)
find_package(Shiboken${PYSIDE_MAJOR_VERSION} REQUIRED)
find_package(PySide${PYSIDE_MAJOR_VERSION} ${QtWidgets_VERSION} EXACT REQUIRED)

if(NOT ${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX)
  set(${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
endif()
set(BINDINGS_DIR "${INSTALL_LIBRARY_DIR}/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages/${PYTHON_BINDING_NAMESPACE}")
set(${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX "${${PROJECT_NAME}_PYTHON_BINDINGS_INSTALL_PREFIX}/${BINDINGS_DIR}")

include(PySide${PYSIDE_MAJOR_VERSION}ModuleBuild)
add_subdirectory(PyKDChart)
