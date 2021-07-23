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
