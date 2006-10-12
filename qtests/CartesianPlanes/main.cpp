#include <QtTest/QtTest>
#include <QStandardItemModel>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartLegend>

#include <TableModel.h>

using namespace KDChart;

class TestPlanes: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_tableModel = new TableModel( this );
        m_tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        m_bars = new BarDiagram();
        m_bars->setModel( m_tableModel );
        m_lines = new LineDiagram();
        m_lines->setModel( m_tableModel );
        m_plane = new CartesianCoordinatePlane();
        m_chart->addCoordinatePlane( m_plane );
        m_plane->setReferenceCoordinatePlane( m_chart->coordinatePlane() );
    }

    void testIntialOwnership()
    {
        AbstractCoordinatePlane *plane = m_chart->coordinatePlane();
        QCOMPARE( m_plane->referenceCoordinatePlane(), m_chart->coordinatePlane() );
        m_chart->takeCoordinatePlane( 0 );
        delete plane;
        QCOMPARE( m_plane->referenceCoordinatePlane(), (AbstractCoordinatePlane*)0 );
    }

    void testDiagramOwnership()
    {
        m_plane->addDiagram(  m_bars );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        m_plane->addDiagram(  m_lines );
        QCOMPARE( m_plane->diagrams().size(),  2 );
        QCOMPARE( m_plane->diagram(),  m_bars );
        m_plane->takeDiagram( m_bars );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( m_plane->diagram(),  m_lines );
        m_plane->replaceDiagram( m_bars,  m_lines );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( m_plane->diagram(),  m_bars );
        m_plane->takeDiagram( m_bars );
        QCOMPARE( m_plane->diagrams().size(),  0 );
        delete m_bars;
    }


    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    BarDiagram *m_bars;
    LineDiagram *m_lines;
    AbstractCoordinatePlane *m_plane;
    TableModel *m_tableModel;

};

QTEST_MAIN(TestPlanes)

#include "main.moc"
