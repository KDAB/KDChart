#include <QtTest/QtTest>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLegend>
#include <KDChartHeaderFooter>

using namespace KDChart;

class TestChartElementOwnership: public QObject {
  Q_OBJECT
private slots:

  void initTestCase()
  {
      m_chart = new Chart(0);
  }

  void testPlaneOwnership()
  {
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
      CartesianCoordinatePlane *p = new CartesianCoordinatePlane();
      m_chart->addCoordinatePlane( p );
      QCOMPARE( m_chart->coordinatePlanes().size(), 2 );
      m_chart->takeCoordinatePlane( p );
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
      m_chart->addCoordinatePlane( p );
      QCOMPARE( m_chart->coordinatePlanes().size(), 2 );
      delete p;
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
  }

  void testLegendOwnership()
  {
      QCOMPARE( m_chart->legends().size(), 0 );
      Legend * legend = new Legend( m_chart->coordinatePlane()->diagram() );
      m_chart->addLegend( legend );
      QCOMPARE( m_chart->legends().size(), 1 );
      m_chart->takeLegend( legend );
      QCOMPARE( m_chart->legends().size(), 0 );
      m_chart->addLegend( legend );
      QCOMPARE( m_chart->legends().size(), 1 );
      delete legend;
      QCOMPARE( m_chart->legends().size(), 0 );
  }

  void testHeaderFooterOwnership()
  {
      QCOMPARE( m_chart->headerFooters().size(), 0 );
      HeaderFooter * h = new HeaderFooter();
      m_chart->addHeaderFooter( h );
      QCOMPARE( m_chart->headerFooters().size(), 1 );
      m_chart->takeHeaderFooter( h );
      QCOMPARE( m_chart->headerFooters().size(), 0 );
      m_chart->addHeaderFooter( h );
      QCOMPARE( m_chart->headerFooters().size(), 1 );
      delete h;
      QCOMPARE( m_chart->headerFooters().size(), 0 );
  }

  
  void cleanupTestCase()
  {
  }

private:
  Chart *m_chart;

};

QTEST_MAIN(TestChartElementOwnership)

#include "main.moc"
