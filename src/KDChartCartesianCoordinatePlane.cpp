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

#include <QFont>
#include <QList>
#include <QtDebug>
#include <QPainter>

#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartCartesianCoordinatePlane_p.h"
#include "CartesianCoordinateTransformation.h"
#include "KDChartGridAttributes.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"

using namespace KDChart;

#define d d_func()

CartesianCoordinatePlane::CartesianCoordinatePlane ( QWidget* parent )
    : AbstractCoordinatePlane ( new Private(), parent )
{
    setAxesCalcMode( Linear );
}

CartesianCoordinatePlane::~CartesianCoordinatePlane()
{
}

void CartesianCoordinatePlane::addDiagram ( AbstractDiagram* diagram )
{
    Q_ASSERT_X ( dynamic_cast<AbstractCartesianDiagram*> ( diagram ),
                 "CartesianCoordinatePlane::addDiagram", "Only cartesian "
                 "diagrams can be added to a cartesian coordinate plane!" );
    AbstractCoordinatePlane::addDiagram ( diagram );
    connect ( diagram,  SIGNAL ( layoutChanged ( AbstractDiagram* ) ),
              SLOT ( slotLayoutChanged ( AbstractDiagram* ) ) );
}

void CartesianCoordinatePlane::paintEvent ( QPaintEvent* )
{
    AbstractDiagramList diags = diagrams();
    if ( !diags.isEmpty() )
    {
        QPainter painter ( this );
        PaintContext ctx;
        ctx.setPainter ( &painter );
        ctx.setCoordinatePlane ( this );
        ctx.setRectangle ( d->drawingArea );

        // paint the coordinate system rulers:
        d->grid->drawGrid( &ctx );

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
            PainterSaver painterSaver( &painter );
            diags[i]->paint ( &ctx );
        }
    }
    //for debugging: painter.drawRect(d->drawingArea);
}
/*
void CartesianCoordinatePlane::paintGrid( PaintContext* ctx )
{
    // FIXME accumulate over all diagrams
    const GridAttributes a = gridAttributes();
    AbstractCartesianDiagram* dgr = dynamic_cast<AbstractCartesianDiagram*> (diagrams().first() );
    Q_ASSERT ( dgr ); // only cartesian diagrams are allowed here

    const int numberOfAbscissaSegments = dgr->numberOfAbscissaSegments();
    const int numberOfOrdinateSegments = dgr->numberOfOrdinateSegments();
    QRectF diagramRect = d->coordinateTransformation.diagramRect;
    bool hasVisibleAbscissa = ( d->coordinateTransformation.unitVectorX
                                * ( diagramRect.left() - diagramRect.right() ) ) <= 0;
    bool hasVisibleOrdinate = ( d->coordinateTransformation.unitVectorY
                                * ( diagramRect.top() - diagramRect.bottom () ) ) <= 0;


    // the axes and rulers to draw, this can be cached:
    QVector<QLineF> axes;
    QVector<QLineF> rulers;

    // draw the abscissa and ordinate, if they are in the visible range:
    if ( hasVisibleAbscissa )
    {
        QLineF l2r ( translate ( QPointF ( diagramRect.left(), 0 ) ),
                     translate ( QPointF ( diagramRect.right(), 0 ) ) );
        axes.append ( l2r );
    }

    if ( hasVisibleOrdinate )
    {
        QLineF t2b ( translate ( QPointF ( 0, diagramRect.top() ) ),
                     translate ( QPointF ( 0, diagramRect.bottom() ) ) );
        axes.append ( t2b );
    }

    double step = diagramRect.width() / ( numberOfAbscissaSegments - 1 );
    double x = diagramRect.left();
    for ( int i = 0; i < numberOfAbscissaSegments; ++i )
    {
        QLineF l ( translate ( QPointF ( x, diagramRect.top() ) ),
                   translate ( QPointF ( x,  diagramRect.bottom() ) ) );
        rulers.append (l );
        x += step;
    }

    step = diagramRect.height() / ( numberOfOrdinateSegments - 1 );
    double y = diagramRect.top();
    for ( int i = 0; i < numberOfOrdinateSegments; ++i )
    {
        QLineF l ( translate ( QPointF ( diagramRect.left(), y ) ),
                   translate ( QPointF ( diagramRect.right(), y ) ) );
        rulers.append (l );
        y += step;
    }

    if ( a.isSubGridVisible() ) {
        ctx->painter()->setPen ( a.subGridPen() );
        ctx->painter()->drawLines ( rulers );
    }
    if ( a.isGridVisible() ) {
        ctx->painter()->setPen ( a.gridPen() );
        ctx->painter()->drawLines ( axes );
    }
}
*/
void CartesianCoordinatePlane::resizeEvent ( QResizeEvent* )
{
    d->initialResizeEventReceived = true;
    layoutDiagrams();
}

void CartesianCoordinatePlane::slotLayoutChanged ( AbstractDiagram* )
{
    if ( d->initialResizeEventReceived ) layoutDiagrams();
}

QRectF CartesianCoordinatePlane::calculateRawDataBoundingRect() const
{
    // determine unit of the rectangles of all involved diagrams:
    QPointF smallestPoint;
    QPointF largestPoint;
    bool bStarting = true;
    foreach ( const AbstractDiagram* diagram, diagrams() )
    {
        QPair<QPointF, QPointF> dataBoundariesPair = diagram->dataBoundaries();
        if ( bStarting || dataBoundariesPair.first.x() < smallestPoint.x() )
            smallestPoint.setX( dataBoundariesPair.first.x() );
        if ( bStarting || dataBoundariesPair.first.y() < smallestPoint.y() )
            smallestPoint.setY( dataBoundariesPair.first.y() );
        if ( bStarting || dataBoundariesPair.second.x() > largestPoint.x() )
            largestPoint.setX( dataBoundariesPair.second.x() );
        if ( bStarting || dataBoundariesPair.second.y() > largestPoint.y() )
            largestPoint.setY( dataBoundariesPair.second.y() );
        bStarting = false;
    }

    // if custom boundaries are set on the plane, use them
    if ( d->horizontalMin != d->horizontalMax  ) {
        smallestPoint.setX( d->horizontalMin );
        largestPoint.setX( d->horizontalMax );
    }
    if ( d->verticalMin != d->verticalMax ) {
        smallestPoint.setY( d->verticalMin );
        largestPoint.setY( d->verticalMax );
    }

    QRectF dataBoundingRect;
    dataBoundingRect.setBottomLeft ( smallestPoint );
    dataBoundingRect.setTopRight ( largestPoint );
    //qDebug() << "CartesianCoordinatePlane::calculateRawDataBoundingRect() returns" << dataBoundingRect;
    return dataBoundingRect;
}

DataDimensionsList CartesianCoordinatePlane::getDataDimensionsList() const
{
    DataDimensionsList l;
    const AbstractCartesianDiagram* dgr = dynamic_cast<const AbstractCartesianDiagram*> (diagrams().first() );
    if( dgr ){
        const QRectF r( calculateRawDataBoundingRect() );
        // note:
        // We do *not* access d->gridAttributesHorizontal here, but
        // we use the getter function, to get the global attrs, if no
        // special ones have been set for the respective orientation.
        const GridAttributes gaH( gridAttributes( Qt::Horizontal ) );
        const GridAttributes gaV( gridAttributes( Qt::Vertical ) );
        l.append(
            DataDimension(
                r.left(), r.right(),
                dgr->datasetDimension() > 1,
                gaH.gridGranularitySequence(),
                gaH.gridStepWidth() ) );
        l.append(
            DataDimension(
                r.bottom(), r.top(),
                true,
                gaV.gridGranularitySequence(),
                gaV.gridStepWidth() ) );
    }else{
        l.append( DataDimension() ); // This gets us the default 1..0 / 1..0 grid
        l.append( DataDimension() ); // shown, if there is no diagram on this plane.
    }
    return l;
}

void CartesianCoordinatePlane::layoutDiagrams()
{
    if ( diagrams().isEmpty() )
    {   // FIXME evaluate what can still be prepared
        // FIXME decide default dimension if no diagrams are present (to make empty planes useable)
    }
    // the rectangle the diagrams cover in the *plane*:
    // (Why -3? We save 1px on each side for the antialiased drawing, and
    // respect the way QPainter calculates the width of a painted rect (the
    // size is the rectangle size plus the pen width). This way, most clipping
    // for regular pens should be avoided. When pens with a penWidth or larger
    // than 1 are used, this may not be sufficient.
    d->drawingArea = QRectF ( 1, 1, width() - 3, height() - 3 );

    const DataDimensionsList dimensions( gridDimensionsList() );
    // test for programming errors: critical
    Q_ASSERT_X ( dimensions.count() == 2, "CartesianCoordinatePlane::layoutDiagrams",
                 "Error: gridDimensionsList() did not return exactly two dimensions." );
    const DataDimension dimX = dimensions.first();
    const DataDimension dimY = dimensions.last();
    const QRectF dataBoundingRect(
        QPointF(qMin(dimX.start, dimX.end), qMax(dimY.start, dimY.end)),
        QSizeF( qAbs(dimX.distance()), -qAbs(dimY.distance()) ) );

    // calculate the remaining rectangle, and use it as the diagram area:
    d->diagramArea = d->drawingArea;
    d->diagramArea.setTopLeft ( QPointF ( d->drawingArea.left(), d->drawingArea.top() ) );
    d->diagramArea.setBottomRight ( QPointF ( d->drawingArea.right(), d->drawingArea.bottom() ) );

    // determine coordinate transformation:
    QPointF diagramTopLeft = dataBoundingRect.topLeft();
    double diagramWidth = dataBoundingRect.width();
    double diagramHeight = dataBoundingRect.height();
    double planeWidth = d->diagramArea.width();
    double planeHeight = d->diagramArea.height();
    double scaleX;
    double scaleY;

    double diagramXUnitInCoordinatePlane;
    double diagramYUnitInCoordinatePlane;

    diagramXUnitInCoordinatePlane = diagramWidth != 0 ? planeWidth / diagramWidth : 1;
    diagramYUnitInCoordinatePlane = diagramHeight != 0 ? planeHeight / diagramHeight : 1;
    // calculate diagram origin in plane coordinates:
    QPointF coordinateOrigin = QPointF (
        diagramTopLeft.x() * -diagramXUnitInCoordinatePlane,
        diagramTopLeft.y() * -diagramYUnitInCoordinatePlane );
    coordinateOrigin += d->diagramArea.topLeft();
    // calculate isometric scaling factor to maxscale the diagram into
    // the coordinate system:
    if ( d->isometricScaling )
    {
        double scale = qMin ( qAbs ( diagramXUnitInCoordinatePlane ),
                              qAbs ( diagramYUnitInCoordinatePlane ) );

        scaleX = qAbs( scale / diagramXUnitInCoordinatePlane );
        scaleY = qAbs( scale / diagramYUnitInCoordinatePlane );
    } else {
        scaleX = 1.0;
        scaleY = 1.0;
    }
    d->coordinateTransformation.diagramRect = dataBoundingRect;
    d->coordinateTransformation.originTranslation = coordinateOrigin;
    d->coordinateTransformation.unitVectorX = diagramXUnitInCoordinatePlane;
    d->coordinateTransformation.unitVectorY = diagramYUnitInCoordinatePlane;
    d->coordinateTransformation.isoScaleX = scaleX;
    d->coordinateTransformation.isoScaleY = scaleY;
    //      adapt diagram area to effect of isometric scaling:
    d->diagramArea.setTopLeft( translate ( dataBoundingRect.topLeft() ) );
    d->diagramArea.setBottomRight ( translate ( dataBoundingRect.bottomRight() ) );
    update();
}

const QPointF CartesianCoordinatePlane::translate( const QPointF& diagramPoint ) const
{
    // i had to disable the assertion, because it fails for diagramPoint's
    // directly on the edge of the diagramRect. i guess it can be replaced by
    // manually comparing he coordinates, but be aware of the fact that the
    // user coordinate system may be directed in any way the user choses!
//     Q_ASSERT_X ( d->coordinateTransformation.diagramRect.contains ( diagramPoint ),
//                  "CartesianCoordinatePlane::translate", "Diagram point has to be inside "
//                  "user-supplied diagram rectangle." );
    return  d->coordinateTransformation.translate ( diagramPoint );
}

void CartesianCoordinatePlane::setIsometricScaling ( bool onOff )
{
    if ( d->isometricScaling != onOff )
    {
        d->isometricScaling = onOff;
        layoutDiagrams();
    }
}

bool CartesianCoordinatePlane::doesIsometricScaling ()
{
    return d->isometricScaling;
}

void CartesianCoordinatePlane::setZoomFactorX( double factor )
{
    d->coordinateTransformation.zoom.xFactor = factor;
}

void CartesianCoordinatePlane::setZoomFactorY( double factor )
{
    d->coordinateTransformation.zoom.yFactor = factor;
}

void CartesianCoordinatePlane::setZoomCenter( QPointF center )
{
    d->coordinateTransformation.zoom.xCenter = center.x();
    d->coordinateTransformation.zoom.yCenter = center.y();
}

QPointF CartesianCoordinatePlane::zoomCenter() const
{
    return QPointF( d->coordinateTransformation.zoom.xCenter,
                    d->coordinateTransformation.zoom.yCenter );
}

double CartesianCoordinatePlane::zoomFactorX() const
{
    return d->coordinateTransformation.zoom.xFactor;
}

double CartesianCoordinatePlane::zoomFactorY() const
{
    return d->coordinateTransformation.zoom.yFactor;
}

CartesianCoordinatePlane::AxesCalcMode CartesianCoordinatePlane::axesCalcMode() const
{
    return d->coordinateTransformation.axesCalcMode;
}

void CartesianCoordinatePlane::setAxesCalcMode( AxesCalcMode mode )
{
    d->coordinateTransformation.axesCalcMode = mode;
}

void KDChart::CartesianCoordinatePlane::setHorizontalRange( const QPair< qreal, qreal > & range )
{
    if ( d->horizontalMin != range.first || d->horizontalMax != range.second ) {
        d->horizontalMin = range.first;
        d->horizontalMax = range.second;
	// Is there no better way to do this???
	layoutDiagrams();
	emit rangeChanged();
    }
}

void KDChart::CartesianCoordinatePlane::setVerticalRange( const QPair< qreal, qreal > & range )
{
    if ( d->verticalMin != range.first || d->verticalMax != range.second ) {
        d->verticalMin = range.first;
        d->verticalMax = range.second;
	// Is there no better way to do this???
	layoutDiagrams();
	emit rangeChanged();
    }
}

QPair< qreal, qreal > KDChart::CartesianCoordinatePlane::horizontalRange( ) const
{
    return QPair<qreal, qreal>( d->horizontalMin, d->horizontalMax );
}

QPair< qreal, qreal > KDChart::CartesianCoordinatePlane::verticalRange( ) const
{
    return QPair<qreal, qreal>( d->verticalMin, d->verticalMax );
}

void KDChart::CartesianCoordinatePlane::setGridAttributes(
    Qt::Orientation orientation,
    const GridAttributes& a )
{
    if( orientation == Qt::Horizontal )
        d->gridAttributesHorizontal = a;
    else
        d->gridAttributesVertical = a;
    setHasOwnGridAttributes( orientation, true );
    update();
}

void KDChart::CartesianCoordinatePlane::resetGridAttributes(
    Qt::Orientation orientation )
{
    setHasOwnGridAttributes( orientation, false );
    update();
}

GridAttributes KDChart::CartesianCoordinatePlane::gridAttributes(
    Qt::Orientation orientation ) const
{
    if( hasOwnGridAttributes( orientation ) ){
        if( orientation == Qt::Horizontal )
            return d->gridAttributesHorizontal;
        else
            return d->gridAttributesVertical;
    }else{
        return globalGridAttributes();
    }
}

void KDChart::CartesianCoordinatePlane::setHasOwnGridAttributes(
    Qt::Orientation orientation, bool on )
{
        if( orientation == Qt::Horizontal )
            d->hasOwnGridAttributesHorizontal = on;
        else
            d->hasOwnGridAttributesVertical = on;
}

bool KDChart::CartesianCoordinatePlane::hasOwnGridAttributes(
    Qt::Orientation orientation ) const
{
    return
        ( orientation == Qt::Horizontal )
        ? d->hasOwnGridAttributesHorizontal
        : d->hasOwnGridAttributesVertical;
}

