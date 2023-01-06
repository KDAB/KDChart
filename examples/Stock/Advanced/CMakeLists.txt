#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    StockAdvanced-example
    main.cpp mainwindow.cpp mainwindow.ui Resources.qrc
)
target_link_libraries(
    StockAdvanced-example ${QT_LIBRARIES} kdchart testtools
)
