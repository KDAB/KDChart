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
