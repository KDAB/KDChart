#include <QtDebug>
#include <QAbstractItemModel>

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartCartesianDiagramDataCompressor_p.h"

using namespace KDChart;

CartesianDiagramDataCompressor::CartesianDiagramDataCompressor( QObject* parent )
    : QObject( parent )
    , m_mode( Precise )
    , m_xResolution( 0 )
    , m_yResolution( 0 )
    , m_sampleStep( 0 )
    , m_datasetDimension( 1 )
{
    calculateSampleStepWidth();
}

void CartesianDiagramDataCompressor::slotModelDataChanged(
    const QModelIndex& topLeftIndex,
    const QModelIndex& bottomRightIndex )
{
    Q_ASSERT( topLeftIndex.row() <= bottomRightIndex.row() );
    Q_ASSERT( topLeftIndex.column() <= bottomRightIndex.column() );
    CachePosition topleft = mapToCache( topLeftIndex );
    CachePosition bottomright = mapToCache( bottomRightIndex );
    for ( int row = topleft.first; row <= bottomright.first; ++row )
        for ( int column = topleft.second; column <= bottomright.second; ++column )
            invalidate( CachePosition( row, column ) );
}

void CartesianDiagramDataCompressor::slotModelLayoutChanged()
{
    rebuildCache();
    calculateSampleStepWidth();
}

void CartesianDiagramDataCompressor::slotDiagramLayoutChanged( AbstractDiagram* diagramBase )
{
    AbstractCartesianDiagram* diagram = qobject_cast< AbstractCartesianDiagram* >( diagramBase );
    Q_ASSERT( diagram );
    if ( diagram->datasetDimension() != m_datasetDimension ) {
        setDatasetDimension( diagram->datasetDimension() );
    }
}

int CartesianDiagramDataCompressor::modelDataColumns() const
{
    Q_ASSERT( m_datasetDimension != 0 );
    // only operational if there is a model and a resolution
    if ( m_model ) {
        const int columns = m_model->columnCount( m_rootIndex ) / m_datasetDimension;

        if( columns != m_data.size() )
        {
            rebuildCache();
        }

        Q_ASSERT( columns == m_data.size() );
        return columns;
    } else {
        return 0;
    }
}

int CartesianDiagramDataCompressor::modelDataRows() const
{
    // only operational if there is a model, columns, and a resolution
    if ( m_model && m_model->columnCount( m_rootIndex ) > 0 && m_xResolution > 0 ) {
        return m_data[0].size();
    } else {
        return 0;
    }
}

void CartesianDiagramDataCompressor::setModel( QAbstractItemModel* model )
{
    if ( m_model != 0 && m_model != model ) {
        disconnect( m_model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),
                    this, SLOT( slotModelDataChanged( QModelIndex, QModelIndex ) ) );
        disconnect( m_model, SIGNAL( layoutChanged() ),
                    this, SLOT( slotModelLayoutChanged() ) );
        m_model = 0;
    }

    if ( model != 0 ) {
        m_model = model;
        connect( m_model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),
                 SLOT( slotModelDataChanged( QModelIndex, QModelIndex ) ) );
        connect( m_model, SIGNAL( layoutChanged() ),
                 SLOT( slotModelLayoutChanged() ) );
    }

    rebuildCache();
    calculateSampleStepWidth();
}

void CartesianDiagramDataCompressor::setRootIndex( const QModelIndex& root )
{
    if ( m_rootIndex != root ) {
        m_rootIndex = root;
        rebuildCache();
        calculateSampleStepWidth();
    }
}
void CartesianDiagramDataCompressor::setResolution( int x, int y )
{
    if ( x != m_xResolution || y != m_yResolution ) {
        m_xResolution = x;
        m_yResolution = y;
        rebuildCache();
        calculateSampleStepWidth();
    }
}

void CartesianDiagramDataCompressor::clearCache()
{
    for ( int column = 0; column < m_data.size(); ++column )
        m_data[column].fill( DataPoint() );
}

void CartesianDiagramDataCompressor::rebuildCache() const
{
    Q_ASSERT( m_datasetDimension != 0 );

    m_data.clear();
    const int columnCount = m_model ? m_model->columnCount( m_rootIndex ) / m_datasetDimension : 0;
    const int rowCount = qMin( m_model ? m_model->rowCount( m_rootIndex ) : 0, m_xResolution );
    m_data.resize( columnCount );
    for ( int i = 0; i < columnCount; ++i ) {
        m_data[i].resize( rowCount );
        m_data[i].fill( DataPoint() );
    }
}

const CartesianDiagramDataCompressor::DataPoint& CartesianDiagramDataCompressor::data( const CachePosition& position ) const
{
    static DataPoint NullDataPoint;
    if ( ! isValidCachePosition( position ) ) return NullDataPoint;
    if ( ! isCached( position ) ) retrieveModelData( position );
    return m_data[position.second][position.first];
}

void CartesianDiagramDataCompressor::retrieveModelData( const CachePosition& position ) const
{
    Q_ASSERT( isValidCachePosition( position ) );
    DataPoint result;

    switch(m_mode ) {
    case Precise:
    {
        result.hidden = true;
        QModelIndexList indexes = mapToModel( position );
        if ( ! indexes.isEmpty() ) {
            Q_FOREACH( QModelIndex index, indexes ) {
                bool ok;
                QVariant valueVariant = m_model->data( index, Qt::DisplayRole );
                double value = valueVariant.toDouble( &ok );
                if ( ok ) result.value += value;
                // the point is visible if any of the points at this pixel position is visible
                if ( qVariantValue<bool>( m_model->data( index, DataHiddenRole ) ) == false ) {
                    result.hidden = false;
                }
            }
            result.index = indexes.at( 0 );
            result.value /= indexes.size();
        }
    }
    break;
    case SamplingSeven:
    default:
    {
    }
    break;
    };

    m_data[position.second][position.first] = result;
    Q_ASSERT( isCached( position ) );
}

CartesianDiagramDataCompressor::CachePosition CartesianDiagramDataCompressor::mapToCache(
    const QModelIndex& index ) const
{
    Q_ASSERT( m_datasetDimension != 0 );

    static const CachePosition NullPosition( -1, -1 );
    if ( ! index.isValid() ) return NullPosition;
    if ( m_data.size() == 0 || m_data[0].size() == 0 ) return NullPosition;
    // assumption: indexes per column == 1
    if ( indexesPerPixel() == 0 ) return NullPosition;
    return CachePosition( index.row() / indexesPerPixel(), index.column() / m_datasetDimension );
}

QModelIndexList CartesianDiagramDataCompressor::mapToModel( const CachePosition& position ) const
{
    if ( isValidCachePosition( position ) ) {
        // assumption: indexes per column == 1
        QModelIndexList indexes;
        for ( int i = 0; i < indexesPerPixel(); ++i ) {
            indexes << m_model->index( position.first * indexesPerPixel(), position.second * m_datasetDimension, m_rootIndex );
        }
        return indexes;
    } else {
        return QModelIndexList();
    }
}

int CartesianDiagramDataCompressor::indexesPerPixel() const
{
    if ( m_data.size() == 0 ) return 0;
    if ( m_data[0].size() == 0 ) return 0;
    if ( ! m_model ) return 0;
    return m_model->rowCount( m_rootIndex ) / m_data[0].size();
}

bool CartesianDiagramDataCompressor::isValidCachePosition( const CachePosition& position ) const
{
    if ( ! m_model ) return false;
    if ( m_data.size() == 0 || m_data[0].size() == 0 ) return false;
    if ( position.second < 0 || position.second >= m_data.size() ) return false;
    if ( position.first < 0 || position.first >= m_data[0].size() ) return false;
    return true;
}

void CartesianDiagramDataCompressor::invalidate( const CachePosition& position )
{
    if ( isValidCachePosition( position ) )
        m_data[position.second][position.first] = DataPoint();
}

bool CartesianDiagramDataCompressor::isCached( const CachePosition& position ) const
{
    Q_ASSERT( isValidCachePosition( position ) );
    return m_data[position.second][position.first].index.isValid();
}

void CartesianDiagramDataCompressor::calculateSampleStepWidth()
{
    if ( m_mode == Precise ) {
        m_sampleStep = 1;
        return;
    }

    static unsigned int SomePrimes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
        53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101,
        151, 211, 313, 401, 503, 607, 701, 811, 911, 1009,
        10037, 12911, 16001, 20011, 50021,
        100003, 137867, 199999, 500009, 707753, 1000003, 0
    }; // ... after that, having a model at all becomes impractical

    // we want at least 17 samples per data point, using a prime step width
    const double WantedSamples = 17;
    if ( WantedSamples > indexesPerPixel() ) {
        m_sampleStep = 1;
    } else {
        int i;
        for ( i = 0; SomePrimes[i] != 0; ++i ) {
            if ( WantedSamples * SomePrimes[i+1] > indexesPerPixel() ) {
                break;
            }
        }
        m_sampleStep = SomePrimes[i];
        if ( SomePrimes[i] == 0 ) {
            m_sampleStep = SomePrimes[i-1];
        } else {
            m_sampleStep = SomePrimes[i];
        }
    }
}

void CartesianDiagramDataCompressor::setDatasetDimension( int dimension )
{
    if ( dimension != m_datasetDimension ) {
        m_datasetDimension = dimension;
        rebuildCache();
        calculateSampleStepWidth();
    }
}