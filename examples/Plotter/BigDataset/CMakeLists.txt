# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    PlotterBigDataset-example
    Controls.ui main.cpp MainWidget.cpp Model.cpp
)
target_link_libraries(
    PlotterBigDataset-example ${QT_LIBRARIES} kdchart testtools
)
