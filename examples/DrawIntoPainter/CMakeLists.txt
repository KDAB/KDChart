#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    DrawIntoPainter-example
    DrawIntoPainter.qrc
    framewidget.cpp
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)
target_link_libraries(
    DrawIntoPainter-example ${QT_LIBRARIES} kdchart testtools
)
