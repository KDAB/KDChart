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

# For Qt::WFlag...
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x000000)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)

# Tests
add_subdirectory(AttributesModel)
add_subdirectory(AxisOwnership)
add_subdirectory(BarDiagrams)
add_subdirectory(CartesianDiagramDataCompressor)
add_subdirectory(CartesianPlanes)
add_subdirectory(ChartElementOwnership)
add_subdirectory(Cloning)
add_subdirectory(DrawIntoPainter)
add_subdirectory(Legends)
add_subdirectory(LineDiagrams)
add_subdirectory(Measure)
add_subdirectory(Palette)
add_subdirectory(ParamVsParam)
add_subdirectory(PieDiagrams)
add_subdirectory(PolarDiagrams)
add_subdirectory(PolarPlanes)
add_subdirectory(QLayout)
add_subdirectory(RelativePosition)
add_subdirectory(WidgetElementOwnership)
