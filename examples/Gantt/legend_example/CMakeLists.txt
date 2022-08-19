## This file is part of the KD Chart library.
##
## SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
##
## SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
##
## Licensees holding valid commercial KD Chart licenses may use this file in
## accordance with the KD Chart Commercial License Agreement provided with
## the Software.
##
## Contact info@kdab.com if any conditions of this licensing are not
## clear to you.
##

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
