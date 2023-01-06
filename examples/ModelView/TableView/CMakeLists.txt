#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    ModelViewTableView-example
    main.cpp mainwindow.cpp ModelView.qrc
)
target_link_libraries(
    ModelViewTableView-example ${QT_LIBRARIES} kdchart testtools
)
