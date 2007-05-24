#ifndef CHARTGRAPHICSITEM_H
#define CHARTGRAPHICSITEM_H

#include <QGraphicsPolygonItem>

namespace KDChart {

    class ChartGraphicsItem : public QGraphicsPolygonItem
    {
    public:
        enum { Type = UserType + 1 };

        ChartGraphicsItem();

        ChartGraphicsItem( int row,  int column );

        int row() const { return m_row; }
        int column() const { return m_column; }
        int type() const { return Type; }

    private:
        int m_row;
        int m_column;
    };

}

#endif
