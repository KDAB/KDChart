#include "KDChartLineDiagramDataCompressor_p.h"

using namespace KDChart;

LineDiagramDataCompressor::LineDiagramDataCompressor( QObject* parent )
    : QObject( parent )
{
}

void LineDiagramDataCompressor::slotModelDataChanged(
    const QModelIndex& topleft,
    const QModelIndex& bottomright )
{
}

int LineDiagramDataCompressor::modelDataColumns() const
{
    return m_data.size();
}

int LineDiagramDataCompressor::modelDataRows() const
{
    if ( m_data.size() > 0 ) {
        return m_data.at( 0 ).size();
    } else {
        return 0;
    }
}

