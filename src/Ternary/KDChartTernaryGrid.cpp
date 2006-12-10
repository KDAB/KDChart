#include <limits>

#include <QtDebug>

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
}

TernaryGrid::~TernaryGrid()
{
}

void TernaryGrid::drawGrid( PaintContext* context )
{
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



