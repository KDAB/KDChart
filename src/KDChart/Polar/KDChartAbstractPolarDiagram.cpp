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

#include "KDChartAbstractPolarDiagram.h"
#include "KDChartAbstractPolarDiagram_p.h"

#include <KDABLibFakes>

using namespace KDChart;

AbstractPolarDiagram::Private::Private()
{
}

AbstractPolarDiagram::Private::~Private()
{
}

void AbstractPolarDiagram::init()
{
}

#define d d_func()

AbstractPolarDiagram::AbstractPolarDiagram(
    QWidget *parent, PolarCoordinatePlane *plane)
    : AbstractDiagram(new Private(), parent, plane)
{
}

const PolarCoordinatePlane *AbstractPolarDiagram::polarCoordinatePlane() const
{
    return dynamic_cast<const PolarCoordinatePlane *>(coordinatePlane());
}

int AbstractPolarDiagram::columnCount() const
{
    return static_cast<int>(numberOfValuesPerDataset());
}

int AbstractPolarDiagram::rowCount() const
{
    return static_cast<int>(numberOfDatasets());
}
