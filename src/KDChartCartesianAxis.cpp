#include <cmath>

#include <QtDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>

#include "KDChartCartesianAxis.h"
#include "KDChartCartesianAxis_p.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartPainterSaver_p.h"

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
    //Pending Michel: Probably not needed
    double basicUnit = powl ( 10, magnitude );
    // - calculate the number of unit, fifth and half measure rulers we will draw:
    /* for line and bar charts the number of rulers should depend of the number of
     * line = rows
     * bar = groups
     */
    /*
    int numberOfUnitRulers = ( int ) ( ( absRange + basicUnit * 1.05 ) / basicUnit );
    int numberOfFourthRulers = numberOfUnitRulers * 5 + 1;
    int numberOfHalfRulers = numberOfUnitRulers * 2 + 1;
    */

    // Fixme Michel: Need to find the type of chart here - Line or Bar
    // if Bars calculate the number of groups

    int numberOfUnitRulers;
    if ( position() == Bottom || position() == Top )
        numberOfUnitRulers = d->diagram()->model()->rowCount() - 1;
    else {
        if ( d->diagram()->percentMode() )
            numberOfUnitRulers = 10;
        else
            numberOfUnitRulers = ( int ) absRange;
    }
    int numberOfFourthRulers = numberOfUnitRulers * 4;
    int numberOfHalfRulers = numberOfUnitRulers * 2;

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
    qDebug() << "screenRange" << screenRange << "p1" << p1 << "p2" << p2;
    bool drawUnitRulers = screenRange / numberOfUnitRulers > MinimumPixelsBetweenRulers;
    bool drawFourthRulers = screenRange / numberOfFourthRulers > MinimumPixelsBetweenRulers;
    bool drawHalfRulers = screenRange / numberOfHalfRulers > MinimumPixelsBetweenRulers;
    // - find the reference point at which to start drawing and the increment (line distance);
    QPointF rulerRef;
    QRectF geoRect( d->geometry );
    QRectF rulerRect;
    double rulerWidth;
    double rulerHeight;


    // FIXME references are of course different for all locations:
    rulerWidth = geoRect.width();
    rulerHeight =  geoRect.height();
    if ( position() == Top )
    {
        rulerRef.setX( geoRect.topLeft().x() );
        rulerRef.setY( geoRect.topLeft().y() + rulerHeight );
    } else if ( position() == Bottom ) {
        rulerRef.setX( geoRect.bottomLeft().x() );
        rulerRef.setY( geoRect.bottomLeft().y() - rulerHeight );
    } else if ( position() == Right ) {
        rulerRef.setX( geoRect.bottomRight().x() - rulerWidth );
        rulerRef.setY( geoRect.bottomRight().y() );
    } else {
        rulerRef.setX( geoRect.bottomLeft().x() + rulerWidth );
        rulerRef.setY( geoRect.bottomLeft().y() );
    }

    // set up the lines to paint:

    // set up a map of integer positions,

    // - starting with the fourth
    // - the the halfs
    // - then the tens
    // this will override all halfs and fourth that hit a higher-order ruler
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
             << "-- numberOfFourthRulers: " << numberOfFourthRulers << endl
             << "-- screenRange: " << screenRange << endl
             << "-- drawUnitRulers: " << drawUnitRulers << endl
             << "-- drawHalfRulers: " << drawHalfRulers << endl
             << "-- drawFourthRulers: " << drawFourthRulers << endl
             << "-- ruler reference point:: " << rulerRef << endl;
    #endif

    ptr->setPen ( Qt::black );
    ptr->setBrush ( Qt::red );
    QPointF fourthRulerRef ( rulerRef );
    QPointF halfRulerRef( rulerRef );
    QPointF unitRulerRef( rulerRef );
    /* Pending Michel: FixMe Percent */
    int minValueY = qRound( dataBoundaries.first.y() );
    int minValueX = qRound( dataBoundaries.first.x() );

    double step;
    if ( drawFourthRulers ) {
        //ptr->save();
        //ptr->setPen( Qt::red );
        step = screenRange / numberOfFourthRulers;
        if ( position() == Top || position() == Bottom ) {
            for ( int i = 0; i < numberOfFourthRulers; i++ ) {
                QPointF topPoint ( fourthRulerRef.x(), position() == Top ? fourthRulerRef.y()-1 : fourthRulerRef.y()+1 );
                QPointF bottomPoint ( fourthRulerRef.x(), fourthRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
                fourthRulerRef.setX( fourthRulerRef.x() + step);
            }
        } else {
            for ( int i = 0; i < numberOfFourthRulers; i++ ) {
                QPointF leftPoint ( position() == Left ? fourthRulerRef.x()-1 : fourthRulerRef.x()+1, fourthRulerRef.y() );
                QPointF rightPoint ( fourthRulerRef.x(), fourthRulerRef.y() );
                ptr->drawLine( leftPoint, rightPoint );
                fourthRulerRef.setY( fourthRulerRef.y() - step);
            }
        }
        //ptr->restore();
    }

    if ( drawHalfRulers ) {
        step = screenRange / numberOfHalfRulers;
        if ( position() == Top || position() == Bottom ) {
            for ( int i = 0; i < numberOfHalfRulers; i++ ) {
                QPointF topPoint ( halfRulerRef.x(), position() == Top ? halfRulerRef.y()-2 : halfRulerRef.y()+2 );
                QPointF bottomPoint ( halfRulerRef.x(), halfRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
                halfRulerRef.setX( halfRulerRef.x() + step);
            }
        } else {
            for ( int i = 0; i < numberOfHalfRulers; i++ ) {
                QPointF leftPoint ( position() == Left ? halfRulerRef.x()-2 : halfRulerRef.x()+2, halfRulerRef.y() );
                QPointF rightPoint ( halfRulerRef.x(), halfRulerRef.y() );
                ptr->drawLine( leftPoint, rightPoint );
                halfRulerRef.setY( halfRulerRef.y() - step);
            }
        }
    }
    if ( drawUnitRulers ) {
        step = screenRange / numberOfUnitRulers;
        if ( position() == Top || position() == Bottom ) {
            for ( int i = 0; i <= numberOfUnitRulers; i++ ) {
                QPointF topPoint ( unitRulerRef.x(), position() == Top ? unitRulerRef.y()-3 : unitRulerRef.y()+3 );
                QPointF bottomPoint ( unitRulerRef.x(), unitRulerRef.y() );
                ptr->drawLine( topPoint, bottomPoint );
                //#ifdef VALUES_PAINTING_DEBUG
                if ( drawHalfRulers ) {
                    QFont textFont( QFont("comic", 5 ) );
                    topPoint.setX( topPoint.x() - textFont.pointSize() );
                    topPoint.setY( position() == Top ? topPoint.y()-textFont.pointSize():topPoint.y()+ (2 * textFont.pointSize()) );
                    PainterSaver painterSaver( ptr );
                    ptr->setPen( Qt::blue );
                    ptr->setFont( textFont );
                    ptr->drawText( topPoint, QString::number( minValueX ) );
                    minValueX += 1;
                }
                //#endif
                unitRulerRef.setX( unitRulerRef.x() + step);
            }

        } else {
            for ( int i = 0; i <= numberOfUnitRulers; i++ ) {
                QPointF leftPoint ( position() == Left ? unitRulerRef.x()-3 : unitRulerRef.x()+3, unitRulerRef.y() );
                QPointF rightPoint ( unitRulerRef.x(), unitRulerRef.y() );
                ptr->drawLine( leftPoint, rightPoint );
                //#ifdef VALUES_PAINTING_DEBUG
                if ( drawHalfRulers ) {
                    QFont textFont( QFont("comic", 5 ) );
                    leftPoint.setX( position() == Left ? leftPoint.x()- (2*textFont.pointSize()) : leftPoint.x()+textFont.pointSize() );
                    leftPoint.setY( leftPoint.y() + textFont.pointSize()/2 );
                    PainterSaver painterSaver( ptr );
                    ptr->setPen( Qt::red );
                    ptr->setFont( textFont );
                    ptr->drawText( leftPoint, QString::number( minValueY ) );
                    d->diagram()->percentMode() ? minValueY += 10 : minValueY += 1;
                }
                //#endif
                unitRulerRef.setY( unitRulerRef.y() - step);
            }

        }
    }
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
