/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartAbstractTernaryDiagram_p.h"

#include "KDChartTernaryCoordinatePlane.h"

using namespace KDChart;

AbstractTernaryDiagram::Private::Private()
    : AbstractDiagram::Private()
{
}

void AbstractTernaryDiagram::init()
{
}

#define d d_func()

AbstractTernaryDiagram::AbstractTernaryDiagram(QWidget *parent,
                                               TernaryCoordinatePlane *plane)
    : AbstractDiagram(parent, plane)
{
}

AbstractTernaryDiagram::~AbstractTernaryDiagram()
{
    while (!d->axesList.isEmpty()) {
        TernaryAxis *axis = d->axesList.takeFirst();
        delete axis;
    }
}

void AbstractTernaryDiagram::addAxis(TernaryAxis *axis)
{
    d->axesList.append(axis);
    // FIXME update
}

void AbstractTernaryDiagram::takeAxis(TernaryAxis *axis)
{
    int index = d->axesList.indexOf(axis);
    if (index != -1)
        d->axesList.removeAt(index);
    // FIXME update
}

TernaryAxisList AbstractTernaryDiagram::axes() const
{
    return d->axesList;
}

void AbstractTernaryDiagram::paint(PaintContext *paintContext)
{
    d->paint(paintContext);
}
