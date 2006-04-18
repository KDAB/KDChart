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

#include <math.h>

#include <QFont>
#include <QList>
#include <QtDebug>
#include <QPainter>

#include "KDChartPaintContext.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractPolarDiagram.h"
#include "KDChartPolarCoordinatePlane.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartZoomParameters.h"

using namespace KDChart;

#define DEGTORAD(d) (d)*M_PI/180

static QPointF polarToCartesian( double R, double theta )
{
    return QPointF( R * cos( DEGTORAD( theta  ) ), R * sin( DEGTORAD( theta ) ) );
}


/*struct PolarCoordinatePlane::CoordinateTransformation
{
    // represents the distance of the diagram coordinate origin to the
    // origin of the coordinate plane space:
    QPointF originTranslation;
    double radiusUnit;
    double angleUnit;

    inline const QPointF translate( const QPointF& diagramPoint ) const
    {
      // calculate the polar coordinates
      const double x = diagramPoint.x() * radiusUnit;
      const double y = ( diagramPoint.y() * angleUnit) - 90;
      // convert to cartesian coordinates
      return originTranslation + polarToCartesian( x, y );
    }

    inline const QPointF translatePolar( const QPointF& diagramPoint ) const
    {
        return QPointF( diagramPoint.x() * angleUnit, diagramPoint.y() * radiusUnit );
    }
};*/

struct PolarCoordinatePlane::CoordinateTransformation
{
    // represents the distance of the diagram coordinate origin to the
    // origin of the coordinate plane space:
    QPointF originTranslation;
    double radiusUnit;
    double angleUnit;

    ZoomParameters zoom;

    inline const QPointF translate( const QPointF& diagramPoint ) const
    {
      // calculate the polar coordinates
      const double x = diagramPoint.x() * radiusUnit;
      const double y = ( diagramPoint.y() * angleUnit) - 90;
      // convert to cartesian coordinates
      return originTranslation + polarToCartesian( x, y );
    }

    inline const QPointF translatePolar( const QPointF& diagramPoint ) const
    {
        return QPointF( diagramPoint.x() * angleUnit, diagramPoint.y() * radiusUnit );
    }
};

class PolarCoordinatePlane::CoordinateTransformationList
    : public QList<PolarCoordinatePlane::CoordinateTransformation> {};

class PolarCoordinatePlane::Private
{
public:
    Private()
        :currentTransformation(0),
        initialResizeEventReceived(false )
        {}
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
};


PolarCoordinatePlane::PolarCoordinatePlane ( QWidget* parent )
    : AbstractCoordinatePlane ( parent )
    , p ( new Private() )
{
}

PolarCoordinatePlane::~PolarCoordinatePlane()
{
}

void PolarCoordinatePlane::addDiagram ( AbstractDiagram* diagram )
{
    Q_ASSERT_X ( dynamic_cast<AbstractPolarDiagram*> ( diagram ),
                 "PolarCoordinatePlane::addDiagram", "Only polar"
                 "diagrams can be added to a polar coordinate plane!" );
    AbstractCoordinatePlane::addDiagram ( diagram );
    connect ( diagram,  SIGNAL ( layoutChanged ( AbstractDiagram* ) ),
              SLOT ( slotLayoutChanged ( AbstractDiagram* ) ) );

}

void PolarCoordinatePlane::paintEvent ( QPaintEvent* )
{
    QPainter painter ( this );
    PaintContext ctx;
    ctx.setPainter( &painter );
    ctx.setCoordinatePlane ( this );
    ctx.setRectangle( p->contentRect );

        // paint the diagrams:
    AbstractDiagramList diags = diagrams();
    if ( p->coordinateTransformations.size() == diags.size() )
    {
        for ( int i = 0; i < diags.size(); i++ )
        {
            p->currentTransformation = & ( p->coordinateTransformations[i] );
            PainterSaver painterSaver( &painter );
            // paint the coordinate system rulers:
            paintRulers ( &ctx );
            diags[i]->paint ( &ctx );
        }
        p->currentTransformation = 0;
    } // else: diagrams have not been set up yet
}

void PolarCoordinatePlane::paintRulers( PaintContext* ctx )
{
    if ( p->coordinateTransformations.size () <= 0 ) return;

    // FIXME: we paint the rulers to the settings of the first diagram for now:
    AbstractPolarDiagram* dgr = dynamic_cast<AbstractPolarDiagram*> (diagrams().first() );
    Q_ASSERT ( dgr ); // only polar diagrams are allowed here

    ctx->painter()->setPen ( QColor ( Qt::lightGray ) );
    QPointF origin = translate( QPointF(0,0) );
    const int numberOfSpokes = ( int ) ( 360 / p->currentTransformation->angleUnit );
    const double r = dgr->dataBoundaries().second.y(); // use the full extents
    for ( int i=0; i<numberOfSpokes ; ++i ) {
        ctx->painter()->drawLine( origin, p->currentTransformation->translate( QPointF( r, i ) ) );
    }
    const int numberOfGridRings = ( int ) dgr->numberOfGridRings();
    for ( int i=0; i<numberOfGridRings; ++i ) {
        const double ringRadius = ( (i+1)*r/numberOfGridRings ) * p->currentTransformation->radiusUnit;
        if ( ringRadius == 0 ) continue;
        QRectF rect;
        rect.setTopLeft( p->currentTransformation->originTranslation + QPointF( -ringRadius, -ringRadius ) );
        rect.setBottomRight( p->currentTransformation->originTranslation + QPointF( ringRadius, ringRadius ) );
        ctx->painter()->drawEllipse( rect );
    }
}

void PolarCoordinatePlane::resizeEvent ( QResizeEvent* )
{
    p->initialResizeEventReceived = true;
    layoutDiagrams();
}

void PolarCoordinatePlane::layoutDiagrams()
{
    // the rectangle the diagrams cover in the *plane*:
    // (Why -3? We save 1px on each side for the antialiased drawing, and
    // respect the way QPainter calculates the width of a painted rect (the
    // size is the rectangle size plus the pen width). This way, most clipping
    // for regular pens should be avoided. When pens with a penWidth or larger
    // than 1 are used, this may not b sufficient.
    p->contentRect = QRectF ( 1, 1, width() - 3, height() - 3 );

    // FIXME distribute space according to options:
    p->coordinateTransformations.clear();
    foreach ( AbstractDiagram* diagram, diagrams() )
        {
            AbstractPolarDiagram *polarDiagram = dynamic_cast<AbstractPolarDiagram*>( diagram );
            Q_ASSERT( polarDiagram );
            QPair<QPointF, QPointF> dataBoundariesPair = polarDiagram->dataBoundaries();

            const double angleUnit = 360 / polarDiagram->valueTotals();
            const double radius = dataBoundariesPair.second.y();
            const double diagramWidth = radius * 2; // == height
            const double planeWidth = p->contentRect.width();
            const double planeHeight = p->contentRect.height();
            const double radiusUnit = qMin( planeWidth, planeHeight ) / diagramWidth;
            QPointF coordinateOrigin = QPointF ( planeWidth / 2, planeHeight / 2 );
            coordinateOrigin += p->contentRect.topLeft();

            CoordinateTransformation diagramTransposition = {
                coordinateOrigin,
                radiusUnit,
                angleUnit,
                ZoomParameters()
            };
            p->coordinateTransformations.append( diagramTransposition );
        }
}

const QPointF PolarCoordinatePlane::translate( const QPointF& diagramPoint ) const
{
    Q_ASSERT_X ( p->currentTransformation != 0, "PolarCoordinatePlane::translate",
                 "Only call translate() from within paint()." );
    return  p->currentTransformation->translate ( diagramPoint );
}

const QPointF PolarCoordinatePlane::translatePolar( const QPointF& diagramPoint ) const
{
    Q_ASSERT_X ( p->currentTransformation != 0, "PolarCoordinatePlane::translate",
                 "Only call translate() from within paint()." );
    return  p->currentTransformation->translatePolar ( diagramPoint );
}

void PolarCoordinatePlane::slotLayoutChanged ( AbstractDiagram* )
{
    if ( p->initialResizeEventReceived ) layoutDiagrams();
}

double PolarCoordinatePlane::zoomFactorX() const
{
    return p->currentTransformation->zoom.xFactor;
}

double PolarCoordinatePlane::zoomFactorY() const
{
    return p->currentTransformation->zoom.yFactor;
}

void PolarCoordinatePlane::setZoomFactorX( double factor )
{
    p->currentTransformation->zoom.xFactor = factor;
}

void PolarCoordinatePlane::setZoomFactorY( double factor )
{
    p->currentTransformation->zoom.yFactor = factor;
}

QPointF PolarCoordinatePlane::zoomCenter() const
{
    return QPointF( p->currentTransformation->zoom.xCenter, p->currentTransformation->zoom.yCenter );
}

void PolarCoordinatePlane::setZoomCenter( QPointF center )
{
    p->currentTransformation->zoom.xCenter = center.x();
    p->currentTransformation->zoom.yCenter = center.y();
}
