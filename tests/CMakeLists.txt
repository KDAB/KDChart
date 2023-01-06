##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

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
