/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRCOORDINATEPLANE_P_H
#define KDCHARTABSTRCOORDINATEPLANE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartAbstractArea_p.h"
#include <KDChartAbstractCoordinatePlane.h>
#include <KDChartAbstractGrid.h>
#include <KDChartGridAttributes.h>
#include <KDChartZoomParameters.h>

#include <KDABLibFakes>

#include <QStack>

QT_BEGIN_NAMESPACE
class QRubberBand;
QT_END_NAMESPACE

namespace KDChart {

/**
 * \internal
 */
class AbstractCoordinatePlane::Private : public AbstractArea::Private
{
    friend class AbstractCoordinatePlane;

protected:
    explicit Private();
    ~Private() override
    {
        delete grid;
    };

    virtual void initialize()
    {
        qDebug("ERROR: Calling AbstractCoordinatePlane::Private::initialize()");
        // can not call the base class: grid = new AbstractGrid();
    }

    virtual bool isVisiblePoint(
        const AbstractCoordinatePlane *plane,
        const QPointF &point) const
    {
        Q_UNUSED(plane);
        Q_UNUSED(point);
        return true;
    }

    KDChart::Chart *parent = nullptr;
    AbstractGrid *grid = nullptr;
    QRect geometry;
    AbstractDiagramList diagrams;
    GridAttributes gridAttributes;
    AbstractCoordinatePlane *referenceCoordinatePlane = nullptr;

    bool enableCornerSpacers = true;

    bool enableRubberBandZooming = false;
    QRubberBand *rubberBand = nullptr;
    QPoint rubberBandOrigin;

    QStack<ZoomParameters> rubberBandZoomConfigHistory;
};

inline AbstractCoordinatePlane::AbstractCoordinatePlane(Private *p, KDChart::Chart *parent)
    : AbstractArea(p)
{
    if (p)
        p->parent = parent;
    init();
}
inline AbstractCoordinatePlane::Private *AbstractCoordinatePlane::d_func()
{
    return static_cast<Private *>(AbstractArea::d_func());
}
inline const AbstractCoordinatePlane::Private *AbstractCoordinatePlane::d_func() const
{
    return static_cast<const Private *>(AbstractArea::d_func());
}
}

#endif /* KDCHARTABSTRACTCOORDINATEPLANE_P_H*/
