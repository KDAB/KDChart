#include <QtTest/QtTest>
#include <QStandardItemModel>
#include <QPointF>
#include <QString>
#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
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

    void testZoomFactorsSettings()
    {
        QCOMPARE( m_plane->zoomFactorX(),  1.0 );
        QCOMPARE( m_plane->zoomFactorY(),  1.0 );
        qDebug() << m_plane->zoomCenter();
        QCOMPARE( m_plane->zoomCenter(), QPointF( 0.5, 0.5 ) );
        m_plane->setZoomFactorX( 1.5 );
        m_plane->setZoomFactorY( 1.5 );
        m_plane->setZoomCenter( QPointF ( 1.0, 1.0 ) );
        QCOMPARE( m_plane->zoomFactorX(),  1.5 );
        QCOMPARE( m_plane->zoomFactorY(),  1.5 );
        QCOMPARE( m_plane->zoomCenter(),  QPointF( 1.0, 1.0 ) );
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
