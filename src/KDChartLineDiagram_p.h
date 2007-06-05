/****************************************************************************
 ** Copyright (C) 2006 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTLINEDIAGRAM_P_H
#define KDCHARTLINEDIAGRAM_P_H

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

#include <QPainterPath.h>

#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartThreeDLineAttributes.h"

#include <KDABLibFakes>


namespace KDChart {

    //class CartesianCoordinatePlane;

/**
 * \internal
 */
    class LineDiagram::Private : public AbstractCartesianDiagram::Private
    {
        friend class LineDiagram;
        friend class LineDiagramType;

    public:
        Private();
        ~Private();

        Private( const Private& rhs )
            : AbstractCartesianDiagram::Private( rhs )
            , lineType( rhs.lineType )
        {
        }

        void paintPolyline( PaintContext* ctx,
                            const QBrush& brush, const QPen& pen,
                            const QPolygonF& points ) const
        {
            ctx->painter()->setBrush( brush );
            ctx->painter()->setPen(
                QPen( pen.color(),
                      pen.width(),
                      pen.style(),
                      Qt::FlatCap,
                      Qt::MiterJoin ) );
#if QT_VERSION > 0x040299
            ctx->painter()->drawPolyline( points );
#else
            // FIXME (Mirko) verify, this sounds reverse-logical
            // For Qt versions older than 4.3 drawPolyline is VERY slow
            // so we use traditional line segments drawing instead then.
            for (int i = 0; i < points.size()-1; ++i)
                ctx->painter()->drawLine( points.at(i), points.at(i+1) );
#endif
        }

/*!
  Projects a point in a space defined by its x, y, and z coordinates
  into a point onto a plane, given two rotation angles around the x
  resp. y axis.
*/
        const QPointF project( LineDiagram* that, QPointF point, QPointF maxLimits,
                               double z, const QModelIndex& index ) const
        {
            ThreeDLineAttributes td = that->threeDLineAttributes( index );

            //Pending Michel FIXME - the rotation does not work as expected atm
            double xrad = DEGTORAD( td.lineXRotation() );
            double yrad = DEGTORAD( td.lineYRotation() );
            QPointF ret = QPointF(point.x()*cos( yrad ) + z * sin( yrad ) ,  point.y()*cos( xrad ) - z * sin( xrad ) );
            return ret;
        }

        void paintThreeDLines( LineDiagram* that, PaintContext* ctx, const QModelIndex& index,
                               const QPointF& from, const QPointF& to, const double depth  )
        {
            // retrieve the boundaries
            const QPair<QPointF, QPointF> boundaries = that->dataBoundaries ();
            QPointF maxLimits = boundaries.second;
            QVector <QPointF > segmentPoints;
            QPointF topLeft = project( that, from, maxLimits, depth, index  );
            QPointF topRight = project ( that, to, maxLimits, depth, index  );

            segmentPoints << from << topLeft << topRight << to;
            QPolygonF segment ( segmentPoints );
            QBrush indexBrush ( that->brush( index ) );
            PainterSaver painterSaver( ctx->painter() );
            if ( that->antiAliasing() )
                ctx->painter()->setRenderHint ( QPainter::Antialiasing );
            ctx->painter()->setBrush( indexBrush );
            ctx->painter()->setPen( that->pen( index ) ) ;
            ctx->painter()->drawPolygon( segment );
        }

// seems to be unused, praise pimpling:
//         void paintAreas( LineDiagram* that, PaintContext* ctx, const QModelIndex& index, const QPolygonF& area, const uint transparency )
//         {
//             QColor trans( that->brush(index).color() );
//             QPen indexPen( that->pen(index) );
//             trans.setAlpha( transparency );
//             indexPen.setColor( trans );
//             PainterSaver painterSaver( ctx->painter() );
//             if ( that->antiAliasing() )
//                 ctx->painter()->setRenderHint ( QPainter::Antialiasing );
//             ctx->painter()->setPen( indexPen );
//             ctx->painter()->setBrush( trans ) ;
//             ctx->painter()->drawPolygon( area );//pol );
//         }

        void paintAreas( LineDiagram* that, PaintContext* ctx, const QModelIndex& index, const QList<QPolygonF>& areas, const uint transparency )
        {
            QColor trans( that->brush(index).color() );
            trans.setAlpha( transparency );
            QPen indexPen( that->pen(index) );
            indexPen.setColor( trans );
            PainterSaver painterSaver( ctx->painter() );
            if ( that->antiAliasing() )
                ctx->painter()->setRenderHint ( QPainter::Antialiasing );
            ctx->painter()->setPen( indexPen );
            ctx->painter()->setBrush( trans );
            QPainterPath path;
            for( int i=0; i<areas.count(); ++i ){
                path.addPolygon( areas[i] );
                path.closeSubpath();
                //qDebug() << "LineDiagram::paintAreas() adding path:"<<areas[i];
            }
            //qDebug() << endl;
            ctx->painter()->drawPath( path );
        }

        LineType lineType;
    };

    KDCHART_IMPL_DERIVED_DIAGRAM( LineDiagram, AbstractCartesianDiagram, CartesianCoordinatePlane );

    class LineDiagram::LineDiagramType : public LineDiagram::Private
    {
    public:
        virtual ~LineDiagramType() {}
        virtual LineDiagram::LineType type() const = 0;
        virtual const QPair<QPointF,  QPointF> calculateDataBoundaries() const = 0;
        virtual void paint(  PaintContext* ctx ) = 0;
    };

/*
  inline LineDiagram::LineDiagram( Private * p, CartesianCoordinatePlane* plane )
  : AbstractCartesianDiagram( p, plane ) { init(); }
  inline LineDiagram::Private * LineDiagram::d_func()
  { return static_cast<Private*>( AbstractCartesianDiagram::d_func() ); }
  inline const LineDiagram::Private * LineDiagram::d_func() const
  { return static_cast<const Private*>( AbstractCartesianDiagram::d_func() ); }
*/

}

#endif /* KDCHARTLINEDIAGRAM_P_H */

