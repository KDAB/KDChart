#include <QtTest/QtTest>
#include <QStandardItem>
#include <QStandardItemModel>

#include <KDChartLineDiagramDataCompressor_p.h>

class LineDiagramDataCompressorTests : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
        // make 10 data sets a n elements
        const int ColumnCount = 10;
        const int RowCount = 1000;
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
        QPair<int, int> NullPoint( -1, -1 );
        struct Match {
            QPair<int, int> cachePosition;
            QModelIndex index;
        } matches[] = {
            { QPair<int, int>( 0, 0 ), model.index( 0, 0 ) },
            { QPair<int, int>( 0, 0 ), model.index( 1, 0 ) },
            { QPair<int, int>( 0, 0 ), model.index( 2, 0 ) },
            { QPair<int, int>( 0, 0 ), model.index( 3, 0 ) },
            { QPair<int, int>( 0, 0 ), model.index( 4, 0 ) },
            { QPair<int, int>( 0, 1 ), model.index( 0, 1 ) },
            { QPair<int, int>( 0, 1 ), model.index( 1, 1 ) },
            { QPair<int, int>( 0, 1 ), model.index( 2, 1 ) },
            { QPair<int, int>( 0, 1 ), model.index( 3, 1 ) },
            { QPair<int, int>( 0, 1 ), model.index( 4, 1 ) },
            { QPair<int, int>( 2, 2 ), model.index( 10, 2 ) },
            { QPair<int, int>( 2, 2 ), model.index( 11, 2 ) },
            { QPair<int, int>( 2, 2 ), model.index( 12, 2 ) },
            { QPair<int, int>( 2, 2 ), model.index( 13, 2 ) },
            { QPair<int, int>( 2, 2 ), model.index( 14, 2 ) },
            // the following are outside the model boundary:
            { NullPoint, model.index( 0, 11 ) },
            { NullPoint, model.index( 1, 11 ) },
            { NullPoint, model.index( 2, 11 ) },
            { NullPoint, model.index( 3, 11 ) },
            { NullPoint, model.index( 4, 11) },
            // sentinel
            { QPair<int, int>( 0, 0 ), QModelIndex() }
        };

        for ( int i = 0; matches[i].index.isValid(); ++i ) {
            QCOMPARE( matches[i].cachePosition, compressor.mapToCache( matches[i].index ) );
        }

        QCOMPARE( NullPoint, compressor.mapToCache( QModelIndex() ) );
    }

    void cleanupTestCase()
    {
    }

private:
    KDChart::LineDiagramDataCompressor compressor;
    QStandardItemModel model;
    int width;
    int height;
};

QTEST_MAIN(LineDiagramDataCompressorTests)

#include "LineDiagramDataCompressorTests.moc"
