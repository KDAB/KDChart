#include <QtDebug>
#include <QtTest/QtTest>
#include <QStandardItem>
#include <QStandardItemModel>

#include <KDChartLineDiagramDataCompressor_p.h>

typedef KDChart::LineDiagramDataCompressor::CachePosition CachePosition;

class LineDiagramDataCompressorTests : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
        // make 10 data sets a n elements
        model.clear();
        model.setColumnCount( ColumnCount );
        model.setRowCount( RowCount );
        for ( int row = 0; row < RowCount; ++row )
            for ( int column = 0; column < ColumnCount; ++column ) {
                QStandardItem* item = new QStandardItem();
                item->setData( 1, Qt::DisplayRole );
                model.setItem( row, column, item );
            }

        width = 200;
        height = 100;
        // these settings result in 5 indexes per pixel
    }

    void initializationTest()
    {
        QVERIFY2( compressor.modelDataColumns() == 0, "without a model, columns should be zero" );
        QVERIFY2( compressor.modelDataRows() == 0, "without a model,rows should be zero" );
    }

    void setModelTest()
    {
        compressor.setModel( &model );
        QVERIFY2( compressor.modelDataColumns() == model.columnCount(),
                  "columns should be equal to model columns, independent of resolution" );
        QVERIFY2( compressor.modelDataRows() == 0, "without a diagram resolution, columns should be zero" );
    }

    void setResolutionTest()
    {
        compressor.setResolution( width, height );
        QVERIFY2( compressor.modelDataColumns() == model.columnCount(),
                  "column count should be equal to dataset count when both model and resolution are set" );
        QVERIFY2( compressor.modelDataRows() == width,
                  "row count should be equal to width when both model and resolution are set, and "
                  "model row count exceeds widget width");
    }

    void mapToCacheTest()
    {
        CachePosition NullPoint( -1, -1 );
        struct Match {
            CachePosition cachePosition;
            QModelIndex index;
        } matches[] = {
            { CachePosition( 0, 0 ), model.index( 0, 0 ) },
            { CachePosition( 0, 0 ), model.index( 1, 0 ) },
            { CachePosition( 0, 0 ), model.index( 2, 0 ) },
            { CachePosition( 0, 0 ), model.index( 3, 0 ) },
            { CachePosition( 0, 0 ), model.index( 4, 0 ) },
            { CachePosition( 0, 1 ), model.index( 0, 1 ) },
            { CachePosition( 0, 1 ), model.index( 1, 1 ) },
            { CachePosition( 0, 1 ), model.index( 2, 1 ) },
            { CachePosition( 0, 1 ), model.index( 3, 1 ) },
            { CachePosition( 0, 1 ), model.index( 4, 1 ) },
            { CachePosition( 2, 2 ), model.index( 10, 2 ) },
            { CachePosition( 2, 2 ), model.index( 11, 2 ) },
            { CachePosition( 2, 2 ), model.index( 12, 2 ) },
            { CachePosition( 2, 2 ), model.index( 13, 2 ) },
            { CachePosition( 2, 2 ), model.index( 14, 2 ) },
            // the following are outside the model boundary:
            { NullPoint, model.index( 0, ColumnCount ) },
            { NullPoint, model.index( 1, ColumnCount ) },
            { NullPoint, model.index( 2, ColumnCount ) },
            { NullPoint, model.index( 3, ColumnCount ) },
            { NullPoint, model.index( 4, ColumnCount) },
            { NullPoint, model.index( RowCount, 0 ) },
            // sentinel
            { CachePosition( 0, 0 ), QModelIndex() }
        };

        for ( int i = 0; matches[i].index.isValid(); ++i ) {
            QCOMPARE( matches[i].cachePosition, compressor.mapToCache( matches[i].index ) );
        }

        QCOMPARE( NullPoint, compressor.mapToCache( QModelIndex() ) );
    }

    void mapToModelTest()
    {
        // test 1: valid point:
        {
            QModelIndexList indexes;
            CachePosition point( 0, 0 );
            indexes = compressor.mapToModel( point );
            Q_FOREACH( QModelIndex index, indexes ) {
                QVERIFY2( compressor.mapToCache( index ) == point,
                          "index mapToModel does not map back to the original cache point" );
            }
        }
        // test 2: invalid point:
        {
            QModelIndexList indexes;
            CachePosition point( 0, ColumnCount ); // just outside column count
            indexes = compressor.mapToModel( point );
            QVERIFY2( indexes.isEmpty(),
                      "index list for a point outside the data space should be empty" );
        }
        {
            QModelIndexList indexes;
            CachePosition point( RowCount, 0 ); // just outside row count
            indexes = compressor.mapToModel( point );
            QVERIFY2( indexes.isEmpty(),
                      "index list for a point outside the data space should be empty" );
        }
    }


   void invalidateTest()
    {
        CachePosition position( 0, 0 );
        // KDChart::LineDiagramDataCompressor::DataPoint point;
        QVERIFY( compressor.isValidCachePosition( position ) );
        compressor.retrieveModelData( position );
        QVERIFY2( compressor.isCached( position ),
                  "datapoint should be valid after retrieveModelData( position )" );
    }

    void cleanupTestCase()
    {
    }

private:
    KDChart::LineDiagramDataCompressor compressor;
    QStandardItemModel model;
    static const int RowCount;
    static const int ColumnCount;
    int width;
    int height;
};

const int LineDiagramDataCompressorTests::ColumnCount = 10;
const int LineDiagramDataCompressorTests::RowCount = 1000;


QTEST_MAIN(LineDiagramDataCompressorTests)

#include "LineDiagramDataCompressorTests.moc"
