#ifndef KDCHARTCARTESIANDIAGRAMDATACOMPRESSOR_H
#define KDCHARTCARTESIANDIAGRAMDATACOMPRESSOR_H

#include <QPair>
#include <QVector>
#include <QObject>
#include <QPointer>
#include <QModelIndex>

#include "kdchart_export.h"

class CartesianDiagramDataCompressorTests;
class QAbstractItemModel;

namespace KDChart {

    class AbstractDiagram;

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

    // KDCHART_EXPORT is needed as long there's a test using
    // this class directly
    class KDCHART_EXPORT CartesianDiagramDataCompressor : public QObject
    {
        Q_OBJECT
        friend class ::CartesianDiagramDataCompressorTests;

    public:
        class DataPoint {
        public:
            DataPoint() : value(), hidden( false ) {}
            double value;
            bool hidden;
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

        explicit CartesianDiagramDataCompressor( QObject* parent = 0 );

        // input: model, chart resolution, approximation mode
        void setModel( QAbstractItemModel* );
        void setRootIndex( const QModelIndex& root );
        void setResolution( int x, int y );
        void setApproximationMode( ApproximationMode mode );
        void setDatasetDimension( int dimension );

        // output: resulting model resolution, data points
        // FIXME (Mirko) rather stupid naming, Mirko!
        int modelDataColumns() const;
        int modelDataRows() const;
        const DataPoint& data( const CachePosition& ) const;

    private Q_SLOTS:
        void slotRowsInserted( const QModelIndex&, int, int );
        void slotRowsRemoved( const QModelIndex&, int, int );

        void slotColumnsInserted( const QModelIndex&, int, int );
        void slotColumnsRemoved( const QModelIndex&, int, int );

        void slotModelHeaderDataChanged( Qt::Orientation, int, int );
        void slotModelDataChanged( const QModelIndex&, const QModelIndex& );
        void slotModelLayoutChanged();
        // FIXME resolution changes and root index changes should all
        // be catchable with this method:
        void slotDiagramLayoutChanged( AbstractDiagram* );

    private:
        // geometry has changed
        void rebuildCache() const;
        // reset all cached values, without changing the cache geometry
        void clearCache();
        // mark a cache position as invalid
        void invalidate( const CachePosition& );
        // verify it is within the range
        bool isValidCachePosition( const CachePosition& ) const;

        CachePosition mapToCache( const QModelIndex& ) const;
        CachePosition mapToCache( int row, int column ) const;
        QModelIndexList mapToModel( const CachePosition& ) const;
        qreal indexesPerPixel() const;

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
        int m_datasetDimension;
    };
}

#endif
