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

#include <KDABLibFakes>


using namespace KDChart;

#define d d_func()

CartesianCoordinatePlane::Private::Private()
    : AbstractCoordinatePlane::Private()
    , hasOwnGridAttributesHorizontal ( false )
    , hasOwnGridAttributesVertical ( false )
    // old: , initialResizeEventReceived ( false )
    , isometricScaling ( false )
    , horizontalMin(0)
    , horizontalMax(0)
    , verticalMin(0)
    , verticalMax(0)
{
    // this bloc left empty intentionally
}


CartesianCoordinatePlane::CartesianCoordinatePlane ( Chart* parent )
    : AbstractCoordinatePlane ( new Private(), parent )
{
    // this bloc left empty intentionally
}

CartesianCoordinatePlane::~CartesianCoordinatePlane()
{
    // this bloc left empty intentionally
}

void CartesianCoordinatePlane::init()
{
    // this bloc left empty intentionally
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

void CartesianCoordinatePlane::paint ( QPainter* painter )
{
    //qDebug("KDChart::CartesianCoordinatePlane::paint() called");
    AbstractDiagramList diags = diagrams();
    //FIXME(khz): make this also work in no diagrams are there
    // (commenting out the following line should do it)
    if ( !diags.isEmpty() )
    {
        PaintContext ctx;
        ctx.setPainter ( painter );
        ctx.setCoordinatePlane ( this );
        const QRectF drawArea( drawingArea() );
        ctx.setRectangle ( drawArea );

        // paint the coordinate system rulers:
        d->grid->drawGrid( &ctx );

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
            PainterSaver painterSaver( painter );
            diags[i]->paint ( &ctx );
        }
        //for debugging:
        //    painter->drawRect( drawArea.adjusted(4,4,4,4) );
        //    painter->drawRect( drawArea.adjusted(2,2,2,2) );
        //    painter->drawRect( drawArea );
    }
    //qDebug("KDChart::CartesianCoordinatePlane::paint() done.");
}

/*
void CartesianCoordinatePlane::paintEvent ( QPaintEvent* )
{
    AbstractDiagramList diags = diagrams();
    if ( !diags.isEmpty() )
    {
        QPainter painter ( this );
        PaintContext ctx;
        ctx.setPainter ( &painter );
        ctx.setCoordinatePlane ( this );
        const QRectF drawArea( drawingArea() );
        ctx.setRectangle ( drawArea );

        // paint the coordinate system rulers:
        d->grid->drawGrid( &ctx );

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
            PainterSaver painterSaver( &painter );
            diags[i]->paint ( &ctx );
        }
    }
    //for debugging: painter.drawRect(drawArea);
}
*/

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

void CartesianCoordinatePlane::slotLayoutChanged ( AbstractDiagram* )
{
    // old: if ( d->initialResizeEventReceived )
    layoutDiagrams();
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
        //qDebug() << "CartesianCoordinatePlane::calculateRawDataBoundingRect() gets diagram->dataBoundaries: " << dataBoundariesPair.first << dataBoundariesPair.second;
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
    const AbstractCartesianDiagram* dgr
        = diagrams().isEmpty() ? 0 : dynamic_cast<const AbstractCartesianDiagram*> (diagrams().first() );

    if( dgr ){
        const QRectF r( calculateRawDataBoundingRect() );
        // note:
        // We do *not* access d->gridAttributesHorizontal here, but
        // we use the getter function, to get the global attrs, if no
        // special ones have been set for the respective orientation.
        const GridAttributes gaH( gridAttributes( Qt::Horizontal ) );
        const GridAttributes gaV( gridAttributes( Qt::Vertical ) );
        // append the first dimension: for Abscissa axes
        l.append(
            DataDimension(
                r.left(), r.right(),
                dgr->datasetDimension() > 1,
                gaH.gridGranularitySequence(),
                gaH.gridStepWidth() ) );
        // append the second dimension: for Ordinate axes
        if( dgr->percentMode() )
            l.append(
                DataDimension(
                    // always return 0-100 when in percentMode
                    0.0, 100.0,
                    true,
                    KDChartEnums::GranularitySequence_10_20,
                    10.0 ) );
        else
            l.append(
                DataDimension(
                    r.bottom(), r.top(),
                    true,
                    gaV.gridGranularitySequence(),
                    gaV.gridStepWidth(),
                    gaV.gridSubStepWidth() ) );
    }else{
        l.append( DataDimension() ); // This gets us the default 1..0 / 1..0 grid
        l.append( DataDimension() ); // shown, if there is no diagram on this plane.
    }
    return l;
}

QRectF CartesianCoordinatePlane::drawingArea() const
{
    const QRect rect( areaGeometry() );
    return QRectF ( rect.left()+1, rect.top()+1, rect.width() - 3, rect.height() - 3 );
}

void CartesianCoordinatePlane::layoutDiagrams()
{
    //qDebug("KDChart::CartesianCoordinatePlane::layoutDiagrams() called");
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
    const QRectF drawArea( drawingArea() );
    //qDebug() << "drawingArea() returns" << drawArea;

    const DataDimensionsList dimensions( gridDimensionsList() );
    // test for programming errors: critical
    Q_ASSERT_X ( dimensions.count() == 2, "CartesianCoordinatePlane::layoutDiagrams",
                 "Error: gridDimensionsList() did not return exactly two dimensions." );
    const DataDimension dimX = dimensions.first();
    const DataDimension dimY = dimensions.last();
    const qreal distX = dimX.distance();
    const qreal distY = dimY.distance();
    const QPointF pt(qMin(dimX.start, dimX.end), qMax(dimY.start, dimY.end));
    const QSizeF siz( qAbs(distX), -qAbs(distY) );
    const QRectF dataBoundingRect( pt, siz );

    // calculate the remaining rectangle, and use it as the diagram area:
    QRectF diagramArea = drawArea;
    diagramArea.setTopLeft ( QPointF ( drawArea.left(), drawArea.top() ) );
    diagramArea.setBottomRight ( QPointF ( drawArea.right(), drawArea.bottom() ) );

    // determine coordinate transformation:
    QPointF diagramTopLeft = dataBoundingRect.topLeft();
    double diagramWidth = dataBoundingRect.width();
    double diagramHeight = dataBoundingRect.height();
    double planeWidth = diagramArea.width();
    double planeHeight = diagramArea.height();
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
    coordinateOrigin += diagramArea.topLeft();
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
    diagramArea.setTopLeft( translate ( dataBoundingRect.topLeft() ) );
    diagramArea.setBottomRight ( translate ( dataBoundingRect.bottomRight() ) );
    //qDebug("KDChart::CartesianCoordinatePlane::layoutDiagrams() done,\ncalling update() now:");
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
        layoutDiagrams();
    }
}

void KDChart::CartesianCoordinatePlane::setVerticalRange( const QPair< qreal, qreal > & range )
{
    if ( d->verticalMin != range.first || d->verticalMax != range.second ) {
        d->verticalMin = range.first;
        d->verticalMax = range.second;
        layoutDiagrams();
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
