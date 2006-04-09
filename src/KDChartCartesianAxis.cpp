#include <cmath>

#include <QtDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>

#include "KDChartCartesianAxis.h"
#include "KDChartCartesianAxis_p.h"
#include "KDChartAbstractCartesianDiagram.h"

using namespace KDChart;

#define d (d_func())

CartesianAxis::CartesianAxis ( AbstractCartesianDiagram* parent )
    : Axis ( new Private(), parent )
{
    init();
}

CartesianAxis::~CartesianAxis ()
{
}

void CartesianAxis::init ()
{
    d->position = Bottom;
}

void CartesianAxis::setPosition ( Position p )
{
    d->position = p;
}

const CartesianAxis::Position CartesianAxis::position() const
{
    return d->position;
}

#define ptr (context->painter())
void CartesianAxis::paint ( PaintContext* context ) const
{
    Q_ASSERT ( d->diagram() );
    const int MinimumPixelsBetweenRulers = 5;
    const QPair<QPointF, QPointF> dataBoundaries = d->diagram()->dataBoundaries();
    // preparations:
    // - calculate the range that will be displayed:
    double range;
    if ( position() == Bottom || position() == Top )
    {
        range = dataBoundaries.second.x() - dataBoundaries.first.x();
    } else {
        range = dataBoundaries.second.y() - dataBoundaries.first.y();
    }
    double absRange = qAbs ( range );
    // - calculate the decimal magnitude of the range and the basic distance we decorate:
    int magnitude = ( int )log10l ( absRange );
    double basicUnit = powl ( 10, magnitude );
    // - calculate the number of unit, fifth and half measure rulers we will draw:
    int numberOfUnitRulers = ( int ) ( ( absRange + basicUnit * 1.05 ) / basicUnit );
    int numberOfFifthRulers = numberOfUnitRulers * 5 + 1;
    int numberOfHalfRulers = numberOfUnitRulers * 2 + 1;
    // - calculate the absolute range in screen pixels:
    QPointF p1 = context->coordinatePlane()->translate( dataBoundaries.first );
    QPointF p2 = context->coordinatePlane()->translate( dataBoundaries.second );
    double screenRange;
    if ( position() == Bottom || position() == Top )
    {
        screenRange = qAbs ( p1.x() - p2.x() );
    } else {
        screenRange = qAbs ( p1.y() - p2.y() );
    }
    bool drawUnitRulers = screenRange / numberOfUnitRulers > MinimumPixelsBetweenRulers;
    bool drawFifthRulers = screenRange / numberOfFifthRulers > MinimumPixelsBetweenRulers;
    bool drawHalfRulers = screenRange / numberOfHalfRulers > MinimumPixelsBetweenRulers;
    // - find the reference point at which to start drawing and the increment (line distance);
    QPointF rulerRef;
    // FIXME references are of course different for all locations:
    if ( position() == Bottom || position() == Top )
    {
        rulerRef.setX( context->rectangle().top() );
        rulerRef.setY( ( int ) dataBoundaries.first.x() );
    } else {
        rulerRef.setX ( basicUnit * ( int ) dataBoundaries.first.y() );
        rulerRef.setY( context->rectangle().left() );
    }


    // set up the lines to paint:
    // set up a map of integer positions,
    // - starting with the fifth
    // - the the halfs
    // - then the tens
    // this will override all halfs and fifth that hit a higher-order ruler
    // MAKE SURE TO START AT (0, 0)!

    // set up a reference point,  a step vector and a unit vector for the drawing:

#ifdef AXES_PAINTING_DEBUG
    qDebug() << "CartesianAxis::paint: reference values:" << endl
             << "-- range: " << range << endl
             << "-- absRange: " << absRange << endl
             << "-- magnitude: " << magnitude << endl
             << "-- basicUnit: " << basicUnit << endl
             << "-- numberOfUnitRulers: " << numberOfUnitRulers << endl
             << "-- numberOfHalfRulers: " << numberOfHalfRulers << endl
             << "-- numberOfFifthRulers: " << numberOfFifthRulers << endl
             << "-- screenRange: " << screenRange << endl
             << "-- drawUnitRulers: " << drawUnitRulers << endl
             << "-- drawHalfRulers: " << drawHalfRulers << endl
             << "-- drawFifthRulers: " << drawFifthRulers << endl
             << "-- ruler reference point:: " << rulerRef << endl;
#endif

    ptr->setPen ( Qt::black );
    ptr->setBrush ( Qt::red );
    ptr->drawRect ( QRectF( rulerRef, QSizeF( 10, 10 ) ) );
}
#undef ptr

QSize CartesianAxis::sizeHint() const
{   // FIXME return real values:
    QSize result;

    switch ( position() )
    {
    case Bottom:
    case Top:
        result = QSize ( -1, 32 );
        break;
    case Left:
    case Right:
        result = QSize ( 32, -1 );
        break;
    default:
        Q_ASSERT( false ); // all positions need to be handled
        break;
    };

    return result;
}

void CartesianAxis::setGeometry ( const QRectF& rect )
{
    d->geometry = rect;
}

QRectF CartesianAxis::geometry() const
{
    return d->geometry;
}

QDomDocumentFragment CartesianAxis::toXML() const
{
    Q_ASSERT_X ( false, "CartesianAxis::toXML()", "not implemented" );
    return QDomDocumentFragment();
}
