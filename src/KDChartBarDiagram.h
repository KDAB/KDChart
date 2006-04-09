/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart licenses may use this file in
 ** accordance with the KDChart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTBARDIAGRAM_H
#define KDCHARTBARDIAGRAM_H

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartBarAttributes.h"
#include "KDChartThreeDBarAttributes.h"

class QDomDocumentFragment;
class QPainter;

namespace KDChart {

class KDCHART_EXPORT BarDiagram : public AbstractCartesianDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( BarDiagram )
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( BarDiagram, CartesianCoordinatePlane * )
public:
    BarDiagram( CartesianCoordinatePlane* plane = 0 );
    virtual ~BarDiagram();

    virtual BarDiagram * clone() const;

    void setBarAttributes( const BarAttributes & a );
    void setBarAttributes( int column, const BarAttributes & a );
    void setBarAttributes( const QModelIndex & index,
				const BarAttributes & a );
    BarAttributes barAttributes( const QModelIndex & index ) const;

    void setThreeDBarAttributes( const ThreeDBarAttributes & a );
    void setThreeDBarAttributes( int column, const ThreeDBarAttributes & a );
    void setThreeDBarAttributes( const QModelIndex & index,
                                  const ThreeDBarAttributes & a );
    ThreeDBarAttributes threeDBarAttributes( const QModelIndex & index ) const;    

    // implement AbstractDiagram
    /** \reimpl */
    const QPair<QPointF, QPointF> dataBoundaries() const;
    // implement AbstractCartesianDiagram
    /** \reimpl */
    const int numberOfAbscissaSegments () const;
    /** \reimpl */
    const int numberOfOrdinateSegments () const;
    /** \reimpl */
    CartesianAxisList axes () const;

    void paint ( PaintContext* paintContext );
    void resize ( const QSizeF& area );

#if 0
    // FIXME merge with 3DAttributes?
    void setThreeDimensionalBars( bool threeDBars );
    bool threeDimensionalBars() const;

    void setThreeDimensionalBarsShadowColors( bool shadow );
    bool threeDimensionalBarsShadowColors() const;

    void setThreeDimensionalBarAngle( uint angle );
    uint threeDimensionalBarAngle() const;

    void setThreeDimensionalBarDepth( double depth );
    double threeDimensionalBarDepth() const;

#endif


protected:
    void paintEvent ( QPaintEvent* );
    void resizeEvent ( QResizeEvent* );

private:
    void paintBars( PaintContext* ctx, const QModelIndex& index, const QRectF& bar );
    void calculateValueAndGapWidths( int rowCount, int colCount, 
                                     double groupWidth,
                                     double& barWidth,
                                     double& spaceBetweenBars,
                                     double& spaceBetweenGroups );
}; // End of class BarDiagram

}

#endif // KDCHARTBARDIAGRAM_H
