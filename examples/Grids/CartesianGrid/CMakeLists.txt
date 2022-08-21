#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    GridsCartesianGrid-example
    main.cpp
)
target_link_libraries(
    GridsCartesianGrid-example ${QT_LIBRARIES} kdchart testtools
)
