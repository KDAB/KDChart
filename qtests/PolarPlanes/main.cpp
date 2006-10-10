#include <QtTest/QtTest>
#include <QStandardItemModel>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartPolarCoordinatePlane>
#include <KDChartAbstractCoordinatePlane>
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
        m_plane = new PolarCoordinatePlane();
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


    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    AbstractCoordinatePlane *m_plane;
    TableModel *m_tableModel;

};

QTEST_MAIN(TestPlanes)

#include "main.moc"
