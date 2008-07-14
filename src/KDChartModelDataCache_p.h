#ifndef KDCHARTMODELDATACACHE_H
#define KDCHARTMODELDATACACHE_H

#include <limits>

#include <QObject>
#include <QModelIndex>
#include <QVector>

class QAbstractItemModel;

namespace KDChart
{
    namespace ModelDataCachePrivate
    {
        class ModelSignalMapper
        {
        protected:
            ModelSignalMapper() {}
        public:
            virtual ~ModelSignalMapper() {}
            virtual void resetModel() = 0;
            virtual void columnsInserted( const QModelIndex&, int, int ) = 0;
            virtual void columnsRemoved( const QModelIndex&, int, int ) = 0;
            virtual void dataChanged( const QModelIndex&, const QModelIndex& ) = 0;
            virtual void layoutChanged() = 0;
            virtual void modelReset() = 0;
            virtual void rowsInserted( const QModelIndex&, int, int ) = 0;
            virtual void rowsRemoved( const QModelIndex&, int, int ) = 0;
        };

        // this class maps slots to a non-QObject instantiating ModelSignalMapper
        class ModelSignalMapperConnector : public QObject
        {
            Q_OBJECT
        public:
            explicit ModelSignalMapperConnector( ModelSignalMapper& mapper );
            ~ModelSignalMapperConnector();

            void connectSignals( QAbstractItemModel* model );
            void disconnectSignals( QAbstractItemModel* model );

        protected Q_SLOTS:
            void resetModel();
            void columnsInserted( const QModelIndex&, int, int );
            void columnsRemoved( const QModelIndex&, int, int );
            void dataChanged( const QModelIndex&, const QModelIndex& );
            void layoutChanged();
            void modelReset();
            void rowsInserted( const QModelIndex&, int, int );
            void rowsRemoved( const QModelIndex&, int, int );

        private:
            ModelSignalMapper& m_mapper;
        };

        template< class T>
        T nan()
        {
            return T();
        }

        template<>
        static double nan< double >()
        {
            return std::numeric_limits< double >::quiet_NaN();
        }
    }

    template< class T, int ROLE = Qt::DisplayRole >
    class ModelDataCache : public ModelDataCachePrivate::ModelSignalMapper
    {
    public:
        ModelDataCache()
            : m_model( 0 ),
              m_connector( *this )
        {
        }

        virtual ~ModelDataCache()
        {
        }

        T data( const QModelIndex& index ) const
        {
            if( !index.isValid() || index.parent() != m_rootIndex )
                return ModelDataCachePrivate::nan< T >();

            if( index.row() >= m_data.count() )
            {
                qWarning( "KDChart didn't got signal rowsInserted, resetModel or layoutChanged, "
                          "but an index with a row outside of the known bounds." );
                // apparently, data were added behind our back (w/o signals)
                const_cast< ModelDataCache< T, ROLE >* >( this )->rowsInserted( m_rootIndex, 
                                                                                m_data.count(), 
                                                                                m_model->rowCount( m_rootIndex ) - 1 );
                Q_ASSERT( index.row() < m_data.count() );
            }

            if( index.column() >= m_data.first().count() )
            {
                qWarning( "KDChart didn't got signal columnsInserted, resetModel or layoutChanged, "
                          "but an index with a column outside of the known bounds." );
                // apparently, data were added behind our back (w/o signals)
                const_cast< ModelDataCache< T, ROLE >* >( this )->columnsInserted( m_rootIndex, 
                                                                                   m_data.first().count(), 
                                                                                   m_model->columnCount( m_rootIndex ) - 1 );
                Q_ASSERT( index.column() < m_data.first().count() );
            }

            return data( index.row(), index.column() );
        }

        T data( int row, int column ) const
        {
            if( row < 0 || column < 0 )
                return ModelDataCachePrivate::nan< T >();

            Q_ASSERT( row < m_data.count() );
            Q_ASSERT( column < m_data.first().count() );

            if( isCached( row, column ) )
                return m_data.at( row ).at( column );

            return fetchFromModel( row, column, ROLE );
        }

        void setModel( QAbstractItemModel* model )
        {
            if( m_model != 0 )
                m_connector.disconnectSignals( m_model );

            m_model = model;
            
            if( m_model != 0 )
                m_connector.connectSignals( m_model );
            
            modelReset();
        }

        QAbstractItemModel* model() const
        {
            return m_model;
        }

        void setRootIndex( const QModelIndex& rootIndex )
        {
            Q_ASSERT( rootIndex.model() == m_model || !rootIndex.isValid() );
            m_rootIndex = rootIndex;
            modelReset();
        }

        QModelIndex rootIndex() const
        {
            return m_rootIndex;
        }

    protected:
        bool isCached( int row, int column ) const
        {
            return m_cacheValid.at( row ).at( column );
        }

        T fetchFromModel( int row, int column, int role ) const
        {
            Q_ASSERT( m_model != 0 );

            const QModelIndex index = m_model->index( row, column, m_rootIndex );
            const QVariant data = index.data( role );
            const T value = data.isNull() ? ModelDataCachePrivate::nan< T >() 
                                          : qVariantValue< T >( data );
    
            m_data[ row ][ column ] = value;
            m_cacheValid[ row ][ column ] = true;

            return value;
        }

    protected:
        void columnsInserted( const QModelIndex& parent, int start, int end )
        {
            Q_ASSERT( m_model != 0 );

            if( parent != m_rootIndex )
                return;

            const int rowCount = m_data.count();

            for( int row = 0; row < rowCount; ++row )
            {
                m_data[ row ].insert( start, end - start + 1, T() );
                m_cacheValid[ row ].insert( start, end - start + 1, false );
                Q_ASSERT( m_data.at( row ).count() == m_model->columnCount( m_rootIndex ) );
                Q_ASSERT( m_cacheValid.at( row ).count() == m_model->columnCount( m_rootIndex ) );
            }
        }

        void columnsRemoved( const QModelIndex& parent, int start, int end )
        {
            Q_ASSERT( m_model != 0 );

            if( parent != m_rootIndex )
                return;

            const int rowCount = m_data.count();

            for( int row = 0; row < rowCount; ++row )
            {
                m_data[ row ].remove( start, end - start + 1 );
                m_cacheValid[ row ].remove( start, end - start + 1 );
                Q_ASSERT( m_data.at( row ).count() == m_model->columnCount( m_rootIndex ) );
                Q_ASSERT( m_cacheValid.at( row ).count() == m_model->columnCount( m_rootIndex ) );
            }
        }

        void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
        {
            Q_ASSERT( m_model != 0 );
            Q_ASSERT( topLeft.parent() == bottomRight.parent() );

            if( topLeft.parent() != m_rootIndex )
                return;
    
            const int minRow = topLeft.row();
            const int maxRow = bottomRight.row();
            const int minCol = topLeft.column();
            const int maxCol = bottomRight.column();

            for( int row = qMax( 0, minRow ); row <= maxRow; ++row )
            {
                for( int col = qMax( 0, minCol ); col <= maxCol; ++col )
                {
                    m_cacheValid[ row ][ col ] = false;
                    Q_ASSERT( !isCached( row, col ) );
                }
            }
        }

        void layoutChanged()
        {
            modelReset();
        }

        void modelReset() 
        {
            m_data.clear();
            m_cacheValid.clear();

            if( m_model == 0 )
                return;
   
            m_data.fill( QVector< T >( m_model->columnCount( m_rootIndex ) ), m_model->rowCount( m_rootIndex ) );
            m_cacheValid.fill( QVector< bool >( m_model->columnCount( m_rootIndex ), false ), m_model->rowCount( m_rootIndex ) );

            Q_ASSERT( m_data.count() == m_model->rowCount( m_rootIndex ) );
            Q_ASSERT( m_cacheValid.count() == m_model->rowCount( m_rootIndex ) );
        }

        void rowsInserted( const QModelIndex& parent, int start, int end )
        {
            Q_ASSERT( m_model != 0 );

            if( parent != m_rootIndex )
                return;

            m_data.insert( start, end - start + 1, QVector< T >( m_model->columnCount( m_rootIndex ) ) );
            m_cacheValid.insert( start, end - start + 1, QVector< bool >( m_model->columnCount( m_rootIndex ), false ) );

            Q_ASSERT( m_data.count() == m_model->rowCount( m_rootIndex ) );
            Q_ASSERT( m_cacheValid.count() == m_model->rowCount( m_rootIndex ) );
        }

        void rowsRemoved( const QModelIndex& parent, int start, int end )
        {
            Q_ASSERT( m_model != 0 );

            if( parent != m_rootIndex )
                return;

            m_data.remove( start, end - start + 1 );
            m_cacheValid.remove( start, end - start + 1 );

            Q_ASSERT( m_data.count() == m_model->rowCount( m_rootIndex ) );
            Q_ASSERT( m_cacheValid.count() == m_model->rowCount( m_rootIndex ) );
        }

        void resetModel()
        {
            // no need to disconnect, this is a response to SIGNAL( destroyed() )
            m_model = 0;
            modelReset();
        }


    private:
        QAbstractItemModel* m_model;
        QModelIndex m_rootIndex;
        ModelDataCachePrivate::ModelSignalMapperConnector m_connector;
        mutable QVector< QVector< T > > m_data;
        mutable QVector< QVector< bool > > m_cacheValid;
    };
}

#endif
