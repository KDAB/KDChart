#ifndef PLOTTERDIAGRAMCOMPRESSOR_H
#define PLOTTERDIAGRAMCOMPRESSOR_H

#include <QtCore/QObject>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QWeakPointer>
#include <QtCore/QVector>
#include <QtCore/QDateTime>

#include <cmath>
#include <limits>

namespace KDChart
{


class PlotterDiagramCompressor : public QObject
{
    Q_OBJECT
public:

    class DataPoint {
    public:
        DataPoint()
            : key( std::numeric_limits< qreal >::quiet_NaN() ),
              value( std::numeric_limits< qreal >::quiet_NaN() ),
              hidden( false )
              {}
        inline qreal distance( const DataPoint &other )
        {
            return std::sqrt( std::pow( key - other.key, 2 ) + std::pow( value - other.value, 2 ) );
        }

        qreal key;
        qreal value;
        bool hidden;
        QModelIndex index;
    };

    class Iterator
    {
        friend class PlotterDiagramCompressor;
    public:
        Iterator( int dataSet, PlotterDiagramCompressor *parent );
        ~Iterator();
        bool isValid() const;
        Iterator& operator++();
        Iterator operator++( int );
        Iterator& operator += ( int value );
        Iterator& operator--();
        Iterator operator--( int );
        Iterator& operator-=( int value );
        DataPoint operator*();
        bool operator==( const Iterator &other ) const;
        bool operator!=( const Iterator &other ) const;
        void invalidate();
    protected:
        Iterator( int dataSet, PlotterDiagramCompressor *parent, QVector< DataPoint > buffer );
    private:
        QWeakPointer< PlotterDiagramCompressor > m_parent;
        QVector< DataPoint > m_buffer;
        int m_index;
        int m_dataset;
        int m_bufferIndex;
        int m_rebuffer;
        QDateTime m_timeOfCreation;
    };

    typedef QVector<DataPoint> DataPointVector;
    class CachePosition {
    public:
        CachePosition()
            : first( -1 ),
              second( -1 )
              {}
        CachePosition( int first, int second )
            : first( first ),
              second( second )
              {}
        int first;
        int second;

        bool operator==( const CachePosition& rhs ) const
        {
            return first == rhs.first &&
                   second == rhs.second;
        }
    };
    explicit PlotterDiagramCompressor(QObject *parent = 0);
    Iterator begin( int dataSet );
    Iterator end( int dataSet );    
    void setMergeRadius( qreal radius );
    void setMergeRadiusPercentage( qreal radius );
    void setModel( QAbstractItemModel *model );
    QAbstractItemModel* model() const;
    DataPoint data( const CachePosition& pos ) const;
    int rowCount() const;
    int datasetCount() const;
    void cleanCache();
    QPair< QPointF, QPointF > dataBoundaries() const;
    void setForcedDataBoundaries( const QPair< qreal, qreal > &bounds, Qt::Orientation direction );
Q_SIGNALS:
    void boundariesChanged();
    void rowCountChanged();

private:
    class Private;
    Private *d;
};

}

#endif // PLOTTERDIAGRAMCOMPRESSOR_H
