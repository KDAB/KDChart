/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartLineDiagram.h"
#include "KDChartDataValueAttributes.h"

#include "KDChartLineDiagram_p.h"

using namespace KDChart;
using namespace std;

LineDiagram::Private::Private( const Private& rhs )
    : AbstractCartesianDiagram::Private( rhs )
{
}

void LineDiagram::Private::paintPolyline(
    PaintContext* ctx,
    const QBrush& brush, const QPen& pen,
    const QPolygonF& points ) const
{
    ctx->painter()->setBrush( brush );
    ctx->painter()->setPen( PrintingParameters::scalePen(
        QPen( pen.color(),
              pen.width(),
              pen.style(),
              Qt::FlatCap,
              Qt::MiterJoin ) ) );
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
const QPointF LineDiagram::LineDiagramType::project(
    QPointF point, QPointF maxLimits,
    qreal z, const QModelIndex& index ) const
{
    Q_UNUSED( maxLimits );
    ThreeDLineAttributes td = diagram()->threeDLineAttributes( index );

    //Pending Michel FIXME - the rotation does not work as expected atm
    qreal xrad = DEGTORAD( td.lineXRotation() );
    qreal yrad = DEGTORAD( td.lineYRotation() );
    QPointF ret = QPointF(point.x()*cos( yrad ) + z * sin( yrad ) ,  point.y()*cos( xrad ) - z * sin( xrad ) );
    return ret;
}

void LineDiagram::LineDiagramType::paintThreeDLines(
    PaintContext* ctx, const QModelIndex& index,
    const QPointF& from, const QPointF& to, const qreal depth )
{
    // retrieve the boundaries
    const QPair< QPointF, QPointF > boundaries = diagram()->dataBoundaries();
    const QPointF& maxLimits = boundaries.second;
    const QPointF topLeft = project( from, maxLimits, depth, index );
    const QPointF topRight = project ( to, maxLimits, depth, index );
    const QPolygonF segment = QPolygonF() << from << topLeft << topRight << to;

    ThreeDLineAttributes threeDAttrs = diagram()->threeDLineAttributes( index );
    QBrush indexBrush ( diagram()->brush( index ) );
    indexBrush = threeDAttrs.threeDBrush( indexBrush, QRectF(topLeft, topRight) );

    const PainterSaver painterSaver( ctx->painter() );

    ctx->painter()->setRenderHint( QPainter::Antialiasing, diagram()->antiAliasing() );
    ctx->painter()->setBrush( indexBrush );
    ctx->painter()->setPen( PrintingParameters::scalePen( diagram()->pen( index ) ) );

    reverseMapper().addPolygon( index.row(), index.column(), segment );
    ctx->painter()->drawPolygon( segment );
}

// this method is factored out from LineDiagram::paint, and contains
// the common parts of the method that  previously implemented all
// chart types in one
void LineDiagram::LineDiagramType::paintElements(
    PaintContext* ctx,
    const LabelPaintCache& lpc,
    LineAttributesInfoList& lineList,
    LineAttributes::MissingValuesPolicy policy )
{
    Q_UNUSED( policy );
    // paint all lines and their attributes
    const PainterSaver painterSaver( ctx->painter() );
    ctx->painter()->setRenderHint( QPainter::Antialiasing, diagram()->antiAliasing() );
    LineAttributesInfoListIterator itline ( lineList );

    QBrush curBrush;
    QPen curPen;
    QPolygonF points;
    while ( itline.hasNext() ) {
        const LineAttributesInfo& lineInfo = itline.next();
        const QModelIndex& index = lineInfo.index;
        const ThreeDLineAttributes td = diagram()->threeDLineAttributes( index );
        const ValueTrackerAttributes vt = diagram()->valueTrackerAttributes( index );

        if( td.isEnabled() ){
            paintThreeDLines( ctx, index, lineInfo.value, lineInfo.nextValue, td.depth() );
        } else {
            const QBrush br( diagram()->brush( index ) );
            const QPen pn( diagram()->pen( index ) );
            if( points.count() && points.last() == lineInfo.value && curBrush == br && curPen == pn ) {
                // line goes from last value in points to lineInfo.nextValue
                reverseMapper().addLine( lineInfo.index.row(), lineInfo.index.column(), points.last(), lineInfo.nextValue );
                points << lineInfo.nextValue;
            } else {
                if( points.count() )
                    paintPolyline( ctx, curBrush, curPen, points );
                curBrush = br;
                curPen   = pn;
                points.clear();
                // line goes from lineInfo.value to lineInfo,nextValue
                reverseMapper().addLine( lineInfo.index.row(), lineInfo.index.column(), lineInfo.value, lineInfo.nextValue );
                points << lineInfo.value << lineInfo.nextValue;
            }
        }
    }
    if( points.count() )
        paintPolyline( ctx, curBrush, curPen, points );

    itline.toFront();
    while ( itline.hasNext() ) {
        const LineAttributesInfo& lineInfo = itline.next();
        const QModelIndex& index = lineInfo.index;
        const ValueTrackerAttributes vt = diagram()->valueTrackerAttributes( index );
        if ( vt.isEnabled() ) {
            paintValueTracker( ctx, vt, lineInfo.nextValue );
        }
    }

    // paint all data value texts and the point markers
    paintDataValueTextsAndMarkers( diagram(), ctx, lpc, true );
}

AttributesModel* LineDiagram::LineDiagramType::attributesModel() const
{
    return m_private->attributesModel;
}

QModelIndex LineDiagram::LineDiagramType::attributesModelRootIndex() const
{
    return m_private->diagram->attributesModelRootIndex();
}

int LineDiagram::LineDiagramType::datasetDimension() const
{
    return m_private->datasetDimension;
}

ReverseMapper& LineDiagram::LineDiagramType::reverseMapper()
{
    return m_private->reverseMapper;
}

LineDiagram* LineDiagram::LineDiagramType::diagram() const
{
    return m_private->diagram;
}

void LineDiagram::LineDiagramType::paintAreas(
    PaintContext* ctx,
    const QModelIndex& index, const QList< QPolygonF >& areas,
    const uint transparency )
{
    QPainterPath path;
    for( int i = 0; i < areas.count(); ++i )
    {
        const QPolygonF& p = areas[ i ];
        path.addPolygon( p );
        reverseMapper().addPolygon( index.row(), index.column(), p );
        path.closeSubpath();
    }

    ThreeDLineAttributes threeDAttrs = m_private->diagram->threeDLineAttributes( index );
    QBrush trans = diagram()->brush( index );
    if( threeDAttrs.isEnabled() ) {
        trans = threeDAttrs.threeDBrush( trans, path.boundingRect() );
    }
    QColor transColor = trans.color();
    transColor.setAlpha( transparency );
    trans.setColor(transColor);
    QPen indexPen = diagram()->pen(index);
    indexPen.setBrush( trans );
    const PainterSaver painterSaver( ctx->painter() );

    ctx->painter()->setRenderHint( QPainter::Antialiasing, diagram()->antiAliasing() );
    ctx->painter()->setPen( PrintingParameters::scalePen( indexPen ) );
    ctx->painter()->setBrush( trans );

    ctx->painter()->drawPath( path );
}

qreal LineDiagram::LineDiagramType::valueForCell( int row, int column ) const
{
    return diagram()->valueForCell( row, column );
}

void LineDiagram::LineDiagramType::addLabel( LabelPaintCache* lpc, const AbstractDiagram* diagram,
                                             const QModelIndex & index,
                                             const CartesianDiagramDataCompressor::CachePosition* position,
                                             const PositionPoints& points,
                                             const Position& autoPositionPositive,
                                             const Position& autoPositionNegative, qreal value )
{
    m_private->addLabel( lpc, diagram, index, position, points,
                         autoPositionPositive, autoPositionNegative, value );
}

void LineDiagram::LineDiagramType::paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt, const QPointF& at )
{
    CartesianCoordinatePlane* plane = qobject_cast<CartesianCoordinatePlane*>( ctx->coordinatePlane() );
    if( !plane )
        return;

    DataDimensionsList gridDimensions = ctx->coordinatePlane()->gridDimensionsList();
    const QPointF bottomLeft( ctx->coordinatePlane()->translate(
                              QPointF( plane->isHorizontalRangeReversed() ?
                                           gridDimensions.at( 0 ).end :
                                           gridDimensions.at( 0 ).start,
                                       plane->isVerticalRangeReversed() ?
                                           gridDimensions.at( 1 ).end :
                                           gridDimensions.at( 1 ).start ) ) );
    const QPointF topRight( ctx->coordinatePlane()->translate(
                            QPointF( plane->isHorizontalRangeReversed() ?
                                         gridDimensions.at( 0 ).start :
                                         gridDimensions.at( 0 ).end,
                                     plane->isVerticalRangeReversed() ?
                                         gridDimensions.at( 1 ).start :
                                         gridDimensions.at( 1 ).end ) ) );
    const QPointF markerPoint = at;

    QPointF startPoint;
    if ( vt.orientations() & Qt::Horizontal ) {
        startPoint = QPointF( bottomLeft.x(), at.y() );
    } else {
        startPoint = QPointF( at.x(), topRight.y() );
    }

    QPointF endPoint;
    if ( vt.orientations() & Qt::Vertical ) {
        endPoint = QPointF( at.x(), bottomLeft.y() );
    } else {
        endPoint = QPointF( topRight.x(), at.y() );
    }

    const QSizeF markerSize = vt.markerSize();
    const QRectF ellipseMarker = QRectF( at.x() - markerSize.width() / 2,
                                         at.y() - markerSize.height() / 2,
                                         markerSize.width(), markerSize.height() );

    QPointF startMarker[3];
    if ( vt.orientations() & Qt::Horizontal ) {
        startMarker[0] = startPoint + QPointF( 0,  markerSize.height() / 2 );
        startMarker[1] = startPoint + QPointF( markerSize.width() / 2, 0 );
        startMarker[2] = startPoint - QPointF( 0, markerSize.height() / 2 );
    } else {
        startMarker[0] = startPoint + QPointF( 0, markerSize.height() / 2 );
        startMarker[1] = startPoint + QPointF( markerSize.width() / 2, 0 );
        startMarker[2] = startPoint - QPointF( markerSize.width() / 2, 0 );
    }

    QPointF endMarker[3];

    if ( vt.orientations() & Qt::Vertical ) {
        endMarker[0] = endPoint + QPointF( markerSize.width() / 2, 0 );
        endMarker[1] = endPoint - QPointF( 0, markerSize.height() / 2 );
        endMarker[2] = endPoint - QPointF( markerSize.width() / 2, 0 );
    } else {
        endMarker[0] = endPoint + QPointF( 0,  markerSize.width() / 2 );
        endMarker[1] = endPoint - QPointF( 0, markerSize.height() / 2 );
        endMarker[2] = endPoint - QPointF( markerSize.width() / 2, 0 );
    }

    QPointF topLeft = startPoint;
    QPointF bottomRightOffset = endPoint - topLeft;
    QSizeF size( bottomRightOffset.x(), bottomRightOffset.y() );
    QRectF area( topLeft, size );

    PainterSaver painterSaver( ctx->painter() );
    ctx->painter()->setPen( PrintingParameters::scalePen( vt.linePen() ) );
    ctx->painter()->setBrush( QBrush() );
    ctx->painter()->drawLine( markerPoint, startPoint );
    ctx->painter()->drawLine( markerPoint, endPoint );

    ctx->painter()->fillRect( area, vt.areaBrush() );

    ctx->painter()->setPen( PrintingParameters::scalePen( vt.markerPen() ) );
    ctx->painter()->setBrush( vt.markerBrush() );
    ctx->painter()->drawEllipse( ellipseMarker );

    ctx->painter()->setPen( PrintingParameters::scalePen( vt.arrowBrush().color() ) );
    ctx->painter()->setBrush( vt.arrowBrush() );
    ctx->painter()->drawPolygon( startMarker, 3 );
    ctx->painter()->drawPolygon( endMarker, 3 );
}

CartesianDiagramDataCompressor& LineDiagram::LineDiagramType::compressor() const
{
    return m_private->compressor;
}

qreal LineDiagram::LineDiagramType::interpolateMissingValue( const CartesianDiagramDataCompressor::CachePosition& pos ) const
{
    qreal leftValue = std::numeric_limits< qreal >::quiet_NaN();
    qreal rightValue = std::numeric_limits< qreal >::quiet_NaN();
    int missingCount = 1;

    const int column = pos.second;
    const int row = pos.first;
    const int rowCount = compressor().modelDataRows();

    // iterate back and forth to find valid values
    for( int r1 = row - 1; r1 > 0; --r1 )
    {
        const CartesianDiagramDataCompressor::CachePosition position( r1, column );
        const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
        leftValue = point.value;
        if( !ISNAN( point.value ) )
            break;
        ++missingCount;
    }
    for( int r2 = row + 1; r2 < rowCount; ++r2 )
    {
        const CartesianDiagramDataCompressor::CachePosition position( r2, column );
        const CartesianDiagramDataCompressor::DataPoint point = compressor().data( position );
        rightValue = point.value;
        if( !ISNAN( point.value ) )
            break;
        ++missingCount;
    }
    if( !ISNAN( leftValue ) && !ISNAN( rightValue ) )
        return leftValue + ( rightValue - leftValue ) / ( missingCount + 1 );
    else
        return std::numeric_limits< qreal >::quiet_NaN();
}
