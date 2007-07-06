#include <QtTest/QtTest>
#include <KDChartLineDiagramDataCompressor_p.h>

class LineDiagramDataCompressorTests : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
    }

    void initializationTest()
    {
        QVERIFY2( compressor.modelDataColumns() == 0, "without a model, columns should be zero" );
        QVERIFY2( compressor.modelDataRows() == 0, "without a model,rows should be zero" );
    }

    void cleanupTestCase()
    {
    }

private:
    KDChart::LineDiagramDataCompressor compressor;
};

QTEST_MAIN(LineDiagramDataCompressorTests)

#include "LineDiagramDataCompressorTests.moc"
