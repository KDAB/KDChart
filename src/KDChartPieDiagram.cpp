/****************************************************************************
 ** Copyright (C) 2006 Klar�vdalens Datakonsult AB.  All rights reserved.
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
#include <QDebug>
#include <QtXml/QDomDocumentFragment>

#include "KDChartAttributesModel.h"
#include "KDChartPaintContext.h"
#include "KDChartPieDiagram.h"
#include "KDChartPieDiagram_p.h"
#include "KDChartPainterSaver_p.h"

//FIXME(khz): use an internal libfakes library instead of this internal haeder file
#include "kdchart_platformdef.h"


using namespace KDChart;

PieDiagram::Private::Private()
{
}

PieDiagram::Private::~Private() {}

#define d d_func()

PieDiagram::PieDiagram( QWidget* parent, PolarCoordinatePlane* plane ) :
    AbstractPieDiagram( new Private(), parent, plane )
{
    init();
}

PieDiagram::~PieDiagram()
{
}

void PieDiagram::init()
{
}

PieDiagram * PieDiagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: PieDiagram * PieDiagram::clone() const" );
    return (PieDiagram*)0xdeadbeef;
}

QDomDocumentFragment PieDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment PieDiagram::toXML() const" );
    return QDomDocumentFragment();
}

const QPair<QPointF, QPointF> PieDiagram::calculateDataBoundaries () const
{
    QPointF bottomLeft ( QPointF( 0, 0 ) );
    QPointF topRight;
    // If we explode, we need extra space for the pie slice that has
    // the largest explosion distance.
    if( explode() ) {
        double maxExplode = 0.0;
        for( int j = 0; j < model()->columnCount(rootIndex()); j++ )
            maxExplode = qMax( maxExplode, explodeFactor( j ) );
        topRight = QPointF( 1.0+maxExplode, 1.0+maxExplode );
    } else
        topRight = QPointF( 1.0, 1.0 );
    return QPair<QPointF, QPointF> ( bottomLeft,  topRight );
}

void PieDiagram::paintEvent( QPaintEvent* )
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

void PieDiagram::resizeEvent ( QResizeEvent*)
{
}

static QRectF buildReferenceRect( const PolarCoordinatePlane* plane )
{
    QRectF contentsRect;
    QPointF referencePointAtTop = plane->translate( QPointF( 1, 0 ) );
    QPointF temp = plane->translate( QPointF( 0, 0 ) ) - referencePointAtTop;
    const double offset = temp.y();
    referencePointAtTop.setX( referencePointAtTop.x() - offset );
    contentsRect.setTopLeft( referencePointAtTop );
    contentsRect.setBottomRight( referencePointAtTop + QPointF( 2*offset, 2*offset) );
    return contentsRect;
}

void PieDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants() ) return;
    const int colCount = model()->columnCount(rootIndex());
    QRectF contentsRect = buildReferenceRect( polarCoordinatePlane() );
    DataValueTextInfoList list;
    double startAngle = startPosition();
    double startAngleValueSpace = valueTotals() / 360 * startAngle;
    for ( int j=0; j<colCount; ++j ) {
      const double nextValue = qAbs( model()->data( model()->index( 0, j,rootIndex() ) ).toDouble() );
        double spanAngle = polarCoordinatePlane()->translatePolar( QPointF( nextValue, 1 ) ).x();
        if ( spanAngle == 0 ) continue;
        QBrush brush = qVariantValue<QBrush>( attributesModel()->headerData( j, Qt::Vertical, KDChart::DatasetBrushRole ) );
        QPen pen = qVariantValue<QPen>( attributesModel()->headerData( j, Qt::Vertical, KDChart::DatasetPenRole ) );
        PainterSaver painterSaver( ctx->painter() );
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( brush );
        ctx->painter()->setPen( pen );

        // Explosion support
        QRectF pieRect = contentsRect;
        if( explode() ) {
            QPointF oldCenter = contentsRect.center();
            QPointF newCenter = polarCoordinatePlane()->translate( QPointF( explodeFactor( j ),
                                                                            startAngleValueSpace + nextValue/2.0 ) );
            QPointF difference = newCenter - oldCenter;
            pieRect.translate( difference );
        }

        ctx->painter()->drawPie( pieRect, ( int ) ((-startAngle + 90 ) * 16), ( int ) (-spanAngle * 16) );
        startAngle += spanAngle;
        startAngleValueSpace += nextValue;
    }
    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
    }
}

void PieDiagram::resize ( const QSizeF& )
{
}

/*virtual*/
double PieDiagram::valueTotals () const
{
    double total = 0;
    const int colCount = model()->columnCount(rootIndex());
    for ( int j=0; j<colCount; ++j ) {
      total += model()->data( model()->index( 0, j, rootIndex() ) ).toDouble();
    }
    return total;
}

/*virtual*/
double PieDiagram::numberOfValuesPerDataset() const
{
    return model()->columnCount();
}

/*virtual*/
double PieDiagram::numberOfGridRings() const
{
    return 1;
}



QPointF PieDiagram::pointOnCircle( const QRectF& rect, qreal angle )
{
    qreal angleRad = DEGTORAD( angle );
    qreal cosAngle = cos( angleRad );
    qreal sinAngle = -sin( angleRad );
    qreal posX = cosAngle * rect.width() / 2.0;
    qreal posY = sinAngle * rect.height() / 2.0;
    return QPointF( posX + rect.center().x(),
                   posY + rect.center().y() );

}




