#include "KDChartModelDataCache_p.h"

using namespace KDChart;

ModelDataCache::ModelDataCache( QObject* parent )
    : QObject( parent ),
      m_model( 0 )
{
}

ModelDataCache::~ModelDataCache()
{
}

float ModelDataCache::data( const QModelIndex& index, int role ) const
{
    if( !index.isValid() || index.model() != m_model )
        return 0.0;
    return data( index.row(), index.column(), role );
}

float ModelDataCache::data( int row, int column, int role ) const
{
    if( row < 0 || column < 0 )
        return 0.0;

    Q_ASSERT( row < m_data.count() );
    Q_ASSERT( column < m_data.first().count() );

    if( isCached( row, column ) )
        return m_data.at( row ).at( column );

    return fetchFromModel( row, column, role );
}

void ModelDataCache::setModel( QAbstractItemModel* model )
{
/*    if( m_model != 0 )
    {
        disconnect( m_model, SIGNAL( columnsInserted( QModelIndex, int, int ) ), this, SLOT( columnsInserted( QModelIndex, int, int ) ) );
        disconnect( m_model, SIGNAL( columnsRemoved( QModelIndex, int, int ) ),  this, SLOT( columnsRemoved( QModelIndex, int, int ) ) );
        disconnect( m_model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),  this, SLOT( dataChanged( QModelIndex, QModelIndex ) ) );
        disconnect( m_model, SIGNAL( layoutChanged() ),                          this, SLOT( layoutChanged() ) );
        disconnect( m_model, SIGNAL( modelReset() ),                             this, SLOT( modelReset() ) );
        disconnect( m_model, SIGNAL( rowsInserted( QModelIndex, int, int ) ),    this, SLOT( rowsInserted( QModelIndex, int, int )) );
        disconnect( m_model, SIGNAL( rowsRemoved( QModelIndex, int, int ) ),     this, SLOT( rowsRemoved( QModelIndex, int, int ) ) );
    }*/
    m_model = model;
    if( m_model != 0 )
    {
        connect( m_model, SIGNAL( columnsInserted( QModelIndex, int, int ) ), this, SLOT( columnsInserted( QModelIndex, int, int ) ) );
        connect( m_model, SIGNAL( columnsRemoved( QModelIndex, int, int ) ),  this, SLOT( columnsRemoved( QModelIndex, int, int ) ) );
        connect( m_model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),  this, SLOT( dataChanged( QModelIndex, QModelIndex ) ) );
        connect( m_model, SIGNAL( layoutChanged() ),                          this, SLOT( layoutChanged() ) );
        connect( m_model, SIGNAL( modelReset() ),                             this, SLOT( modelReset() ) );
        connect( m_model, SIGNAL( rowsInserted( QModelIndex, int, int ) ),    this, SLOT( rowsInserted( QModelIndex, int, int )) );
        connect( m_model, SIGNAL( rowsRemoved( QModelIndex, int, int ) ),     this, SLOT( rowsRemoved( QModelIndex, int, int ) ) );
    }
    modelReset();
}

QAbstractItemModel* ModelDataCache::model() const
{
    return m_model;
}

bool ModelDataCache::isCached( int row, int column ) const
{
    return m_cacheValid.at( row ).at( column );
}

float ModelDataCache::fetchFromModel( int row, int column, int role ) const
{
    Q_ASSERT( m_model != 0 );

    const QModelIndex index = m_model->index( row, column );
    const float value = index.data( role ).toDouble();

    m_data[ row ][ column ] = value;
    m_cacheValid[ row ][ column ] = true;

    return value;
}

void ModelDataCache::columnsInserted( const QModelIndex& parent, int start, int end )
{
    Q_ASSERT( m_model != 0 );

    if( parent != m_rootIndex )
        return;

    const int rowCount = m_data.count();

    for( int row = 0; row < rowCount; ++row )
    {
        m_data[ row ].insert( start, end - start + 1, 0.0 );
        m_cacheValid[ row ].insert( start, end - start + 1, false );
        Q_ASSERT( m_data.at( row ).count() == m_model->columnCount( m_rootIndex ) );
        Q_ASSERT( m_cacheValid.at( row ).count() == m_model->columnCount( m_rootIndex ) );
    }
}

void ModelDataCache::columnsRemoved( const QModelIndex& parent, int start, int end )
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

void ModelDataCache::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    Q_ASSERT( m_model != 0 );
    Q_ASSERT( topLeft.parent() == bottomRight.parent() );

    if( topLeft.parent() != m_rootIndex )
        return;
    
    const int minRow = topLeft.row();
    const int maxRow = bottomRight.row();
    const int minCol = topLeft.column();
    const int maxCol = bottomRight.column();

    for( int row = minRow; row <= maxRow; ++row )
    {
        for( int col = minCol; col <= maxCol; ++col )
        {
            m_cacheValid[ row ][ col ] = false;
            Q_ASSERT( !isCached( row, col ) );
        }
    }
}

void ModelDataCache::layoutChanged()
{
    modelReset();
}

void ModelDataCache::modelReset()
{
    m_data.clear();
    m_cacheValid.clear();

    Q_ASSERT( m_model != 0 );
   
    m_data.fill( QVector< float >( m_model->columnCount( m_rootIndex ), 0.0 ), m_model->rowCount( m_rootIndex ) );
    m_cacheValid.fill( QVector< bool >( m_model->columnCount( m_rootIndex ), false ), m_model->rowCount( m_rootIndex ) );

    Q_ASSERT( m_data.count() == m_model->rowCount( m_rootIndex ) );
    Q_ASSERT( m_cacheValid.count() == m_model->rowCount( m_rootIndex ) );
}

void ModelDataCache::rowsInserted( const QModelIndex& parent, int start, int end )
{
    Q_ASSERT( m_model != 0 );

    if( parent != m_rootIndex )
        return;

    m_data.insert( start, end - start + 1, QVector< float >( m_model->columnCount( m_rootIndex ), 0.0 ) );
    m_cacheValid.insert( start, end - start + 1, QVector< bool >( m_model->columnCount( m_rootIndex ), false ) );

    Q_ASSERT( m_data.count() == m_model->rowCount( m_rootIndex ) );
    Q_ASSERT( m_cacheValid.count() == m_model->rowCount( m_rootIndex ) );
}

void ModelDataCache::rowsRemoved( const QModelIndex& parent, int start, int end )
{
    Q_ASSERT( m_model != 0 );

    if( parent != m_rootIndex )
        return;

    m_data.remove( start, end - start + 1 );
    m_cacheValid.remove( start, end - start + 1 );

    Q_ASSERT( m_data.count() == m_model->rowCount( m_rootIndex ) );
    Q_ASSERT( m_cacheValid.count() == m_model->rowCount( m_rootIndex ) );
}
