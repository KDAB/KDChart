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

#include "KDChartPlotter_p.h"
#include "KDChartPlotter.h"

#include "KDChartValueTrackerAttributes.h"
#include "PaintingHelpers_p.h"

using namespace KDChart;

Plotter::Private::Private( const Private& rhs )
    : QObject()
    , AbstractCartesianDiagram::Private( rhs )
    , useCompression( rhs.useCompression )
{
}

void Plotter::Private::init()
{
    AbstractCartesianDiagram::Private::init();
    useCompression = Plotter::NONE;
}

void Plotter::Private::setCompressorResolution(
    const QSizeF& size,
    const AbstractCoordinatePlane* plane )
{
    compressor.setResolution( static_cast<int>( size.width()  * plane->zoomFactorX() ),
                              static_cast<int>( size.height() * plane->zoomFactorY() ) );
}

void Plotter::Private::changedProperties()
{
    if ( CartesianCoordinatePlane* plane = dynamic_cast< CartesianCoordinatePlane* > ( diagram->coordinatePlane() ) )
    {
        QPair< qreal, qreal > verticalRange = plane->verticalRange();
        if ( verticalRange.first != verticalRange.second )
            implementor->plotterCompressor().setForcedDataBoundaries( verticalRange, Qt::Vertical );
        QPair< qreal, qreal > horizontalRange = plane->horizontalRange();
        if ( verticalRange.first != horizontalRange.second )
            implementor->plotterCompressor().setForcedDataBoundaries( horizontalRange, Qt::Horizontal );
    }
}

/*!
  Projects a point in a space defined by its x, y, and z coordinates
  into a point onto a plane, given two rotation angles around the x
  resp. y axis.
*/
const QPointF Plotter::PlotterType::project(
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

void Plotter::PlotterType::paintThreeDLines(
    PaintContext* ctx, const QModelIndex& index,
    const QPointF& from, const QPointF& to, const qreal depth )
{
    // retrieve the boundaries
    const QPair< QPointF, QPointF > boundaries = diagram()->dataBoundaries();
    const QPointF& maxLimits = boundaries.second;
    const QPointF topLeft = project( from, maxLimits, depth, index );
    const QPointF topRight = project ( to, maxLimits, depth, index );

    const QPolygonF segment = QPolygonF() << from << topLeft << topRight << to;
    const QBrush indexBrush ( diagram()->brush( index ) );
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
void Plotter::PlotterType::paintElements(
    PaintContext* ctx,
    const LabelPaintCache& lpc,
    LineAttributesInfoList& lineList,
    LineAttributes::MissingValuesPolicy policy )
{
    Q_UNUSED( policy );
    // paint all lines and their attributes
    PainterSaver painterSaver( ctx->painter() );
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
                if ( points.count() ) {
                    PaintingHelpers::paintPolyline( ctx, curBrush, curPen, points );
                }
                curBrush = br;
                curPen   = pn;
                points.clear();
                // line goes from lineInfo.value to lineInfo,nextValue
                reverseMapper().addLine( lineInfo.index.row(), lineInfo.index.column(), lineInfo.value, lineInfo.nextValue );
                points << lineInfo.value << lineInfo.nextValue;
            }
        }
    }
    if ( points.count() ) {
        PaintingHelpers::paintPolyline( ctx, curBrush, curPen, points );
    }
    itline.toFront();
    while ( itline.hasNext() ) {
        const LineAttributesInfo& lineInfo = itline.next();
        const QModelIndex& index = lineInfo.index;
        const ValueTrackerAttributes vt = diagram()->valueTrackerAttributes( index );
        if ( vt.isEnabled() ) {
            PaintingHelpers::paintValueTracker( ctx, vt, lineInfo.nextValue );
        }
    }

    // paint all data value texts and the point markers
    m_private->paintDataValueTextsAndMarkers( diagram(), ctx, lpc, true );
}

AttributesModel* Plotter::PlotterType::attributesModel() const
{
    return m_private->attributesModel;
}

ReverseMapper& Plotter::PlotterType::reverseMapper()
{
    return m_private->reverseMapper;
}

Plotter* Plotter::PlotterType::diagram() const
{
    return m_private->diagram;
}

void Plotter::PlotterType::paintAreas(
    PaintContext* ctx,
    const QModelIndex& index, const QList< QPolygonF >& areas,
    uint opacity )
{
    QColor trans = diagram()->brush( index ).color();
    trans.setAlpha( opacity );
    QPen indexPen = diagram()->pen(index);
    indexPen.setColor( trans );
    const PainterSaver painterSaver( ctx->painter() );

    ctx->painter()->setRenderHint( QPainter::Antialiasing, diagram()->antiAliasing() );
    ctx->painter()->setPen( PrintingParameters::scalePen( indexPen ) );
    ctx->painter()->setBrush( trans );

    QPainterPath path;
    for( int i = 0; i < areas.count(); ++i )
    {
        const QPolygonF& p = areas[ i ];
        path.addPolygon( p );
        reverseMapper().addPolygon( index.row(), index.column(), p );
        path.closeSubpath();
    }
    ctx->painter()->drawPath( path );
}

void Plotter::PlotterType::addLabel( LabelPaintCache *lpc, const AbstractDiagram* diagram,
                                     const QModelIndex& index, const PositionPoints& points,
                                     const Position& autoPositionPositive,
                                     const Position& autoPositionNegative, qreal value )
{
    m_private->addLabel( lpc, diagram, index, 0, points, autoPositionPositive, autoPositionNegative, value );
}

CartesianDiagramDataCompressor& Plotter::PlotterType::compressor() const
{
    return m_private->compressor;
}

PlotterDiagramCompressor& Plotter::PlotterType::plotterCompressor() const
{
    return m_private->plotterCompressor;
}

Plotter::CompressionMode Plotter::PlotterType::useCompression() const
{
    return m_private->useCompression;
}

void Plotter::PlotterType::setUseCompression( Plotter::CompressionMode value )
{
    m_private->useCompression = value;
}
