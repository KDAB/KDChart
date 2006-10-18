#include <QtTest/QtTest>
#include <QStandardItemModel>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLegend>

#include <TableModel.h>

using namespace KDChart;

class TestLegends: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_lines = new LineDiagram();
        m_bars = new BarDiagram();
        m_tableModel = new TableModel( this );
        m_tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        m_lines->setModel( m_tableModel );
        m_bars->setModel( m_tableModel );
        m_chart->coordinatePlane()->replaceDiagram( m_lines );
    }

    void testIntialOwnership()
    {
        Legend* l = new Legend( m_chart );
        m_chart->addLegend( l );
        QCOMPARE( m_chart->legend(), l );
        QCOMPARE( l->diagram(), (AbstractDiagram*)0);
        l->setDiagram( m_lines );
        QCOMPARE( l->diagram(), m_lines );
    }

    void testIntialOwnershipFromCtor()
    {
        Legend* l = new Legend( m_lines, m_chart );
        m_chart->replaceLegend( l );
        QCOMPARE( m_chart->legend(), l );
        QCOMPARE( l->diagram(), m_lines );
    }

    void testReplacing()
    {
        Legend* l = new Legend( m_chart );
        QPointer<Legend> oldLegend = m_chart->legend();
        QCOMPARE( oldLegend->diagram(), m_lines );
        m_chart->replaceLegend( l, oldLegend );
        QVERIFY( oldLegend.isNull() );
        QCOMPARE( l->diagram(), (AbstractDiagram*)0 );
        l->setDiagram( m_lines );
        QCOMPARE( l->diagram(), m_lines );
    }

    void testReferenceArea()
    {
         Legend* l = new Legend( );
         QCOMPARE( l->referenceArea(), ( QWidget* )0 );
         l->setReferenceArea( m_chart );
         QCOMPARE( l->referenceArea(), m_chart );
         Legend* l1 = new Legend( m_chart );
         QCOMPARE( l1->referenceArea(), m_chart );
         Legend* l2 = new Legend( m_lines,  m_chart );
         QCOMPARE( l2->referenceArea(), m_chart );
    }

    void testDiagramOwnership()
    {
        Legend* l = new Legend( m_chart );
        QVERIFY( l->diagrams().size() == 0 );
        l->addDiagram( m_lines );
        QVERIFY( l->diagrams().size() == 1 );
        l->addDiagram( m_bars );
        QVERIFY( l->diagrams().size() == 2 );
        QCOMPARE(l->diagram(),  m_lines );
        l->removeDiagram( m_lines );
        QVERIFY( l->diagrams().size() == 1 );
        QCOMPARE(l->diagram(),  m_bars );
        l->replaceDiagram( m_lines, m_bars );
        QVERIFY( l->diagrams().size() == 1 );
        QCOMPARE(l->diagram(),  m_lines );
    }

    void testLegendSettings()
    {
       Legend* l = new Legend( m_lines,  m_chart );
       QVERIFY( l->position() == Position::NorthEast );
       QVERIFY( l->alignment() == Qt::AlignCenter );
       QVERIFY( l->orientation() == Qt::Vertical );
       QVERIFY( l->showLines() == false );
       QVERIFY( l->titleText() != QString::null );
       QVERIFY( l->spacing() == 1 );
       l->setPosition( Position::North );
       l->setAlignment( Qt::AlignLeft );
       l->setOrientation( Qt::Horizontal );
       l->setShowLines( true );
       l->setTitleText( QString( tr( "Lines" ) ) );
       l->setSpacing( 2 );
       QVERIFY( l->position() == Position::North );
       QVERIFY( l->alignment() == Qt::AlignLeft );
       QVERIFY( l->orientation() == Qt::Horizontal );
       QVERIFY( l->showLines() == true );
       QVERIFY( l->titleText() == QString( tr( "Lines" ) ) );
       QVERIFY( l->spacing() == 2 );
    }


    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    BarDiagram *m_bars;
    LineDiagram *m_lines;
    Legend* m_legend;
    TableModel *m_tableModel;

};

QTEST_MAIN(TestLegends)

#include "main.moc"
