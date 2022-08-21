#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    Ganttproject-example
    main.cpp mainwindow.cpp projectmodel.cpp
)
target_link_libraries(
    Ganttproject-example ${QT_LIBRARIES} kdchart testtools
)
