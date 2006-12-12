#include <limits>

#include <QtDebug>
#include <QApplication>

#include "TernaryPoint.h"
#include "TernaryConstants.h"
#include "KDChartPaintContext.h"
#include "KDChartTernaryGrid.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartTernaryCoordinatePlane.h"

using namespace KDChart;

bool operator==(const TickInfo& left, const TickInfo& right)
{
    return fabs( left.percentage - right.percentage )
        <= std::numeric_limits<double>::epsilon()
        && left.depth == right.depth;
}

TernaryGrid::TernaryGrid()
    : AbstractGrid()
{
//     PrerenderedLabel labelA, labelB, labelC,
//         label4, label5, label6;
//     // FIXME this has to come fro TextAttributes
//     QFont font ( qApp->font() );
//     font.setPointSizeF( 1.3 * font.pointSize() );
//     labelA.paint( font, m_labelA, 0 );
//     labelA.setPosition( TriangleTop );
//     labelA.setReferencePoint( KDChartEnums::PositionSouth );
//     labelB.paint( font, m_labelB, 240 );
//     labelB.setPosition( TriangleBottomLeft );
//     labelB.setReferencePoint( KDChartEnums::PositionSouth );
//     labelC.paint( font, m_labelC, 120 );
//     labelC.setPosition( TriangleBottomRight );
//     labelC.setReferencePoint( KDChartEnums::PositionSouth );
//     font.setPointSizeF( 0.8 * qApp->font().pointSize() );
//     QString fifty( QObject::tr( "50%" ) );
//     label4.paint( font, fifty, 0 );
//     label4.setPosition( 0.5 * AxisVector_B_C - RelMarkerLength * Norm_B_C );
//     label4.setReferencePoint( KDChartEnums::PositionNorth );
//     label5.paint( font, fifty, 60 );
//     label5.setPosition( AxisVector_B_C + 0.5 * AxisVector_C_A - RelMarkerLength * Norm_C_A );
//     label5.setReferencePoint( KDChartEnums::PositionSouth );
//     label6.paint( font, fifty, 300 );
//     label6.setPosition( 0.5 * AxisVector_B_A + RelMarkerLength * Norm_B_A );
//     label6.setReferencePoint( KDChartEnums::PositionSouth );

//     // do not change the order of the first three labels, those are
//     // referenced by position later:
//     m_labels << labelA << labelB << labelC
//              << label4 << label5 << label6;
}

TernaryGrid::~TernaryGrid()
{
}

void TernaryGrid::drawGrid( PaintContext* context )
{
    qDebug() << "TernaryGrid::drawGrid:" << this;

    static const int GridLineDistanceTreshold = 20; // <Treshold> pixels between each grid line

    QPainter& painter ( *context->painter() ); // recover from pointer madness
    PainterSaver s( &painter ); // can i have a reference based version of that?
    TernaryCoordinatePlane* plane = dynamic_cast<TernaryCoordinatePlane*>(context->coordinatePlane());
    Q_ASSERT_X ( plane, "TernaryGrid::drawGrid",
                 "Bad function call: PaintContext::coodinatePlane() NOT a ternary plane." );

    // translate the points and see how many grid lines we can draw:
    const int MaxDepth = 3;
    double xPixels = plane->translate( TriangleBottomRight ).x() -
                     plane->translate( TriangleBottomLeft ).x();
    int granularity = 20;
    if ( xPixels > 10 * GridLineDistanceTreshold ) granularity = 10;
    if ( xPixels > 20 * GridLineDistanceTreshold ) granularity = 5;

    m_tickInfo.clear();
    for ( int i = granularity; i < 100; i+=granularity )
    {
        TickInfo tick = { ( 1.0 * i ) / 100.0, 2 };
        if ( i % 10 == 0 ) tick.depth = 1;
        if ( i % 20 == 0 ) tick.depth = 0;
        m_tickInfo.append( tick );
    }

    QVector<QLineF> lines[MaxDepth];
    Q_FOREACH( TickInfo tick, m_tickInfo ) {
        const double& percent = tick.percentage;
        {   // draw parallels to B
            TernaryPoint ternaryStart( percent, 1.0 - percent );
            TernaryPoint ternaryEnd( 0.0, 1.0 - percent );
            QPointF start( translate( ternaryStart ) );
            QPointF end( translate( ternaryEnd ) );
            lines[tick.depth].append( QLineF( plane->translate( start ),
                                              plane->translate( end ) ) );
        }
        {   // draw parallels to C
            TernaryPoint ternaryStart( percent, 0.0 );
            TernaryPoint ternaryEnd( 0.0, percent );
            QPointF start( translate( ternaryStart ) );
            QPointF end( translate( ternaryEnd ) );
            lines[tick.depth].append( QLineF( plane->translate( start ),
                                              plane->translate( end ) ) );
        }
        {   // draw parallels to A
            TernaryPoint ternaryStart( percent, 1.0 - percent );
            TernaryPoint ternaryEnd( percent, 0.0 );
            QPointF start( translate( ternaryStart ) );
            QPointF end( translate( ternaryEnd ) );
            lines[tick.depth].append( QLineF( plane->translate( start ),
                                              plane->translate( end ) ) );
        }
    }

    // FIXME: this is not a good location for that:
    painter.setRenderHint(QPainter::Antialiasing, true );

    // now draw the lines:
    painter.setPen( QPen( QColor( "lightgray" ), 1 ) );
    painter.setBrush( QColor( "lightgray" ) );
    painter.drawLines( lines[2] );
    painter.setPen( QPen( QColor( "gray" ), 1 ) );
    painter.setBrush( QColor( "gray" ) );
    painter.drawLines( lines[1] );
    painter.setPen( QPen( QColor( "darkslategray" ), 1 ) );
    painter.setBrush( QColor( "darkslategray" ) );
    painter.drawLines( lines[0] );

    // now draw the triangle (this could be part of the axis, in fact):
    painter.setPen( QPen( Qt::black, 1 ) );
    // make sure this does not fill, otherwise it wipes the contents
    // of the triangle (doh!):
    painter.setBrush( Qt::NoBrush );
    QPolygonF points;
    points << plane->translate( TriangleBottomLeft )
           << plane->translate( TriangleBottomRight )
           << plane->translate( TriangleTop );
    painter.drawPolygon( points );
}

DataDimensionsList TernaryGrid::calculateGrid( const DataDimensionsList& ) const
{
    return DataDimensionsList();
}

QPair<QSizeF, QSizeF> TernaryGrid::requiredMargins() const
{
    return QPair<QSizeF, QSizeF>( QSizeF(), QSizeF() );

/*
    // well, we just know about the semantics of the labels, so:
    double topMargin = m_labels[0].pixmap().height(); // label A
    double labelBWidth = m_labels[1].pixmap().width(); // label B
    double leftMargin = labelBWidth -
                        ( labelBWidth
                          - m_labels[1].referencePointLocation( KDChartEnums::PositionSouth ).x() );
    double rightMargin = m_labels[2].pixmap().width() //label C
                         - m_labels[2].referencePointLocation( KDChartEnums::PositionSouth ).x();
    return QPair<QSizeF, QSizeF>
        ( QSizeF( leftMargin, topMargin ),
          QSizeF( rightMargin,topMargin ) );
*/
}

const QVector<TickInfo>& TernaryGrid::tickInfo() const
{
    return m_tickInfo;
}
