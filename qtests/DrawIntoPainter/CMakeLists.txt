##
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    DrawIntoPainter-test
    DrawIntoPainter.qrc
    framewidget.cpp
    main.cpp
    mainwindow.cpp
    mainwindow.ui
)
target_link_libraries(
    DrawIntoPainter-test ${QT_LIBRARIES} kdchart testtools
)
add_test(NAME DrawIntoPainter-test COMMAND DrawIntoPainter-test)
