#include <QtTest/QtTest>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartCartesianAxis>

#include <TableModel.h>

using namespace KDChart;

class TestAxisOwnership: public QObject {
  Q_OBJECT
private slots:

  void initTestCase()
  {
      m_chart = new Chart(0);
      m_bars = new BarDiagram();
      m_lines = new LineDiagram();
      TableModel *tableModel = new TableModel( this );
      tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
      m_bars->setModel( tableModel );
      m_lines->setModel( tableModel );
      m_chart->coordinatePlane()->replaceDiagram( m_bars );
      m_chart->coordinatePlane()->addDiagram( m_lines );
  }

  void testIntialOwnership()
  {
      m_axis = new CartesianAxis( m_bars );
      QCOMPARE( m_axis->diagram(), m_bars );
      QVERIFY( m_axis->observedBy( m_bars ) );
  }

  void testTakingOwnership()
  {
      m_axis = new CartesianAxis();
      m_bars->addAxis( m_axis );
      QCOMPARE( m_axis->diagram(), m_bars );
      QVERIFY( m_axis->observedBy( m_bars ) );
      QVERIFY( !m_axis->observedBy( m_lines ) );
  }

  void testGivingUpOwnership()
  {
      m_axis = new CartesianAxis();
      m_bars->addAxis( m_axis );
      m_lines->addAxis( m_axis );
      QCOMPARE( m_axis->diagram(), m_bars );
      QVERIFY( m_axis->observedBy( m_bars ) );
      QVERIFY( m_axis->observedBy( m_lines ) );
      m_bars->takeAxis( m_axis );
      QCOMPARE( m_axis->diagram(), m_lines );
      QVERIFY( m_axis->observedBy( m_lines ) );
      QVERIFY( !m_axis->observedBy( m_bars ) );
  }

  void cleanupTestCase()
  {
  }

private:
  Chart *m_chart;
  BarDiagram *m_bars;
  LineDiagram *m_lines;
  CartesianAxis *m_axis;

};

QTEST_MAIN(TestAxisOwnership)

#include "main.moc"
