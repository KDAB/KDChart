#include "ChartGraphicsItem.h"

using namespace KDChart;

ChartGraphicsItem::ChartGraphicsItem()
    : QGraphicsPolygonItem()
    , m_row( -1 )
    , m_column( -1 )
{
}

ChartGraphicsItem::ChartGraphicsItem( int row, int column )
    : QGraphicsPolygonItem()
    , m_row( row )
    , m_column( column )
{
}
