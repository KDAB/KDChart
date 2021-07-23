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

#include "KDChartRadarCoordinatePlane.h"
#include "KDChartRadarCoordinatePlane_p.h"

using namespace KDChart;

#define d d_func()

KDChart::RadarCoordinatePlane::RadarCoordinatePlane(Chart *parent)
    : PolarCoordinatePlane(new Private(), parent)
{
}
KDChart::RadarCoordinatePlane::~RadarCoordinatePlane()
{
}

void KDChart::RadarCoordinatePlane::setTextAttributes(const KDChart::TextAttributes &attr)
{
    d->textAttributes = attr;
}

const KDChart::TextAttributes RadarCoordinatePlane::textAttributes() const
{
    return d->textAttributes;
}

void RadarCoordinatePlane::init()
{
    // this block left empty intentionally
}
