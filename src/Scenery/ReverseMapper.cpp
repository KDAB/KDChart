#include <math.h>

#include <QRect>
#include <QtDebug>
#include <QPolygonF>
#include <QPainterPath>
#include <QGraphicsScene>

#include "ReverseMapper.h"
#include "../KDChartAbstractDiagram.h"
#include "ChartGraphicsItem.h"

using namespace KDChart;

ReverseMapper::ReverseMapper()
    : m_scene( 0 )
    , m_diagram( 0 )
{
}

ReverseMapper::ReverseMapper( AbstractDiagram* diagram )
    : m_scene( 0 )
    , m_diagram( diagram )
{
}

ReverseMapper::~ReverseMapper()
{
    delete m_scene; m_scene = 0;
}

void ReverseMapper::setDiagram( AbstractDiagram* diagram )
{

    m_diagram = diagram;
}

void ReverseMapper::clear()
{
    delete m_scene;
    m_scene = new QGraphicsScene();
}

QModelIndexList ReverseMapper::indexesAt( const QPointF& point ) const
{
    Q_ASSERT( m_diagram );
    if ( m_scene && m_scene->sceneRect().contains( point ) ) {
        QList<QGraphicsItem *> items = m_scene->items( point );
        QModelIndexList indexes;
        Q_FOREACH( QGraphicsItem* item, items ) {
            ChartGraphicsItem* i = qgraphicsitem_cast<ChartGraphicsItem*>( item );
            if ( i ) {
                QModelIndex index ( m_diagram->model()->index( i->row(), i->column() ) );
                indexes << index;
            }
        }
        return indexes;
    } else {
        return QModelIndexList();
    }
}

void ReverseMapper::addItem( ChartGraphicsItem* item )
{
    Q_ASSERT( m_scene );
    m_scene->addItem( item );
}

void ReverseMapper::addRect( int row, int column, const QRectF& rect )
{
    QPolygonF polygon( rect );
    ChartGraphicsItem* item = new ChartGraphicsItem( row, column );
    item->setPolygon( polygon );
    addItem( item );
}

void ReverseMapper::addCircle( int row, int column, const QPointF& location, const QSizeF& diameter )
{
    QPainterPath path;
    QPointF ossfet( -0.5*diameter.width(), -0.5*diameter.height() );
    path.addEllipse( QRectF( location + ossfet, diameter ) );
    QPolygonF polygon( path.toFillPolygon() );
    ChartGraphicsItem* item = new ChartGraphicsItem( row, column );
    item->setPolygon( polygon );
    addItem( item );
}

void ReverseMapper::addLine( int row, int column, const QPointF& from, const QPointF& to )
{
    // lines do not make good polygons to click on. we calculate a 2
    // pixel wide rectangle, where the original line is excatly
    // centered in.
    // make a 3 pixel wide polygon from the line:
    static QPointF pixel( 1.0, 1.0 );
    QPointF left, right;
    if ( from.x() < to.x() ) {
        left = from;
        right = to;
    } else {
        right = from;
        left = to;
    }
    QPointF lineVector( right - left );
    qreal lineVectorLength = sqrt( lineVector.rx()*lineVector.rx() + lineVector.ry()*lineVector.ry() );
    QPointF lineVectorUnit( lineVector / lineVectorLength );
    QPointF normOfLineVectorUnit( -lineVectorUnit.ry(), lineVectorUnit.rx() );
    // now the four polygon end points:
    QPointF one( left - lineVectorUnit + normOfLineVectorUnit );
    QPointF two( left - lineVectorUnit - normOfLineVectorUnit );
    QPointF three( right + lineVectorUnit - normOfLineVectorUnit );
    QPointF four( right + lineVectorUnit + normOfLineVectorUnit );
    QPolygonF polygon;
    polygon << one << two << three << four << one;
    ChartGraphicsItem* item = new ChartGraphicsItem( row, column );
    item->setPolygon( polygon );
    addItem( item );
}

