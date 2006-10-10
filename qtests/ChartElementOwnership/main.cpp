#include <QtTest/QtTest>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartPolarCoordinatePlane>
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
      // check number plane
      AbstractCoordinatePlane*orig = m_chart->coordinatePlane();
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );

      // add and take
      CartesianCoordinatePlane *p = new CartesianCoordinatePlane();
      m_chart->addCoordinatePlane( p );
      QCOMPARE( m_chart->coordinatePlanes().size(), 2 );
      m_chart->takeCoordinatePlane( orig );
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
      QCOMPARE( m_chart->coordinatePlane(), p );
      m_chart->addCoordinatePlane( orig );
      QCOMPARE( m_chart->coordinatePlanes().size(), 2 );

      // replace abstract by polar
      PolarCoordinatePlane *po = new PolarCoordinatePlane();
      m_chart->replaceCoordinatePlane( po, orig );
      QCOMPARE( m_chart->coordinatePlanes().size(), 2 );
      m_chart->takeCoordinatePlane( p );
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
      QCOMPARE( m_chart->coordinatePlane(), po );
      m_chart->addCoordinatePlane( p );
      QCOMPARE( m_chart->coordinatePlanes().size(), 2 );

      // delete
      delete po;
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
      QCOMPARE( m_chart->coordinatePlane(), p );

      // replace cartesian by polar
      PolarCoordinatePlane*polast = new PolarCoordinatePlane();
      m_chart->replaceCoordinatePlane( polast );
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
      QCOMPARE( m_chart->coordinatePlane(), polast );

      // replace polar by cartesian
      CartesianCoordinatePlane* plast = new CartesianCoordinatePlane();
      m_chart->replaceCoordinatePlane( plast,  polast );
      QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
      QCOMPARE( m_chart->coordinatePlane(), plast );

  }

  void testLegendOwnership()
  {
      // check no legend
      QCOMPARE( m_chart->legends().size(), 0 );

      // check add legend - take legend - delete legend
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

  void testLegendReplace()
    {
      // check add several legends - take legend
      // replace legend - delete legend
      Legend * legend = new Legend( m_chart->coordinatePlane()->diagram() );
      Legend * legend2 = new Legend( m_chart->coordinatePlane()->diagram() );
      m_chart->addLegend( legend );
      QCOMPARE( m_chart->legends().size(), 1 );
      m_chart->addLegend( legend2 );
      QCOMPARE( m_chart->legends().size(), 2 );
      m_chart->takeLegend( legend );
      QCOMPARE( m_chart->legends().size(), 1 );
      m_chart->replaceLegend( legend  );
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

  void testChartDeletion()
  {
      delete m_chart;
  }

  void cleanupTestCase()
  {
  }

private:
  Chart *m_chart;

};

QTEST_MAIN(TestChartElementOwnership)

#include "main.moc"
