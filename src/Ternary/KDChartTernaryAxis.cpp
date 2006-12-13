#include <QPainter>

#include <KDChartChart>
#include <KDChartPaintContext>

#include "TernaryConstants.h"
#include "KDChartTernaryAxis.h"
#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartAbstractTernaryDiagram.h"


#include "../src/KDChartLayoutItems.h"
#include "PrerenderedElements/KDChartTextLabelCache.h"

using namespace KDChart;

// m_label and m_fifty do not have to be pointers, once the class is
// pimpled (PrerenderedLabel is not published API)

TernaryAxis::TernaryAxis ( AbstractTernaryDiagram* diagram)
    : AbstractAxis( diagram )
    , m_position( KDChartEnums::PositionSouth )
    , m_label( new PrerenderedLabel )
    , m_fifty( new PrerenderedLabel )
{
    resetTitleTextAttributes();
    updatePrerenderedLabels();
    // FIXME is this consistent with other diagram/axis/plane implementations?
    diagram->addAxis( this );
}

TernaryAxis::~TernaryAxis()
{
    delete m_label; m_label = 0;
    delete m_label; m_fifty = 0;
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
    qDebug() << "TernaryAxis::paintCtx:" << this;

    QPainter* p = paintContext->painter();
    TernaryCoordinatePlane* plane =
        (TernaryCoordinatePlane*) paintContext->coordinatePlane();
    // QObject* refArea = plane->parent();
    QRectF drawArea = paintContext->rectangle();
    QRectF titleArea;

//     TextAttributes ta( textAttributes() );
//     p->setFont( ta.font() );
//     p->setPen( ta.pen() );

    // paint the axis label (across the triangle, that one):
    QList<PrerenderedLabel> labels;
    labels << *m_label << *m_fifty;
    Q_FOREACH( const PrerenderedLabel& label, labels ) {
        const QPixmap& pixmap = label.pixmap();
        KDChartEnums::PositionValue position = label.referencePoint();
        QPointF point = plane->translate( label.position() )
                        - label.referencePointLocation( position );
        p->drawPixmap( point, pixmap );
    }

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
    if ( p == KDChartEnums::PositionSouth
         || p == KDChartEnums::PositionWest
         || p == KDChartEnums::PositionEast ) {
        m_position = p;
        updatePrerenderedLabels();
    } else {
        qDebug() << "TernaryAxis::setPosition: only south, east and west are supported "
            "positions for ternary axes.";
    }
}

void TernaryAxis::setTitleText( const QString& text )
{
    m_title = text;
    updatePrerenderedLabels();
}

QString TernaryAxis::titleText() const
{
    return m_title;
}

void TernaryAxis::setTitleTextAttributes( const TextAttributes &a )
{
    m_titleAttributes = a;
    updatePrerenderedLabels();
}

TextAttributes TernaryAxis::titleTextAttributes() const
{
    return m_titleAttributes;
}

void TernaryAxis::resetTitleTextAttributes()
{
    TextAttributes a;
    m_titleAttributes = a;
    updatePrerenderedLabels();
}

bool TernaryAxis::hasDefaultTitleTextAttributes() const
{
    TextAttributes a;
    return m_titleAttributes == a;
}

void TernaryAxis::updatePrerenderedLabels()
{
    TextAttributes attributes = titleTextAttributes();
    double axisLabelAngle;
    double fiftyMarkAngle;
    QPointF axisLabelPosition;
    QPointF fiftyMarkPosition;
    KDChartEnums::PositionValue fiftyMarkReferencePoint;

    switch( position().value() ) { // hehe
    case KDChartEnums::PositionSouth:
        // this is the axis on the other side of A
        axisLabelAngle = 0.0;
        fiftyMarkAngle = 0.0;
        axisLabelPosition = TriangleTop;
        fiftyMarkPosition = 0.5 * AxisVector_B_C - RelMarkerLength * Norm_B_C;
        fiftyMarkReferencePoint = KDChartEnums::PositionNorth;
        break;
    case KDChartEnums::PositionEast:
        // this is the axis on the other side of B
        axisLabelAngle = 240.0;
        fiftyMarkAngle = 60;
        axisLabelPosition = TriangleBottomLeft;
        fiftyMarkPosition = AxisVector_B_C + 0.5 * AxisVector_C_A - RelMarkerLength * Norm_C_A;
        fiftyMarkReferencePoint = KDChartEnums::PositionSouth;
        break;
    case KDChartEnums::PositionWest:
        // this is the axis on the other side of C
        axisLabelAngle = 120.0;
        fiftyMarkAngle = 300.0;
        axisLabelPosition = TriangleBottomRight;
        fiftyMarkPosition = 0.5 * AxisVector_B_A + RelMarkerLength * Norm_B_A;
        fiftyMarkReferencePoint = KDChartEnums::PositionSouth;
        break;
    default:
        qDebug() << "TernaryAxis::updatePrerenderedLabel: unknown location";
    };

    m_label->setFont( attributes.font() );
    m_label->setText( titleText() );
    m_label->setAngle( axisLabelAngle );
    m_label->setPosition( axisLabelPosition );
    m_label->setReferencePoint( KDChartEnums::PositionSouth );
    m_fifty->setFont( attributes.font() );
    m_fifty->setText( QObject::tr( "50%" ) );
    m_fifty->setAngle( fiftyMarkAngle );
    m_fifty->setPosition( fiftyMarkPosition );
    m_fifty->setReferencePoint( fiftyMarkReferencePoint );
}
