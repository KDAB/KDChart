/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartLeveyJenningsCoordinatePlane.h"
#include "KDChartLeveyJenningsCoordinatePlane_p.h"

#include <QPainter>
#include <QtDebug>

#include "KDChartCartesianAxis.h"
#include "KDChartLeveyJenningsDiagram.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"

using namespace KDChart;

#define d d_func()

LeveyJenningsCoordinatePlane::Private::Private()
    : CartesianCoordinatePlane::Private()
{
}

LeveyJenningsCoordinatePlane::LeveyJenningsCoordinatePlane(Chart *parent)
    : CartesianCoordinatePlane(new Private(), parent)
{
}

LeveyJenningsCoordinatePlane::~LeveyJenningsCoordinatePlane()
{
}

void LeveyJenningsCoordinatePlane::init()
{
}

void LeveyJenningsCoordinatePlane::addDiagram(AbstractDiagram *diagram)
{
    Q_ASSERT_X(dynamic_cast<LeveyJenningsDiagram *>(diagram),
               "LeveyJenningsCoordinatePlane::addDiagram", "Only Levey Jennings "
                                                           "diagrams can be added to a ternary coordinate plane!");
    CartesianCoordinatePlane::addDiagram(diagram);
}

LeveyJenningsGrid *LeveyJenningsCoordinatePlane::grid() const
{
    auto *leveyJenningsGrid = static_cast<LeveyJenningsGrid *>(d->grid);
    Q_ASSERT(dynamic_cast<LeveyJenningsGrid *>(d->grid));
    return leveyJenningsGrid;
}

LeveyJenningsGridAttributes LeveyJenningsCoordinatePlane::gridAttributes() const
{
    return d->gridAttributes;
}

void LeveyJenningsCoordinatePlane::setGridAttributes(const LeveyJenningsGridAttributes &attr)
{
    d->gridAttributes = attr;
}

const QPointF LeveyJenningsCoordinatePlane::translateBack(const QPointF &screenPoint) const
{
    return CartesianCoordinatePlane::translateBack(screenPoint);
}

#undef d
