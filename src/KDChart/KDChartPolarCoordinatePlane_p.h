/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

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
#include "KDChartZoomParameters.h"
#include "KDChartPolarGrid.h"

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
    double radiusUnit;
    double angleUnit;
    double minValue;

    qreal startPosition;
    ZoomParameters zoom;

    static QPointF polarToCartesian( double R, double theta )
    {
        // de-inline me
        return QPointF( R * cos( DEGTORAD( theta  ) ), R * sin( DEGTORAD( theta ) ) );
    }

    inline const QPointF translate( const QPointF& diagramPoint ) const
    {
        // ### de-inline me
        // calculate the polar coordinates
        const double x = (diagramPoint.x() * radiusUnit) - (minValue * radiusUnit);
//qDebug() << x << "=" << diagramPoint.x() << "*" << radiusUnit << "  startPosition: " << startPosition;
        const double y = ( diagramPoint.y() * -angleUnit) - 90.0 - startPosition;
        // convert to cartesian coordinates
        QPointF cartesianPoint = polarToCartesian( x, y );
        cartesianPoint.setX( cartesianPoint.x() * zoom.xFactor );
        cartesianPoint.setY( cartesianPoint.y() * zoom.yFactor );

        QPointF newOrigin = originTranslation;
        double minOrigin = qMin( newOrigin.x(), newOrigin.y() );
        newOrigin.setX( newOrigin.x() + minOrigin * ( 1 - zoom.xCenter * 2 ) * zoom.xFactor );
        newOrigin.setY( newOrigin.y() + minOrigin * ( 1 - zoom.yCenter * 2 ) * zoom.yFactor );

        return newOrigin + cartesianPoint;
    }

    inline const QPointF translatePolar( const QPointF& diagramPoint ) const
    {
        // ### de-inline me
        return QPointF( diagramPoint.x() * angleUnit, diagramPoint.y() * radiusUnit );
    }
};

class PolarCoordinatePlane::Private : public AbstractCoordinatePlane::Private
{
    friend class PolarCoordinatePlane;
public:
    explicit Private()
        : currentTransformation(0)
        , initialResizeEventReceived(false )
        , hasOwnGridAttributesCircular ( false )
        , hasOwnGridAttributesSagittal ( false )
    {}

    virtual ~Private() { }

    virtual void initialize()
    {
        grid = new PolarGrid();
    }

    // the coordinate plane will calculate coordinate transformations for all
    // diagrams and store them here:
    CoordinateTransformationList coordinateTransformations;
    // when painting, this pointer selects the coordinate transformation for
    // the current diagram:
    CoordinateTransformation* currentTransformation;
    // the reactangle occupied by the diagrams, in plane coordinates
    QRectF contentRect;
    // true after the first resize event came in
    bool initialResizeEventReceived;

    // true after setGridAttributes( Qt::Orientation ) was used,
    // false if resetGridAttributes( Qt::Orientation ) was called
    bool hasOwnGridAttributesCircular;
    bool hasOwnGridAttributesSagittal;

    GridAttributes gridAttributesCircular;
    GridAttributes gridAttributesSagittal;

    qreal newZoomX, newZoomY;
};


KDCHART_IMPL_DERIVED_PLANE(PolarCoordinatePlane, AbstractCoordinatePlane)

}

#endif /* KDCHARTBARDIAGRAM_P_H */
