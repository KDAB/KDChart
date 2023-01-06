/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartPaintContext.h"
#include "KDChartAbstractCoordinatePlane.h"

#include <QPainter>
#include <QRectF>

#include <KDABLibFakes>

using namespace KDChart;

#define d (d_func())

class PaintContext::Private
{
public:
    QPainter *painter = nullptr;
    QRectF rect;
    AbstractCoordinatePlane *plane = nullptr;

    Private()
    {
    }
};

PaintContext::PaintContext()
    : _d(new Private())
{
}

PaintContext::~PaintContext()
{
    delete _d;
}

const QRectF PaintContext::rectangle() const
{
    return d->rect;
}

void PaintContext::setRectangle(const QRectF &rect)
{
    d->rect = rect;
}

QPainter *PaintContext::painter() const
{
    return d->painter;
}

void PaintContext::setPainter(QPainter *painter)
{
    d->painter = painter;
}

AbstractCoordinatePlane *PaintContext::coordinatePlane() const
{
    return d->plane;
}

void PaintContext::setCoordinatePlane(AbstractCoordinatePlane *plane)
{
    d->plane = plane;
}
