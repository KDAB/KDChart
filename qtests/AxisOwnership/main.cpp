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

  void testReplacing()
  {
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      m_chart->coordinatePlane()->takeDiagram( m_bars );
      QVERIFY( m_axis->observedBy( m_lines ) );
      QVERIFY( !m_axis->observedBy( m_bars ) );
      QPointer<LineDiagram> p( m_lines);
      m_chart->coordinatePlane()->replaceDiagram( m_bars, m_lines );
      QVERIFY( !p );
      AbstractDiagram * nullDiagram = 0;
      QCOMPARE( m_axis->diagram(), nullDiagram );
      QVERIFY( !m_axis->observedBy( m_bars ) );
  }

  void testReplacingWithPropagation()
  {
      initTestCase();
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      m_bars->addAxis( m_axis );
      m_chart->coordinatePlane()->takeDiagram( m_bars );
      QCOMPARE( m_axis->diagram(), m_lines );
      QVERIFY( m_axis->observedBy( m_bars ) );
      QPointer<LineDiagram> p( m_lines);
      m_chart->coordinatePlane()->replaceDiagram( m_bars, m_lines );
      QVERIFY( !p );
      QCOMPARE( m_axis->diagram(), m_bars );
      QVERIFY( m_axis->observedBy( m_bars ) );
  }

  void testAxisDeletion()
  {
      initTestCase();
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      CartesianAxisList list = m_lines->axes();
      QVERIFY( !list.isEmpty() );
      delete m_axis;
      list = m_lines->axes();
      QVERIFY( list.isEmpty() );
  }

  void testAxisDeletionAndUnregister()
  {
      initTestCase();
      m_axis = new CartesianAxis;
      m_lines->addAxis( m_axis );
      m_bars->addAxis( m_axis );
      CartesianAxisList list = m_lines->axes();
      QVERIFY( !list.isEmpty() );
      delete m_axis;
      list = m_lines->axes();
      QVERIFY( list.isEmpty() );
      list = m_bars->axes();
      QVERIFY( list.isEmpty() );
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
