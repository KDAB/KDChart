#include "KDChartPlotterDiagramCompressor.h"

#include "KDChartPlotterDiagramCompressor_p.h"
#include <QtCore/QPointF>

#include <limits>

using namespace KDChart;

PlotterDiagramCompressor::Iterator::Iterator( int dataSet, PlotterDiagramCompressor *parent )
    : m_parent( parent )
    , m_index( 0 )
    , m_dataset( dataSet )
    , m_bufferIndex( 0 )
    , m_rebuffer( true )
{
    if ( m_parent )
    {
        if ( parent->rowCount() > m_dataset && parent->rowCount() > 0 )
        {
            m_buffer.append( parent->data( CachePosition( m_index, m_dataset ) ) );
        }
    }
    else
    {
        m_dataset = - 1;
        m_index = - 1;
    }
}

PlotterDiagramCompressor::Iterator::Iterator( int dataSet, PlotterDiagramCompressor *parent, QVector< DataPoint > buffer )
    : m_parent( parent )
    , m_buffer( buffer )
    , m_index( 0 )
    , m_dataset( dataSet )
    , m_bufferIndex( 0 )
    , m_rebuffer( false )
    , m_timeOfCreation( QDateTime::currentDateTime() )
{
    if ( !m_parent )
    {
        m_dataset = -1 ;
        m_index = - 1;
    }
    else
    {
        // buffer needs to be filled
        if ( parent->datasetCount() > m_dataset && parent->rowCount() > 0 && m_buffer.isEmpty() )
        {
            m_buffer.append( parent->data( CachePosition( m_index, m_dataset ) ) );
            m_rebuffer = true;
        }
    }
}

PlotterDiagramCompressor::Iterator::~Iterator()
{
    if ( m_parent )
    {
        if ( m_parent.data()->d->m_timeOfLastInvalidation < m_timeOfCreation )
            m_parent.data()->d->m_bufferlist[ m_dataset ] = m_buffer;
    }
}

bool PlotterDiagramCompressor::Iterator::isValid() const
{
    if ( m_parent == 0 )
        return false;
    return m_dataset >= 0 && m_index >= 0 && m_parent.data()->rowCount() > m_index;
}

PlotterDiagramCompressor::Iterator& PlotterDiagramCompressor::Iterator::operator++()
{
    ++m_index;

    ++m_bufferIndex;
    if ( m_parent.data()->d->forcedBoundaries( Qt::Vertical ) || m_parent.data()->d->forcedBoundaries( Qt::Vertical ) )
    {
        if ( m_bufferIndex >= m_buffer.count()  && m_rebuffer )
        {
            if ( m_index < m_parent.data()->rowCount() )
            {
                PlotterDiagramCompressor::DataPoint dp = m_parent.data()->data( CachePosition( m_index, m_dataset ) );
                if ( m_parent.data()->d->inBoundaries( Qt::Vertical, dp ) && m_parent.data()->d->inBoundaries( Qt::Horizontal, dp ) )
                {
                    m_buffer.append( dp );
                }
                else
                {
                    if ( m_index + 1 < m_parent.data()->rowCount() )
                    {
                        PlotterDiagramCompressor::DataPoint dp1 = m_parent.data()->data( CachePosition( m_index, m_dataset ) );
                        if ( m_parent.data()->d->inBoundaries( Qt::Vertical, dp1 ) && m_parent.data()->d->inBoundaries( Qt::Horizontal, dp1 ) )
                        {
                            m_buffer.append( dp );
                        }
                    }
                }
            }
        }
        else
        {
            if ( m_bufferIndex == m_buffer.count() )
                m_index = - 1;
            return *this;
        }
        PlotterDiagramCompressor::DataPoint dp;
        if ( isValid() )
            dp = m_parent.data()->data( CachePosition( m_index - 1, m_dataset ) );
        if ( m_parent )
        {
            if ( m_index >= m_parent.data()->rowCount() )
                m_index = -1;
            else
            {
                const qreal mergeRadius = m_parent.data()->d->m_mergeRadius;
                PlotterDiagramCompressor::DataPoint newdp = m_parent.data()->data( CachePosition( m_index, m_dataset ) );
                while ( dp.distance( newdp ) <= mergeRadius
                        || !( m_parent.data()->d->inBoundaries( Qt::Vertical, dp ) || m_parent.data()->d->inBoundaries( Qt::Horizontal, dp ) ) )
                {
                    ++m_index;
                    if ( m_index >= m_parent.data()->rowCount() )
                    {
                        m_index = - 1;
                        break;
                    }
                    newdp = m_parent.data()->data( CachePosition( m_index, m_dataset ) );
                }
            }
        }
    }
    else
    {
        // we have a new point in the buffer
        if ( m_bufferIndex >= m_buffer.count()  && m_rebuffer )
        {
            if ( m_index < m_parent.data()->rowCount() )
                m_buffer.append( m_parent.data()->data( CachePosition( m_index, m_dataset ) ) );
        }
        else
        {
            if ( m_bufferIndex == m_buffer.count() )
                m_index = - 1;
            return *this;
        }
        PlotterDiagramCompressor::DataPoint dp;
        if ( isValid() )
            dp = m_parent.data()->data( CachePosition( m_index - 1, m_dataset ) );
        // make sure we switch to the next point which would be in the buffer
        if ( m_parent )
        {
            if ( m_index >= m_parent.data()->rowCount() )
                m_index = -1;
            else
            {
                const qreal mergeRadius = m_parent.data()->d->m_mergeRadius;
                PlotterDiagramCompressor::DataPoint newdp = m_parent.data()->data( CachePosition( m_index, m_dataset ) );
                while ( dp.distance( newdp ) <= mergeRadius )
                {
                    ++m_index;
                    if ( m_index >= m_parent.data()->rowCount() )
                    {
                        m_index = - 1;
                        break;
                    }
                    newdp = m_parent.data()->data( CachePosition( m_index, m_dataset ) );
                }
            }
        }
    }
    return *this;
}

PlotterDiagramCompressor::Iterator PlotterDiagramCompressor::Iterator::operator++( int )
{
    Iterator result = *this;
    ++result;
    return result;
}

PlotterDiagramCompressor::Iterator& PlotterDiagramCompressor::Iterator::operator += ( int value )
{    
    for ( int index = m_index; index + value != m_index; ++( *this ) );
    return *this;
}

PlotterDiagramCompressor::Iterator& PlotterDiagramCompressor::Iterator::operator--()
{    
    --m_index;
    --m_bufferIndex;
    return *this;
}

PlotterDiagramCompressor::Iterator PlotterDiagramCompressor::Iterator::operator--( int )
{
    Iterator result = *this;
    --result;
    return result;
}

PlotterDiagramCompressor::Iterator& PlotterDiagramCompressor::Iterator::operator-=( int value )
{
    m_index -= value;
    return *this;
}

PlotterDiagramCompressor::DataPoint PlotterDiagramCompressor::Iterator::operator*()
{
    if ( !m_parent )
        return PlotterDiagramCompressor::DataPoint();
    Q_ASSERT( m_parent );
    return m_buffer[ m_bufferIndex ];
}

bool PlotterDiagramCompressor::Iterator::operator==( const PlotterDiagramCompressor::Iterator &other ) const
{
    return m_parent.data() == other.m_parent.data() && m_index == other.m_index && m_dataset == other.m_dataset;
}

bool PlotterDiagramCompressor::Iterator::operator!=( const PlotterDiagramCompressor::Iterator &other ) const
{
    return ! ( *this == other );
}

void PlotterDiagramCompressor::Iterator::invalidate()
{
    m_dataset = - 1;
}

PlotterDiagramCompressor::Private::Private( PlotterDiagramCompressor *parent )
    : m_parent( parent )
    , m_model( 0 )
    , m_boundary( qMakePair( QPointF( std::numeric_limits<qreal>::quiet_NaN(), std::numeric_limits<qreal>::quiet_NaN() )
                                      , QPointF( std::numeric_limits<qreal>::quiet_NaN(), std::numeric_limits<qreal>::quiet_NaN() ) ) )
    , m_forcedXBoundaries( qMakePair( std::numeric_limits<qreal>::quiet_NaN(), std::numeric_limits<qreal>::quiet_NaN() ) )
    , m_forcedYBoundaries( qMakePair( std::numeric_limits<qreal>::quiet_NaN(), std::numeric_limits<qreal>::quiet_NaN() ) )
{

}

inline bool inBoundary( const QPair< qreal, qreal > &bounds, qreal value )
{
    return bounds.first <= value && value <= bounds.second;
}

bool PlotterDiagramCompressor::Private::inBoundaries( Qt::Orientation orient, const PlotterDiagramCompressor::DataPoint &dp ) const
{
    if ( orient == Qt::Vertical && forcedBoundaries( Qt::Vertical ) )
    {
        return inBoundary( m_forcedYBoundaries, dp.value );
    }
    else if ( forcedBoundaries( Qt::Horizontal ) )
    {
        return inBoundary( m_forcedXBoundaries, dp.key );
    }
    return true;
}

// TODO this is not threadsafe do never try to invoke the painting in a different thread than this
// method
void PlotterDiagramCompressor::Private::rowsInserted( const QModelIndex& /*parent*/, int start, int end )
{

    if( m_bufferlist.count() > 0 && !m_bufferlist[ 0 ].isEmpty() && start < m_bufferlist[ 0 ].count() )
    {
        calculateDataBoundaries();
        clearBuffer();
        return;
    }
    // we are handling appends only here, a prepend might be added, insert is expensive if not needed
    for ( int dataset = 0; dataset < m_bufferlist.size(); ++dataset )
    {
        PlotterDiagramCompressor::DataPoint predecessor = m_bufferlist[ dataset ].isEmpty() ? DataPoint() : m_bufferlist[ dataset ].last();

        for ( int row = start; row <= end; ++row )
        {
            PlotterDiagramCompressor::DataPoint curdp = m_parent->data( CachePosition( row, dataset ) );
            const bool checkcur = inBoundaries( Qt::Vertical, curdp ) && inBoundaries( Qt::Horizontal, curdp );
            const bool checkpred = inBoundaries( Qt::Vertical, predecessor ) && inBoundaries( Qt::Horizontal, predecessor );
            const bool check = checkcur || checkpred;
            if ( predecessor.distance( curdp ) > m_mergeRadius && check )
            {
                if ( start > m_bufferlist[ dataset ].count() && !m_bufferlist[ dataset ].isEmpty() )
                {
                    m_bufferlist[ dataset ].append( curdp );
                }
                else if( !m_bufferlist[ dataset ].isEmpty() )
                {
                    m_bufferlist[ dataset ].insert( row, curdp );
                }
                predecessor = curdp;
                qreal minX = qMin( curdp.key, m_boundary.first.x() );
                qreal minY = qMin( curdp.value, m_boundary.first.y() );
                qreal maxX = qMax( curdp.key, m_boundary.second.x() );
                qreal maxY = qMax( curdp.value, m_boundary.second.y() );
                setBoundaries( qMakePair( QPointF( minX, minY ), QPointF( maxX, maxY ) ) );
            }
        }
    }
    emit m_parent->rowCountChanged();
}

void PlotterDiagramCompressor::Private::setBoundaries( const Boundaries & bound )
{
    if ( bound != m_boundary )
    {
        m_boundary = bound;
        emit m_parent->boundariesChanged();
    }
}

void PlotterDiagramCompressor::Private::calculateDataBoundaries()
{
    if ( !forcedBoundaries( Qt::Vertical) && !forcedBoundaries( Qt::Horizontal ) )
    {
        qreal minX = std::numeric_limits<qreal>::max();
        qreal minY = std::numeric_limits<qreal>::max();
        qreal maxX = std::numeric_limits<qreal>::min();
        qreal maxY = std::numeric_limits<qreal>::min();
        for ( int dataset = 0; dataset < m_parent->datasetCount(); ++dataset )
        {
            for ( int row = 0; row < m_parent->rowCount(); ++ row )
            {
                PlotterDiagramCompressor::DataPoint dp = m_parent->data( CachePosition( row, dataset ) );
                minX = qMin( minX, dp.key );
                maxX = qMax( maxX, dp.key );
                minY = qMin( minY, dp.value );
                maxY = qMax( maxY, dp.value );
            }
        }
        if ( forcedBoundaries( Qt::Vertical ) )
        {
            minY = m_forcedYBoundaries.first;
            maxY = m_forcedYBoundaries.second;
        }
        if ( forcedBoundaries( Qt::Horizontal ) )
        {
            minX = m_forcedXBoundaries.first;
            maxX = m_forcedXBoundaries.second;
        }
        setBoundaries( qMakePair( QPointF( minX, minY ), QPointF( maxX, maxY ) ) );
    }
}

QModelIndexList PlotterDiagramCompressor::Private::mapToModel( const CachePosition &pos )
{
    QModelIndexList indexes;
    QModelIndex index;
    index = m_model->index( pos.first, pos.second * 2, QModelIndex() );
    Q_ASSERT( index.isValid() );
    indexes << index;
    index = m_model->index( pos.first, pos.second * 2 + 1, QModelIndex() );
    Q_ASSERT( index.isValid() );
    indexes << index;
    return indexes;
}

bool PlotterDiagramCompressor::Private::forcedBoundaries( Qt::Orientation orient ) const
{
    if ( orient == Qt::Vertical )
        return !std::isnan( m_forcedYBoundaries.first ) && !std::isnan( m_forcedYBoundaries.second );
    else
        return !std::isnan( m_forcedXBoundaries.first ) && !std::isnan( m_forcedXBoundaries.second );
}

void PlotterDiagramCompressor::Private::clearBuffer()
{
    //TODO all iterator have to be invalid after this operation
    m_bufferlist.clear();
    m_bufferlist.resize( m_parent->datasetCount() );
    m_timeOfLastInvalidation = QDateTime::currentDateTime();
}

PlotterDiagramCompressor::PlotterDiagramCompressor(QObject *parent)
    : QObject(parent)
    , d( new Private( this ) )
{
}

void PlotterDiagramCompressor::setForcedDataBoundaries( const QPair< qreal, qreal > &bounds, Qt::Orientation direction )
{
    if ( direction == Qt::Vertical )
    {
        d->m_forcedYBoundaries = bounds;
    }
    else
    {
        d->m_forcedXBoundaries = bounds;
    }
    d->clearBuffer();
    emit boundariesChanged();
}

QAbstractItemModel* PlotterDiagramCompressor::model() const
{
    Q_ASSERT( d );
    return d->m_model;
}

void PlotterDiagramCompressor::setModel( QAbstractItemModel *model )
{
    Q_ASSERT( d );
    if ( d->m_model )
    {
        d->m_model->disconnect( this );
        d->m_model->disconnect( d );
    }
    d->m_model = model;
    if ( d->m_model)
    {
        d->m_bufferlist.resize( datasetCount() );
        d->calculateDataBoundaries();
        connect( d->m_model, SIGNAL( rowsInserted ( QModelIndex, int, int ) ), d, SLOT( rowsInserted( QModelIndex, int, int ) ) );
        connect( d->m_model, SIGNAL( modelReset() ), d, SLOT( clearBuffer() ) );
    }
}

PlotterDiagramCompressor::DataPoint PlotterDiagramCompressor::data( const CachePosition& pos ) const
{
    DataPoint point;
    QModelIndexList indexes = d->mapToModel( pos );
    Q_ASSERT( indexes.count() == 2 );
    QVariant yValue = d->m_model->data( indexes.last() );
    QVariant xValue = d->m_model->data( indexes.first() );
    Q_ASSERT( xValue.isValid() );
    Q_ASSERT( yValue.isValid() );
    bool ok = false;
    point.key = xValue.toDouble( &ok );
    Q_ASSERT( ok );
    ok = false;
    point.value = yValue.toDouble( &ok );    
    Q_ASSERT( ok );
    point.index = indexes.first();
    return point;
}

void PlotterDiagramCompressor::setMergeRadius( qreal radius )
{
    d->m_mergeRadius = radius;
    emit rowCountChanged();
}

void PlotterDiagramCompressor::setMergeRadiusPercentage( qreal radius )
{
    Boundaries bounds = dataBoundaries();
    const qreal width = radius * ( bounds.second.x() - bounds.first.x() );
    const qreal height = radius * ( bounds.second.y() - bounds.first.y() );
    const qreal realRadius = std::sqrt( width * height );
    setMergeRadius( realRadius );
}

int PlotterDiagramCompressor::rowCount() const
{
    return d->m_model ? d->m_model->rowCount() : 0;
}

void PlotterDiagramCompressor::cleanCache()
{
    d->clearBuffer();
}

int PlotterDiagramCompressor::datasetCount() const
{
    if ( d->m_model && d->m_model->columnCount() == 0 )
        return 0;
    return d->m_model ? ( d->m_model->columnCount() + 1 ) / 2  : 0;
}

QPair< QPointF, QPointF > PlotterDiagramCompressor::dataBoundaries() const
{
    Boundaries bounds = d->m_boundary;
    if ( d->forcedBoundaries( Qt::Vertical ) )
    {
        bounds.first.setY( d->m_forcedYBoundaries.first );
        bounds.second.setY( d->m_forcedYBoundaries.second );
    }
    if ( d->forcedBoundaries( Qt::Horizontal ) )
    {
        bounds.first.setX( d->m_forcedXBoundaries.first );
        bounds.second.setX( d->m_forcedXBoundaries.second );
    }
    return bounds;
}

PlotterDiagramCompressor::Iterator PlotterDiagramCompressor::begin( int dataSet )
{
    Q_ASSERT( dataSet >= 0 && dataSet < d->m_bufferlist.count() );
    return Iterator( dataSet, this, d->m_bufferlist[ dataSet ] );
}

PlotterDiagramCompressor::Iterator PlotterDiagramCompressor::end( int dataSet )
{
    Iterator it( dataSet, this );
    it.m_index = -1;
    return it;
}
