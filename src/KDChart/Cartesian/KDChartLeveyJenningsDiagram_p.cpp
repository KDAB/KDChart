/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#include "KDChartLeveyJenningsDiagram.h"
#include "KDChartDataValueAttributes.h"

#include "KDChartLeveyJenningsDiagram_p.h"

using namespace KDChart;

LeveyJenningsDiagram::Private::Private(const Private &rhs)
    : LineDiagram::Private(rhs)
    , lotChangedPosition(rhs.lotChangedPosition)
    , fluidicsPackChangedPosition(rhs.fluidicsPackChangedPosition)
    , sensorChangedPosition(rhs.sensorChangedPosition)
    , fluidicsPackChanges(rhs.fluidicsPackChanges)
    , sensorChanges(rhs.sensorChanges)
    , scanLinePen(rhs.scanLinePen)
    , icons(rhs.icons)
    , expectedMeanValue(rhs.expectedMeanValue)
    , expectedStandardDeviation(rhs.expectedStandardDeviation)
{
}

void LeveyJenningsDiagram::Private::setYAxisRange() const
{
    auto *const plane = static_cast<CartesianCoordinatePlane *>(diagram->coordinatePlane());
    if (plane == nullptr)
        return;

    plane->setVerticalRange(QPair<qreal, qreal>(expectedMeanValue - 4 * expectedStandardDeviation,
                                                expectedMeanValue + 4 * expectedStandardDeviation));
}
