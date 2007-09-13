#ifndef REVERSEMAPPER_H
#define REVERSEMAPPER_H

#include <QModelIndex>

class QRectF;
class QGraphicsScene;

namespace KDChart {

    class AbstractDiagram;
    class ChartGraphicsItem;

    /**
      * @brief The ReverseMapper stores information about objects on a chart and their respective model indexes
      * \internal
      */
    class ReverseMapper
    {

    public:
        ReverseMapper();
        explicit ReverseMapper( AbstractDiagram* diagram );

        ~ReverseMapper();

        void setDiagram( AbstractDiagram* diagram );

        void clear();

        QModelIndexList indexesAt( const QPointF& point ) const;
        QModelIndexList indexesIn( const QRect& rect ) const;

        void addItem( ChartGraphicsItem* item );

        // convenience methods:
        void addPolygon( int row, int column, const QPolygonF& polygon );
        void addRect( int row, int column, const QRectF& rect );
        void addCircle( int row, int column, const QPointF& location, const QSizeF& diameter );
        void addLine( int row, int column, const QPointF& from, const QPointF& to );

    private:
        QGraphicsScene* m_scene;
        AbstractDiagram* m_diagram;
    };

}

#endif
