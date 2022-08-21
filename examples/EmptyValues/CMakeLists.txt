#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    EmptyValues-example
    BarChart.qrc main.cpp mainwindow.cpp
)
target_link_libraries(
    EmptyValues-example ${QT_LIBRARIES} kdchart testtools
)
