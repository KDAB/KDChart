## This file is part of the KD Chart library.
##
## SPDX-FileCopyrightText: 2019-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

add_executable(demo-example axissettings.cpp axissettings.ui colorslider.cpp datasetsettings.cpp datasetsettings.ui datavaluesettings.cpp datavaluesettings.ui demo.qrc diagramsettings.cpp diagramsettings.ui diagramtypedialog.cpp diagramtypedialog.ui gradientdialog.cpp gradientdialog.ui main.cpp mainwindow.cpp)
target_link_libraries(demo-example ${QT_LIBRARIES} kdchart testtools)
