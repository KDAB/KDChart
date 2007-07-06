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
        const int RowCount = 100 * 1000; // (a whole fricking lot :-)
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
