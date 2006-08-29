/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef CARTESIANCOORDINATETRANSFORMATION_H
#define CARTESIANCOORDINATETRANSFORMATION_H

#include <QList>
#include <QRectF>
#include <QPointF>

#include "KDChartZoomParameters.h"

#include <math.h>

namespace KDChart {

    // FIXME: if this struct is used more often, we need to make it a class
    // with proper accessor methods:

    /**
      * \internal
      */
    struct CoordinateTransformation {
        QRectF diagramRect;
        // represents the distance of the diagram coordinate origin to the
        // origin of the coordinate plane space:
        QPointF originTranslation;
        // make a vector base for R2:
        double unitVectorX;
        double unitVectorY;
        // implement isometric scaling:
        double isoScaleX;
        double isoScaleY;

        CartesianCoordinatePlane::AxesCalcMode axesCalcMode;

        ZoomParameters zoom;

        inline const qreal makeLogarithmic( qreal reference, qreal value ) const
        {
            qreal result = value;
            const qreal relation = reference / log10( reference );

            if( result > 0.0 )
                result = log10( result ) * relation;
            else if( result < 0.0 )
                result = -log10( -result ) * relation;

            return result;
        }

        inline const QPointF translate( const QPointF& diagramPoint ) const
        {
            // ### de-inline me
            QPointF result = originTranslation;
            QPointF tempPoint = diagramPoint;
            if ( axesCalcMode == CartesianCoordinatePlane::Logarithmic )
                tempPoint.setY( makeLogarithmic( diagramRect.y(), tempPoint.y() ) );

            //FIXME(khz): Make this work, so we can have logarithmic X axes too.
            // also needed: extend the API, so user can decide if Y and/or X shall be logarithmic.
//            if ( axesCalcMode == CartesianCoordinatePlane::Logarithmic )
//                tempPoint.setX( makeLogarithmic( diagramRect.x(), tempPoint.x() ) );

            tempPoint.setX( tempPoint.x() + diagramRect.width() / (2 * zoom.xFactor) );
            tempPoint.setY( tempPoint.y() + diagramRect.height() / (2 * zoom.yFactor ) );

            tempPoint.setX( tempPoint.x() - diagramRect.width() * zoom.xCenter );
            tempPoint.setY( tempPoint.y() - diagramRect.height() * zoom.yCenter );

            result.setX( result.x() + isoScaleX * unitVectorX * tempPoint.x() );
            result.setY( result.y() + isoScaleY * unitVectorY * tempPoint.y() );

            result.setX( result.x() * zoom.xFactor );
            result.setY( result.y() * zoom.yFactor );

            return result;
        }
    };

    typedef QList<CoordinateTransformation> CoordinateTransformationList;

}

#endif
