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
