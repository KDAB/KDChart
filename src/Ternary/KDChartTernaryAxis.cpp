#include <QPainter>

#include <KDChartChart>
#include <KDChartPaintContext>

#include "KDChartTernaryAxis.h"
#include "../src/KDChartLayoutItems.h"

using namespace KDChart;

TernaryAxis::TernaryAxis ( AbstractTernaryDiagram* diagram)
    : AbstractAxis( diagram )
{
    resetTitleTextAttributes();
}

TernaryAxis::~TernaryAxis()
{
    // empty
}

void  TernaryAxis::paintAll (QPainter &)
{
    // not used
}

void  TernaryAxis::paint (QPainter *)
{
    // not used
}

void  TernaryAxis::paintCtx (PaintContext * paintContext)
{
    QPainter* p = paintContext->painter();
    TernaryCoordinatePlane* plane =
        (TernaryCoordinatePlane*) paintContext->coordinatePlane();
    // QObject* refArea = plane->parent();
    QRectF drawArea = paintContext->rectangle();
    QRectF titleArea;

    TextAttributes ta( textAttributes() );
    p->setFont( ta.font() );
    p->setPen( ta.pen() );

//    // todo: should be a variable setting
//    const int nr_of_ticks = 10;

//    DoubleRange range = plane->range( m_position );
//    double range_inc = (range.second - range.first) / nr_of_ticks;


//    switch( m_position.value() )
//    {
//    case KDChartEnums::PositionSouth:
//       {
//          QPointF tick = drawArea.topLeft();
//          QPointF offset = QPointF( drawArea.width() / nr_of_ticks, 0.0 );

//          for( int i=1; i<nr_of_ticks; i++ )
//          {
//             tick += offset;
//             p->drawLine( tick, tick + QPointF( 0.0, 5.0 ) );
//             p->drawText( QRectF( tick.x() - 50.0, tick.y()+5.0, 100.0, 50.0 ),
//                QString( tr( "%1" ) ).arg( range.first + range_inc * i ),
//                QTextOption( Qt::AlignHCenter ) );
//          }

//          titleArea = QRectF( drawArea.x(), 25.0, drawArea.width(), 50.0 );

//          break;
//       }
//    case KDChartEnums::PositionWest:
//       {
//          QPointF tick = drawArea.topRight();
//          QPointF offset = QPointF( 0.0, drawArea.height() / nr_of_ticks );

//          for( int i=1; i<nr_of_ticks; i++ )
//          {
//             tick += offset;
//             p->drawLine( tick, tick + QPointF( -5.0, 0.0 ) );
//             p->drawText( QRectF( tick.x() -106.0, tick.y()-7.0, 100.0, 50.0 ),
//                          QString( tr( "%1" ) ).arg( range.first + range_inc * i  ),
//                QTextOption( Qt::AlignRight ) );
//          }

//          // prepare painter for title drawing
//          p->rotate( -90 );
//          titleArea =  QRectF( -1* drawArea.height(), 45.0, drawArea.height(), 50.0 );

//          break;
//       }
//    case KDChartEnums::PositionEast:
//       {
//          QPointF tick = drawArea.topLeft();
//          QPointF offset = QPointF( 0.0, drawArea.height() / nr_of_ticks );

//          for( int i=1; i<nr_of_ticks; i++ )
//          {
//             tick += offset;
//             p->drawLine( tick, tick + QPointF( 5.0, 0.0 ) );
//             p->drawText( QRectF( tick.x() + 6.0, tick.y()-7.0, 100.0, 50.0 ),
//                          QString( tr( "%1" ) ).arg( range.first + range_inc * i  ),
//                QTextOption( Qt::AlignLeft ) );
//          }

//          // prepare painter for title drawing
//          p->rotate( 90 );
//          titleArea = QRectF( 0.0, -45.0, drawArea.height(), 50.0 );

//          break;
//       }
//    default:
//       {
//          Q_ASSERT( false );
//          break;
//       }
//    }

//    // draw the title
//    TextAttributes titleTA( titleTextAttributes() );
//    QString titleTxt( titleText() );
//    if( titleTA.isVisible() && ! titleTxt.isEmpty() )
//    {
//       p->setFont( titleTA.font() );
//       p->setPen( titleTA.pen() );

//       p->drawText( titleArea, titleTxt, QTextOption( Qt::AlignHCenter ) );
//    }
}

bool TernaryAxis::isEmpty() const
{
    // todo: what's this method for?
    return false;
}

QRect TernaryAxis::geometry () const
{
    return m_geometry;
}

void TernaryAxis::setGeometry (const QRect &rect)
{
    m_geometry = rect;
}

QSize  TernaryAxis::minimumSize () const
{
    // todo: return realistic sizes
    return QSize( 100, 100 );
}

QSize  TernaryAxis::maximumSize () const
{
    return QSize( 300, 200 );
}

QSize  TernaryAxis::sizeHint () const
{
    return QSize( 150, 100 );
}

Qt::Orientations TernaryAxis::expandingDirections () const
{
    return Qt::Vertical | Qt::Horizontal;
}

const Position TernaryAxis::position () const
{
    return m_position;
}

void  TernaryAxis::setPosition (Position p)
{
    m_position = p;
}

void TernaryAxis::setTitleText( const QString& text )
{
    m_title = text;
}

QString TernaryAxis::titleText() const
{
    return m_title;
}

void TernaryAxis::setTitleTextAttributes( const TextAttributes &a )
{
    m_titleAttributes = a;
}

TextAttributes TernaryAxis::titleTextAttributes() const
{
    return m_titleAttributes;
}

void TernaryAxis::resetTitleTextAttributes()
{
    TextAttributes a;
    m_titleAttributes = a;
}

bool TernaryAxis::hasDefaultTitleTextAttributes() const
{
    TextAttributes a;
    return m_titleAttributes == a;
}
