#ifndef PLOTTERDIAGRAMCOMPRESSOR_P_H
#define PLOTTERDIAGRAMCOMPRESSOR_P_H

#include "KDChartPlotterDiagramCompressor.h"

#include <QtCore/QPointF>
#include <QtCore/QDateTime>

typedef QPair< QPointF, QPointF > Boundaries;

namespace KDChart
{

class PlotterDiagramCompressor::Private : public QObject
{
    Q_OBJECT
public:
    Private( PlotterDiagramCompressor *parent );
    QModelIndexList mapToModel( const CachePosition& pos );
    void calculateDataBoundaries();    
    void setBoundaries( const Boundaries &bound );
    bool forcedBoundaries( Qt::Orientation orient ) const;
    bool inBoundaries( Qt::Orientation orient, const PlotterDiagramCompressor::DataPoint &dp ) const;
    PlotterDiagramCompressor *m_parent;
    QAbstractItemModel *m_model;
    qreal m_mergeRadius;
    QVector< QVector< DataPoint > > m_bufferlist;
    Boundaries m_boundary;
    QPair< qreal, qreal > m_forcedXBoundaries;
    QPair< qreal, qreal > m_forcedYBoundaries;
    QDateTime m_timeOfLastInvalidation;
    //QVector< PlotterDiagramCompressor::Iterator > exisitingIterators;
public Q_SLOTS:
    void rowsInserted( const QModelIndex& parent, int start, int end );
    void clearBuffer();
};

}

#endif // PLOTTERDIAGRAMCOMPRESSOR_P_H
