/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTPOLARCOORDINATEPLANE_P_H
#define KDCHARTPOLARCOORDINATEPLANE_P_H

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

#include "KDChartAbstractCoordinatePlane_p.h"
#include "KDChartPolarGrid.h"
#include "KDChartZoomParameters.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
struct PolarCoordinatePlane::CoordinateTransformation
{
    // represents the distance of the diagram coordinate origin to the
    // origin of the coordinate plane space:
    QPointF originTranslation;
    qreal radiusUnit;
    qreal angleUnit;
    qreal minValue;

    qreal startPosition;
    ZoomParameters zoom;

    static QPointF polarToCartesian(qreal R, qreal theta)
    {
        // de-inline me
        return QPointF(R * cos(DEGTORAD(theta)), R * sin(DEGTORAD(theta)));
    }

    inline const QPointF translate(const QPointF &diagramPoint) const
    {
        // ### de-inline me
        // calculate the polar coordinates
        const qreal x = (diagramPoint.x() * radiusUnit) - (minValue * radiusUnit);
        // qDebug() << x << "=" << diagramPoint.x() << "*" << radiusUnit << "  startPosition: " << startPosition;
        const qreal y = (diagramPoint.y() * -angleUnit) - 90.0 - startPosition;
        // convert to cartesian coordinates
        QPointF cartesianPoint = polarToCartesian(x, y);
        cartesianPoint.setX(cartesianPoint.x() * zoom.xFactor);
        cartesianPoint.setY(cartesianPoint.y() * zoom.yFactor);

        QPointF newOrigin = originTranslation;
        qreal minOrigin = qMin(newOrigin.x(), newOrigin.y());
        newOrigin.setX(newOrigin.x() + minOrigin * (1 - zoom.xCenter * 2) * zoom.xFactor);
        newOrigin.setY(newOrigin.y() + minOrigin * (1 - zoom.yCenter * 2) * zoom.yFactor);

        return newOrigin + cartesianPoint;
    }

    inline const QPointF translatePolar(const QPointF &diagramPoint) const
    {
        // ### de-inline me
        return QPointF(diagramPoint.x() * angleUnit, diagramPoint.y() * radiusUnit);
    }
};

class PolarCoordinatePlane::Private : public AbstractCoordinatePlane::Private
{
    friend class PolarCoordinatePlane;

public:
    explicit Private()
    {
    }

    ~Private() override
    {
    }

    void initialize() override
    {
        grid = new PolarGrid();
    }

    static QRectF contentsRect(const PolarCoordinatePlane *plane);

    // the coordinate plane will calculate coordinate transformations for all
    // diagrams and store them here:
    CoordinateTransformationList coordinateTransformations;
    // when painting, this pointer selects the coordinate transformation for
    // the current diagram:
    CoordinateTransformation *currentTransformation = nullptr;
    // the reactangle occupied by the diagrams, in plane coordinates
    QRectF contentRect;
    // true after the first resize event came in
    bool initialResizeEventReceived = false;

    // true after setGridAttributes( Qt::Orientation ) was used,
    // false if resetGridAttributes( Qt::Orientation ) was called
    bool hasOwnGridAttributesCircular = false;
    bool hasOwnGridAttributesSagittal = false;

    GridAttributes gridAttributesCircular;
    GridAttributes gridAttributesSagittal;

    qreal newZoomX = 0.0;
    qreal newZoomY = 0.0;
};

KDCHART_IMPL_DERIVED_PLANE(PolarCoordinatePlane, AbstractCoordinatePlane)
}

#endif /* KDCHARTBARDIAGRAM_P_H */
