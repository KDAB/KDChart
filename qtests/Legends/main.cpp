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
      m_tableModel = new TableModel( this );
      m_tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
      m_lines->setModel( m_tableModel );
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
