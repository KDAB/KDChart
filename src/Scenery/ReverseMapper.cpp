#include <QRect>
#include <QtDebug>
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




