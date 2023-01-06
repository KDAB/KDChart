#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    PlotterTimeline-example
    main.cpp timeaxis.cpp timechartmodel.cpp
)
target_link_libraries(
    PlotterTimeline-example ${QT_LIBRARIES} kdchart testtools
)
