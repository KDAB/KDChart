#include <QStandardItemModel>

#include "KDChartLineDiagramDataCompressor_p.h"

using namespace KDChart;

LineDiagramDataCompressor::LineDiagramDataCompressor( QObject* parent )
    : QObject( parent )
    , m_xResolution( 0 )
    , m_yResolution( 0 )
{
}

void LineDiagramDataCompressor::slotModelDataChanged(
    const QModelIndex& topLeftIndex,
    const QModelIndex& bottomRightIndex )
{
    Q_ASSERT( topLeftIndex.row() <= bottomRightIndex.row() );
    Q_ASSERT( topLeftIndex.column() <= bottomRightIndex.column() );
    CachePosition topleft = mapToCache( topLeftIndex );
    CachePosition bottomright = mapToCache( bottomRightIndex );
    for ( int row = topleft.first; row < bottomright.first; ++row )
        for ( int column = topleft.second; column < bottomright.second; ++column )
            invalidate( CachePosition( row, column ) );
}

int LineDiagramDataCompressor::modelDataColumns() const
{
    // only operational if there is a model and a resolution
    if ( m_model ) {
        Q_ASSERT( m_model->columnCount() == m_data.size() );
        return m_model->columnCount();
    } else {
        return 0;
    }
}

int LineDiagramDataCompressor::modelDataRows() const
{
    // only operational if there is a model, columns, and a resolution
    if ( m_model && m_model->columnCount() > 0 && m_xResolution > 0 ) {
        return m_data[0].size();
    } else {
        return 0;
    }
}

void LineDiagramDataCompressor::setModel( QStandardItemModel* model )
{
    if ( m_model != 0 && m_model != model ) {
        disconnect( m_model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),
                    this, SLOT( slotModelDataChanged( QModelIndex, QModelIndex ) ) );
        m_model = 0;
    }

    if ( model != 0 ) {
        m_model = model;
        connect( m_model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),
                 SLOT( slotModelDataChanged( QModelIndex, QModelIndex ) ) );
    }

    rebuildCache();
}

void LineDiagramDataCompressor::setResolution( int x, int y )
{
    if ( x != m_xResolution || y != m_yResolution ) {
        m_xResolution = x;
        m_yResolution = y;
        rebuildCache();
    }
}

void LineDiagramDataCompressor::clearCache()
{
    for ( int column = 0; column < m_data.size(); ++column )
        m_data[column].fill( DataPoint() );
}

void LineDiagramDataCompressor::rebuildCache()
{
    m_data.clear();
    const int columnCount = m_model ? m_model->columnCount() : 0;
    const int rowCount = qMin( m_model ? m_model->rowCount() : 0, m_xResolution );
    m_data.resize( columnCount );
    for ( int i = 0; i < columnCount; ++i ) {
        m_data[i].resize( rowCount );
        m_data[i].fill( DataPoint() );
    }
}

const LineDiagramDataCompressor::DataPoint& LineDiagramDataCompressor::data( const CachePosition& position ) const
{
    static DataPoint NullDataPoint;
    if ( ! isValidCachePosition( position ) ) return NullDataPoint;
    if ( ! isCached( position ) ) retrieveModelData( position );
    return m_data[position.second][position.first];
}

LineDiagramDataCompressor::DataPoint LineDiagramDataCompressor::retrieveModelData( const CachePosition& position ) const
{
    return DataPoint();
}

LineDiagramDataCompressor::CachePosition LineDiagramDataCompressor::mapToCache(
    const QModelIndex& index ) const
{
    static const CachePosition NullPosition( -1, -1 );
    if ( ! index.isValid() ) return NullPosition;
    if ( m_data.size() == 0 || m_data[0].size() == 0 ) return NullPosition;
    // assumption: indexes per column == 1
    return CachePosition( index.row() / indexesPerPixel(), index.column() );
}

QModelIndexList LineDiagramDataCompressor::mapToModel( const CachePosition& position ) const
{
    if ( isValidCachePosition( position ) ) {
        // assumption: indexes per column == 1
        QModelIndexList indexes;
        for ( int i = 0; i < indexesPerPixel(); ++i ) {
            indexes << m_model->index( position.first * indexesPerPixel(), position.second );
        }
        return indexes;
    } else {
        return QModelIndexList();
    }
}

int LineDiagramDataCompressor::indexesPerPixel() const
{
    if ( m_data.size() == 0 ) return 0;
    if ( m_data[0].size() == 0 ) return 0;
    if ( ! m_model ) return 0;
    return m_model->rowCount() / m_data[0].size();
}

bool LineDiagramDataCompressor::isValidCachePosition( const CachePosition& position ) const
{
    if ( ! m_model ) return false;
    if ( m_data.size() == 0 || m_data[0].size() == 0 ) return false;
    if ( position.second < 0 || position.second >= m_data.size() ) return false;
    if ( position.first < 0 || position.first >= m_data[0].size() ) return false;
    return true;
}

void LineDiagramDataCompressor::invalidate( const CachePosition& position )
{
    if ( isValidCachePosition( position ) )
        m_data[position.second][position.first] = DataPoint();
}

bool LineDiagramDataCompressor::isCached( const CachePosition& position ) const
{
    Q_ASSERT( isValidCachePosition( position ) );
    return m_data[position.second][position.first].index.isValid();
}
