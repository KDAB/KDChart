#include <QtTest/QtTest>
#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartBarDiagram>
#include <KDChartCartesianCoordinatePlane>

#include <TableModel.h>

using namespace KDChart;

class TestBarDiagrams: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_model = new TableModel( this );
        m_model->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        m_bars = new BarDiagram();
        m_bars->setModel( m_model );
        m_chart->coordinatePlane()->replaceDiagram( m_bars );
    }

    void testBarDiagramType()
    {
        QCOMPARE( m_bars->type(), BarDiagram::Normal );
        m_bars->setType( BarDiagram::Stacked );
        QCOMPARE(  m_bars->type(),  BarDiagram::Stacked );
        m_bars->setType( BarDiagram::Percent );
        QCOMPARE(  m_bars->type(),  BarDiagram::Percent );
        m_bars->setType( BarDiagram::Rows );
        QCOMPARE(  m_bars->type(),  BarDiagram::Rows );
        //reset to normal
        m_bars->setType( BarDiagram::Normal );
    }

    void testBarAttributesSettings()
    {
        //check segments
        const int rows = m_bars->model()->rowCount();
        QCOMPARE( m_bars->numberOfAbscissaSegments(), rows );
        const int cols = m_bars->model()->columnCount();
        QCOMPARE( m_bars->numberOfOrdinateSegments(), cols );
        QModelIndex idx = m_model->index(rows-3, cols-3, QModelIndex());
        // create attribut
        BarAttributes ba( m_bars->barAttributes() );
        BarAttributes baCol( m_bars->barAttributes() );
        BarAttributes baIndex( m_bars->barAttributes() );
        // modify at different level and compare
        baCol.setBarGapFactor(2 );
        baIndex.setBarGapFactor( 3 );
        m_bars->setBarAttributes( ba  );
        m_bars->setBarAttributes( cols-2,  baCol );
        m_bars->setBarAttributes( idx,  baIndex );
        QVERIFY( m_bars->barAttributes() != m_bars->barAttributes(cols-2) );
        QVERIFY( m_bars->barAttributes() != m_bars->barAttributes(idx) );
        QVERIFY( m_bars->barAttributes(cols-2) != m_bars->barAttributes(idx) );
        QCOMPARE( m_bars->barAttributes(),  ba );
        QCOMPARE( m_bars->barAttributes( cols - 2 ),  baCol );
        QCOMPARE( m_bars->barAttributes( idx ),  baIndex );
    }


    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    BarDiagram *m_bars;
    TableModel *m_model;

};

QTEST_MAIN(TestBarDiagrams)

#include "main.moc"
