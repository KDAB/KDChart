#ifndef KDCHARTLINEDIAGRAMDATACOMPRESSOR_H
#define KDCHARTLINEDIAGRAMDATACOMPRESSOR_H

#include <QPair>
#include <QVector>
#include <QObject>
#include <QPointer>
#include <QModelIndex>

class LineDiagramDataCompressorTests;
class QAbstractItemModel;

namespace KDChart {

    // - transparently compress table model data if the diagram widget
    // size does not allow to display all data points in an acceptable way
    // - the class acts much like a proxy model, but is not
    // implemented as one, to avoid performance penalties by QVariant
    // conversions
    // - a wanted side effect is that the compressor will deliver
    // more precise values for more precise media, like paper
    // (a) this is absolutely strictly seriously private API of KDChart
    // (b) if possible, this class is going to be templatized for
    // different diagram types
    class LineDiagramDataCompressor : public QObject
    {
        Q_OBJECT
        friend class ::LineDiagramDataCompressorTests;

    public:
        class DataPoint {
        public:
            DataPoint() : value() {}
            double value;
            QModelIndex index;
        };
        typedef QVector<DataPoint> DataPointVector;
        typedef QPair<int, int> CachePosition;

        enum ApproximationMode {
            // do not approximate, interpolate by averaging all
            // datapoints for a pixel
            Precise,
            // approximate by averaging out over prime number distances
            SamplingSeven
        };

        explicit LineDiagramDataCompressor( QObject* parent = 0 );

        // input: model, chart resolution, approximation mode
        void setModel( QAbstractItemModel* );
        void setRootIndex( const QModelIndex& root );
        void setResolution( int x, int y );
        void setApproximationMode( ApproximationMode mode );

        // output: resulting model resolution, data points
        int modelDataColumns() const;
        int modelDataRows() const;
        const DataPoint& data( const CachePosition& ) const;

    private Q_SLOTS:
        void slotModelDataChanged( const QModelIndex&, const QModelIndex& );

    private:
        // geometry has changed
        void rebuildCache();
        // reset all cached values, without changing the cache geometry
        void clearCache();
        // mark a cache position as invalid
        void invalidate( const CachePosition& );
        // verify it is within the range
        bool isValidCachePosition( const CachePosition& ) const;

        CachePosition mapToCache( const QModelIndex& ) const;
        QModelIndexList mapToModel( const CachePosition& ) const;
        int indexesPerPixel() const;

        // retrieve data from the model, put it into the cache
        void retrieveModelData( const CachePosition& ) const;
        // check if a data point is in the cache:
        bool isCached( const CachePosition& ) const;
        // set sample step width according to settings:
        void calculateSampleStepWidth();

        // one per dataset
        mutable QVector<DataPointVector> m_data;
        ApproximationMode m_mode;
        int m_xResolution;
        int m_yResolution;
        QPointer<QAbstractItemModel> m_model;
        unsigned int m_sampleStep;
        QModelIndex m_rootIndex;
    };
}

#endif

