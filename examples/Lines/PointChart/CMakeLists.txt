# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    LinesPointChart-example
    LineChart.qrc main.cpp mainwindow.cpp mainwindow.ui
)
target_link_libraries(
    LinesPointChart-example ${QT_LIBRARIES} kdchart testtools
)
