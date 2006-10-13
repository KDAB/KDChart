#include <QtTest/QtTest>
#include <QStandardItemModel>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartPolarCoordinatePlane>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLegend>

#include <TableModel.h>

using namespace KDChart;

#if QT_VERSION < 0x040200
namespace QTest{
template <> inline char *toString(const QPointF &p) {
    return qstrdup(QString::fromLatin1("QPointF(%1,%2)").arg(p.x()).arg(p.y()).toLatin1().constData());
 }
}
#endif

class TestPolarPlanes: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_tableModel = new TableModel( this );
        m_tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        m_pie = new PieDiagram();
        m_pie->setModel( m_tableModel );
        m_polar = new PolarDiagram();
        m_polar->setModel( m_tableModel );
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

    void testDiagramOwnership()
    {
        m_plane->addDiagram(  m_pie );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        m_plane->addDiagram(  m_polar );
        QCOMPARE( m_plane->diagrams().size(),  2 );
        QCOMPARE( m_plane->diagram(),  m_pie );
        m_plane->takeDiagram( m_pie );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( m_plane->diagram(),  m_polar );
        m_plane->replaceDiagram( m_pie,  m_polar );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( m_plane->diagram(),  m_pie );
        m_plane->takeDiagram( m_pie );
        QCOMPARE( m_plane->diagrams().size(),  0 );
        delete m_pie;
    }


    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    PieDiagram *m_pie;
    PolarDiagram *m_polar;
    PolarCoordinatePlane *m_plane;
    TableModel *m_tableModel;

};

QTEST_MAIN(TestPolarPlanes)

#include "main.moc"
