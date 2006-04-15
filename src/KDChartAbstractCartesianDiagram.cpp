#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartPaintContext.h"
#include <QDebug>
#include <QPainter>

using namespace KDChart;
AbstractCartesianDiagram::Private::Private()
    : referenceDiagram( 0 )
{
}

AbstractCartesianDiagram::Private::~Private() 
{
 
}

void AbstractCartesianDiagram::init()
{
}

#define d d_func()

AbstractCartesianDiagram::AbstractCartesianDiagram ( CartesianCoordinatePlane* parent )
    : AbstractDiagram ( new Private(), parent )
{
}

void AbstractCartesianDiagram::addAxes( CartesianAxis *axis )
{
    d->axesList.append( axis );
}

KDChart::CartesianAxisList AbstractCartesianDiagram::axes( ) const 
{
    return d->axesList;
}

void AbstractCartesianDiagram::setReferenceDiagram( AbstractCartesianDiagram* diagram, const QPointF& offset )
{
    d->referenceDiagram = diagram;
    d->referenceDiagramOffset = offset;
}

AbstractCartesianDiagram* AbstractCartesianDiagram::referenceDiagram() const
{
    return d->referenceDiagram;
}

QPointF AbstractCartesianDiagram::referenceDiagramOffset() const
{
    return d->referenceDiagramOffset;
}

GridAttributes* AbstractCartesianDiagram::gridAttributes()
{
    return &(d->gridAttributes);
}

const GridAttributes* AbstractCartesianDiagram::gridAttributes() const
{
    return &(d->gridAttributes);
}

void AbstractCartesianDiagram::drawGrid( PaintContext* context )
{
    if ( !gridAttributes()->isGridVisible() )
        return;

    const int MinimumPixelsBetweenLines = 10;
    
    QPointF p1 = context->coordinatePlane()->translate( dataBoundaries().first );
    QPointF p2 = context->coordinatePlane()->translate( dataBoundaries().second );

    double screenRangeX = qAbs ( p1.x() - p2.x() );
    double screenRangeY = qAbs ( p1.y() - p2.y() );

    int numberOfUnitLinesX = model()->rowCount() - 1;
    int numberOfUnitLinesY = (int) (dataBoundaries().second.y() - dataBoundaries().first.y());
    
    bool drawUnitLinesX = screenRangeX / numberOfUnitLinesX > MinimumPixelsBetweenLines;
    bool drawUnitLinesY = screenRangeY / numberOfUnitLinesY > MinimumPixelsBetweenLines;
    int unitFactorX = 1;
    int unitFactorY = 1;

    while ( !drawUnitLinesX ) {
        unitFactorX *= 10;
        drawUnitLinesX = screenRangeX / (numberOfUnitLinesX / unitFactorX) > MinimumPixelsBetweenLines;
    }
    while ( !drawUnitLinesY ) {
        unitFactorY *= 10;
        drawUnitLinesY = screenRangeY / (numberOfUnitLinesY / unitFactorY) > MinimumPixelsBetweenLines;
    }

    bool drawHalfLinesX = screenRangeX / (numberOfUnitLinesX * 2) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawHalfLinesY = screenRangeY / (numberOfUnitLinesY * 2) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawFourthLinesX = screenRangeX / (numberOfUnitLinesX * 4) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();
    bool drawFourthLinesY = screenRangeY / (numberOfUnitLinesY * 4) > MinimumPixelsBetweenLines && gridAttributes()->isSubGridVisible();

    int minValueX = qRound( dataBoundaries().first.x() );
    int maxValueX = qRound( dataBoundaries().second.x() );
    int minValueY = qRound( dataBoundaries().first.y() );
    int maxValueY = qRound( dataBoundaries().second.y() );

    if ( drawFourthLinesX ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueX; f <= maxValueX; f += 0.25 ) {
            QPointF topPoint( f, maxValueY );
	    QPointF bottomPoint( f, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawFourthLinesY ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueY; f <= maxValueY; f += 0.25 ) {
            QPointF leftPoint( minValueX, f );
	    QPointF rightPoint( maxValueX, f );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    if ( drawHalfLinesX ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueX; f <= maxValueX; f += 0.5 ) {
            QPointF topPoint( f, maxValueY );
	    QPointF bottomPoint( f, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawHalfLinesY ) {
        context->painter()->setPen( gridAttributes()->subGridPen() );
        for ( float f = minValueY; f <= maxValueY; f += 0.5 ) {
            QPointF leftPoint( minValueX, f );
	    QPointF rightPoint( maxValueX, f );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    if ( drawUnitLinesX ) {
        context->painter()->setPen( gridAttributes()->gridPen() );
        int minX = minValueX;
        if ( minX % unitFactorX != 0 )
            minX = minX + unitFactorX * (minX > 0 ? 1 : -1) - minX % unitFactorX;
        for ( int i = minX; i <= maxValueX; i += unitFactorX ) {
            QPointF topPoint( i, maxValueY );
	    QPointF bottomPoint( i, minValueY );
            topPoint = context->coordinatePlane()->translate( topPoint );
            bottomPoint = context->coordinatePlane()->translate( bottomPoint );
            context->painter()->drawLine( topPoint, bottomPoint );
        }
    }
    if ( drawUnitLinesY ) {
        context->painter()->setPen( gridAttributes()->gridPen() );
        int minY = minValueY;
        if ( minY % unitFactorY != 0 )
            minY = minY + unitFactorY * (minY > 0 ? 1 : -1) - minY % unitFactorY;
        for ( int i = minY; i <= maxValueY; i += unitFactorY ) {
            QPointF leftPoint( minValueX, i );
	    QPointF rightPoint( maxValueX, i );
            leftPoint = context->coordinatePlane()->translate( leftPoint );
            rightPoint = context->coordinatePlane()->translate( rightPoint );
            context->painter()->drawLine( leftPoint, rightPoint );
        }
    }

    context->painter()->setPen( gridAttributes()->zeroLinePen() );
    QPointF topPoint( 0, maxValueY );
    QPointF bottomPoint( 0, minValueY );
    QPointF leftPoint( minValueX, 0 );
    QPointF rightPoint( maxValueX, 0 );
    topPoint = context->coordinatePlane()->translate( topPoint );
    bottomPoint = context->coordinatePlane()->translate( bottomPoint );
    leftPoint = context->coordinatePlane()->translate( leftPoint );
    rightPoint = context->coordinatePlane()->translate( rightPoint );
    context->painter()->drawLine( topPoint, bottomPoint );
    context->painter()->drawLine( leftPoint, rightPoint );
}
