#ifndef KDCHARTMODELDATACACHE_H
#define KDCHARTMODELDATACACHE_H

#include <QObject>
#include <QModelIndex>
#include <QVector>

class QAbstractItemModel;

namespace KDChart
{
    class ModelDataCache : public QObject
    {
        Q_OBJECT
    public:
        ModelDataCache( QObject* parent = 0 );
        ~ModelDataCache();

        float data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
        float data( int row, int column, int role = Qt::DisplayRole ) const;

        void setModel( QAbstractItemModel* model );
        QAbstractItemModel* model() const;

    protected:
        bool isCached( int row, int column ) const;
        float fetchFromModel( int row, int column, int role = Qt::DisplayRole ) const;

    private Q_SLOTS:
        void columnsInserted( const QModelIndex& parent, int start, int end );
        void columnsRemoved( const QModelIndex& parent, int start, int end );
        void dataChanged( const QModelIndex& topLeft, const QModelIndex & bottomRight );
        void layoutChanged();
        void modelReset();        
        void rowsInserted( const QModelIndex& parent, int start, int end );
        void rowsRemoved( const QModelIndex& parent, int start, int end );

        void resetModel();

    private:
        QAbstractItemModel* m_model;
        QModelIndex m_rootIndex;
        mutable QVector< QVector< float > > m_data;
        mutable QVector< QVector< bool > > m_cacheValid;
    };
}

#endif
