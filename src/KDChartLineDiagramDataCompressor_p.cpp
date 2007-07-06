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
    const QModelIndex& topleft,
    const QModelIndex& bottomright )
{
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

const LineDiagramDataCompressor::DataPoint& LineDiagramDataCompressor::data( int row, int column ) const
{
    static DataPoint NullDataPoint;
    if ( column < 0 || column > m_data.size() ) return NullDataPoint;
    if ( row < 0 || row > m_data[column].size() ) return NullDataPoint;
    if ( ! m_data[column][row].index.isValid() ) retrieveModelData( row, column );
    return m_data[column][row];
}

LineDiagramDataCompressor::DataPoint LineDiagramDataCompressor::retrieveModelData( int row, int column ) const
{
    return DataPoint();
}

QPair<int, int> LineDiagramDataCompressor::mapToCache( const QModelIndex& index ) const
{
    static const QPair<int, int> NullPosition( -1, -1 );
    if ( ! index.isValid() ) return NullPosition;
    if ( m_data.size() == 0 || m_data[0].size() == 0 ) return NullPosition;
    // assumption: indexes per column == 1
    return QPair<int, int>( index.row() / indexesPerPixel(), index.column() );
}

QModelIndexList LineDiagramDataCompressor::mapToModel( int row, int column ) const
{
    if ( ! m_model ) return QModelIndexList();
    if ( m_data.size() == 0 || m_data[0].size() == 0 ) return QModelIndexList();
    if ( column < 0 || column >= m_data.size() ) return QModelIndexList();
    if ( row < 0 || row >= m_data[0].size() ) return QModelIndexList();
    // assumption: indexes per column == 1
    QModelIndexList indexes;
    for ( int i = 0; i < indexesPerPixel(); ++i ) {
        indexes << m_model->index( row * indexesPerPixel(), column );
    }
    return indexes;
}

int LineDiagramDataCompressor::indexesPerPixel() const
{
    if ( m_data.size() == 0 ) return 0;
    if ( m_data[0].size() == 0 ) return 0;
    if ( ! m_model ) return 0;
    return m_model->rowCount() / m_data[0].size();
}
