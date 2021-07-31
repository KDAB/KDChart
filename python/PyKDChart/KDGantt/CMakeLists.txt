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

set(PyKDGantt_SRC
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_abstractgrid_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_abstractgrid_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_abstractrowcontroller_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_abstractrowcontroller_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraintgraphicsitem_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraintgraphicsitem_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraintmodel_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraintmodel_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraintproxy_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraintproxy_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraint_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_constraint_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_datetimegrid_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_datetimegrid_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_datetimescaleformatter_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_datetimescaleformatter_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_forwardingproxymodel_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_forwardingproxymodel_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_graphicsscene_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_graphicsscene_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_graphicsview_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_graphicsview_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_itemdelegate_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_itemdelegate_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_legend_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_legend_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_listviewrowcontroller_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_listviewrowcontroller_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_proxymodel_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_proxymodel_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_python.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_span_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_span_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_styleoptionganttitem_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_styleoptionganttitem_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_summaryhandlingproxymodel_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_summaryhandlingproxymodel_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_treeviewrowcontroller_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_treeviewrowcontroller_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_view_wrapper.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_view_wrapper.h
  ${CMAKE_CURRENT_BINARY_DIR}/KDGantt/kdgantt_module_wrapper.cpp
)

set(PyKDGantt_include_paths
  $<JOIN:$<TARGET_PROPERTY:kdchart,INTERFACE_INCLUDE_DIRECTORIES>,${PATH_SEP}>
  $<JOIN:$<TARGET_PROPERTY:Qt${QT_MAJOR_VERSION}::Core,INTERFACE_INCLUDE_DIRECTORIES>,${PATH_SEP}>
  $<JOIN:$<TARGET_PROPERTY:Qt${QT_MAJOR_VERSION}::Widgets,INTERFACE_INCLUDE_DIRECTORIES>,${PATH_SEP}>
  $<JOIN:$<TARGET_PROPERTY:Qt${QT_MAJOR_VERSION}::PrintSupport,INTERFACE_INCLUDE_DIRECTORIES>,${PATH_SEP}>
)

set(PyKDGantt_typesystem_paths
  ${PYSIDE_TYPESYSTEMS}
)

set(PyKDGantt_target_include_directories
  ${PYSIDE_INCLUDE_DIR}/QtCore
  ${PYSIDE_INCLUDE_DIR}/QtGui
  ${PYSIDE_INCLUDE_DIR}/QtWidgets
)

set(PyKDGantt_target_link_libraries
  kdchart
  Qt5::Core
  Qt5::Widgets
  Qt5::PrintSupport
)

set(PyKDGantt_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/kdgantt_global.h
)

create_python_bindings(
  "KDGantt"
  "${PyKDGantt_typesystem_paths}"
  "${PyKDGantt_include_paths}"
  "${PyKDGantt_SRC}"
  "${PyKDGantt_target_include_directories}"
  "${PyKDGantt_target_link_libraries}"
  ${CMAKE_CURRENT_SOURCE_DIR}/kdgantt_global.h
  ${CMAKE_CURRENT_SOURCE_DIR}/typesystem_kdgantt.xml
  "${PyKDGantt_DEPENDS}"
  ${BINDING_OUTPUT_DIR}
)
