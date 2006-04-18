/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTLINEDIAGRAM_H
#define KDCHARTLINEDIAGRAM_H

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartLineAttributes.h"

class QDomDocumentFragment;
class QPainter;
class QPolygonF;

namespace KDChart {

class KDCHART_EXPORT LineDiagram : public AbstractCartesianDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( LineDiagram )
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( LineDiagram, CartesianCoordinatePlane * )

public:
    LineDiagram( CartesianCoordinatePlane* plane = 0 );
    virtual ~LineDiagram();

    virtual LineDiagram * clone() const;

      enum LineType {
        Normal =  0,
        Stacked = 1,
        Percent = 2
    };


    void setType( const LineType type );
    LineType type() const;

    void setLineAttributes( const LineAttributes & a );
    void setLineAttributes( int column, const LineAttributes & a );
    void setLineAttributes( const QModelIndex & index, const LineAttributes & a );
    LineAttributes lineAttributes(  const QModelIndex & index ) const;

    void setThreeDLineAttributes( const ThreeDLineAttributes & a );
    void setThreeDLineAttributes( int column, const ThreeDLineAttributes & a );
    void setThreeDLineAttributes( const QModelIndex & index,
                                  const ThreeDLineAttributes & a );
    ThreeDLineAttributes threeDLineAttributes( const QModelIndex & index ) const;

    // implement AbstractDiagram
    /* reimpl */
    const QPair<QPointF, QPointF> dataBoundaries() const;

    // implement AbstractCartesianDiagram
    /* reimpl */
    const int numberOfAbscissaSegments () const;
    /* reimpl */
    const int numberOfOrdinateSegments () const;

    void paint ( PaintContext* paintContext );
    void resize ( const QSizeF& area );

protected:
    void paintEvent ( QPaintEvent* );
    void resizeEvent ( QResizeEvent* );

private:
    void paintLines( PaintContext* painter,const QModelIndex& index, double from, double to );
    const QPointF project( QPointF point, QPointF maxLimits, double z, const QModelIndex& index ) const;
    void paintThreeDLines( PaintContext* painter,const QModelIndex& index, double from, double to, const int depth );
    void paintAreas( PaintContext* painter, const QModelIndex& index, const QPolygonF& area, const uint transparency );
}; // End of class KDChartLineDiagram

}

#endif // KDCHARTLINEDIAGRAM_H
