#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    Ganttlegend_example-example
    entrydelegate.cpp
    entrydialog.cpp
    entrydialog.ui
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)
target_link_libraries(
    Ganttlegend_example-example ${QT_LIBRARIES} kdchart testtools
)
