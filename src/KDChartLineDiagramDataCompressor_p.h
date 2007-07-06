#ifndef KDCHARTLINEDIAGRAMDATACOMPRESSOR_H
#define KDCHARTLINEDIAGRAMDATACOMPRESSOR_H

#include <QVector>
#include <QObject>
#include <QPointer>
#include <QModelIndex>

class QStandardItemModel;

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

    public:
        class DataPoint {
            double value;
            QModelIndex index;
        };
        typedef QVector<DataPoint> DataPointVector;

        enum ApproximationMode {
            // do not approximate
            Precise,
            // approximate by averaging out over prime number distances
            SamplingSeven
        };

        explicit LineDiagramDataCompressor( QObject* parent = 0 );

        // input: model, chart resolution, approximation mode
        void setModel( QStandardItemModel* );
        void setResolution( int x, int y );
        void setApproximationMode( ApproximationMode mode );

        // output: resulting model resolution, data points
        int modelDataColumns() const;
        int modelDataRows() const;
        const DataPoint& data( int row, int column ) const;

    private Q_SLOTS:
        void slotModelDataChanged( const QModelIndex&, const QModelIndex& );

    private:
        void rebuildCache(); // geometry has changed
        void clearCache(); // reset all cached values, without changing the cache geometry
        // one per dataset
        QVector<DataPointVector> m_data;
        int m_xResolution;
        int m_yResolution;
        QPointer<QStandardItemModel> m_model;
    };
}

#endif
