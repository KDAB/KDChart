/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
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
