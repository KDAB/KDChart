#
# This file is part of the KD Chart library.
#
# SPDX-FileCopyrightText: 2019-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#

add_executable(
    demo-example
    axissettings.cpp
    axissettings.ui
    colorslider.cpp
    datasetsettings.cpp
    datasetsettings.ui
    datavaluesettings.cpp
    datavaluesettings.ui
    demo.qrc
    diagramsettings.cpp
    diagramsettings.ui
    diagramtypedialog.cpp
    diagramtypedialog.ui
    gradientdialog.cpp
    gradientdialog.ui
    main.cpp
    mainwindow.cpp
)
target_link_libraries(
    demo-example ${QT_LIBRARIES} kdchart testtools
)
