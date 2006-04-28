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


#include <QPainter>
#include <QtXml/QDomDocumentFragment>
#include "KDChartAttributesModel.h"
#include "KDChartPaintContext.h"
#include "KDChartPolarDiagram.h"
#include "KDChartPolarDiagram_p.h"
#include "KDChartPainterSaver_p.h"

using namespace KDChart;

PolarDiagram::Private::Private() :
    zeroDegreePosition( 0 ),
    rotateCircularLabels( false )
{
}

PolarDiagram::Private::~Private() {}

#define d d_func()

PolarDiagram::PolarDiagram( PolarCoordinatePlane* plane ) :
    AbstractPolarDiagram( new Private( ), plane )
{
}

PolarDiagram::~PolarDiagram()
{
}


void PolarDiagram::init()
{
    setShowDelimitersAtPosition( AbstractArea::TopLeft, false );
    setShowDelimitersAtPosition( AbstractArea::Top, true );
    setShowDelimitersAtPosition( AbstractArea::TopRight, false );
    setShowDelimitersAtPosition( AbstractArea::Left, false );
    setShowDelimitersAtPosition( AbstractArea::Right, false );
    setShowDelimitersAtPosition( AbstractArea::BottomLeft, false );
    setShowDelimitersAtPosition( AbstractArea::Bottom, true );
    setShowDelimitersAtPosition( AbstractArea::BottomRight, false );
    setShowLabelsAtPosition( AbstractArea::TopLeft, false );
    setShowLabelsAtPosition( AbstractArea::Top, true );
    setShowLabelsAtPosition( AbstractArea::TopRight, false );
    setShowLabelsAtPosition( AbstractArea::Left, false );
    setShowLabelsAtPosition( AbstractArea::Right, false );
    setShowLabelsAtPosition( AbstractArea::BottomLeft, false );
    setShowLabelsAtPosition( AbstractArea::Bottom, true );
    setShowLabelsAtPosition( AbstractArea::BottomRight, false );
}




PolarDiagram * PolarDiagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: PolarDiagram * PolarDiagram::clone() const" );
    return (PolarDiagram*)0xdeadbeef;
}

QDomDocumentFragment PolarDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment PolarDiagram::toXML() const" );
    return QDomDocumentFragment();
}

const QPair<QPointF, QPointF> PolarDiagram::dataBoundaries () const
{
    if ( !checkInvariants() ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    double xMin = 0.0;
    double xMax = colCount;
    double yMin = 0, yMax = 0;
    for ( int j=0; j<colCount; ++j ) {
        for ( int i=0; i< rowCount; ++i ) {
            double value = model()->data( model()->index( i, j, rootIndex() ) ).toDouble();
            yMax = qMax( yMax, value );
        }
    }
    QPointF bottomLeft ( QPointF( xMin, yMin ) );
    QPointF topRight ( QPointF( xMax, yMax ) );
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}



void PolarDiagram::paintEvent ( QPaintEvent*)
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void PolarDiagram::resizeEvent ( QResizeEvent*)
{
}

void PolarDiagram::paintPolarMarkers( PaintContext* ctx, const QPolygonF& polygon )
{
    const double markerSize = 4; // FIXME use real markers
    for ( int i=0; i<polygon.size(); ++i ) {
        QPointF p = polygon.at( i );
        p.setX( p.x() - markerSize/2 );
        p.setY( p.y() - markerSize/2 );
        ctx->painter()->drawRect( QRectF( p, QSizeF( markerSize, markerSize ) ) );
    }
}

void PolarDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    DataValueTextInfoList list;
    for ( int j=0; j<colCount; ++j ) {
        QBrush brush = qVariantValue<QBrush>( attributesModel()->headerData( j, Qt::Vertical, KDChart::DatasetBrushRole ) );
        QPolygonF polygon;
        for ( int i=0; i<rowCount; ++i ) {
            QModelIndex index = model()->index( i, j, rootIndex() );
            const double value = model()->data( index ).toDouble();
            QPointF point = coordinatePlane()->translate( QPointF( value, i ) );
            polygon.append( point );
            list.append( DataValueTextInfo( index, point, value ) );
        }
        PainterSaver painterSaver( ctx->painter() );
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush );
        QPen p( ctx->painter()->pen() );
        p.setColor( brush.color() ); // FIXME use DatasetPenRole
        p.setWidth( 2 );// FIXME properties
        ctx->painter()->setPen( p );
        ctx->painter()->drawPolyline( polygon );
        paintPolarMarkers( ctx, polygon );
    }
    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
    }
}

void PolarDiagram::resize ( const QSizeF& )
{
}

/*virtual*/
double PolarDiagram::valueTotals () const
{
    return model()->rowCount(rootIndex());
}

/*virtual*/
double PolarDiagram::numberOfValuesPerDataset() const
{
    return model()->rowCount(rootIndex());
}

/*virtual*/
double PolarDiagram::numberOfGridRings() const
{
    return 5; // FIXME
}

void PolarDiagram::setZeroDegreePosition( int degrees )
{
    d->zeroDegreePosition = degrees;
}

int PolarDiagram::zeroDegreePosition() const
{
    return d->zeroDegreePosition;
}

void PolarDiagram::setRotateCircularLabels( bool rotateCircularLabels )
{
    d->rotateCircularLabels = rotateCircularLabels;
}

bool PolarDiagram::rotateCircularLabels() const
{
    return d->rotateCircularLabels;
}

void PolarDiagram::setShowDelimitersAtPosition( AbstractArea::DockingPointType position,
                                                       bool showDelimiters )
{
    d->showDelimitersAtPosition[position] = showDelimiters;
}

void PolarDiagram::setShowLabelsAtPosition( AbstractArea::DockingPointType position,
                                                   bool showLabels )
{
    d->showLabelsAtPosition[position] = showLabels;
}

bool PolarDiagram::showDelimitersAtPosition( AbstractArea::DockingPointType position ) const
{
    return d->showDelimitersAtPosition[position];
}

bool PolarDiagram::showLabelsAtPosition( AbstractArea::DockingPointType position ) const
{
    return d->showLabelsAtPosition[position];
}


























