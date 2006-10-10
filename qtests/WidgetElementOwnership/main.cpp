#include <QtTest/QtTest>

#include <KDChartWidget>
#include <KDChartGlobal>
//#include <KDChartBarDiagram>
//#include <KDChartLineDiagram>
//#include <KDChartCartesianCoordinatePlane>
//#include <KDChartPolarCoordinatePlane>
#include <KDChartLegend>
//#include <KDChartHeaderFooter>

using namespace KDChart;

class TestWidgetElementOwnership: public QObject {
  Q_OBJECT
private slots:

  void initTestCase()
  {
      m_widget = new Widget(0);
  }

  void testPlaneOwnership()
  {
      // check plane
      AbstractCoordinatePlane*orig = m_widget->coordinatePlane();
      QCOMPARE( m_widget->coordinatePlane(), orig );
  }
  void testDiagramTypeSubType()
  {
      QCOMPARE( m_widget->type(), Widget::Line );
      QCOMPARE( m_widget->subType(),  Widget::Normal );
      //check type subtype = default
      m_widget->setType( Widget::Bar );
      QCOMPARE( m_widget->type(), Widget::Bar );
      QCOMPARE( m_widget->subType(),  Widget::Normal );
      //check type subtype
      m_widget->setType( Widget::Line,  Widget::Stacked );
      QCOMPARE( m_widget->type(), Widget::Line );
      QCOMPARE( m_widget->subType(),  Widget::Stacked );
      //check subtype
      m_widget->setSubType( Widget::Percent );
      QCOMPARE( m_widget->subType(),  Widget::Percent );
  }

/*
  void testLegendOwnerShip()
  {
      // check no legend
      QCOMPARE( m_widget->allLegends().size(), 0 );
      // check add legend - take legend - delete legend
      m_widget->addLegend( North );
      QCOMPARE( m_widget->allLegends().size(), 1 );

      m_chart->takeLegend( legend );
      QCOMPARE( m_chart->legends().size(), 0 );
      m_chart->addLegend( legend );
      QCOMPARE( m_chart->legends().size(), 1 );
      delete legend;
      QCOMPARE( m_chart->legends().size(), 0 );

  }

  void testLegendReplace()
    {

      // check no legend
      QCOMPARE( m_chart->legends().size(), 0 );
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
      QCOMPARE( m_chart->legend(), legend2 );
      m_chart->replaceLegend( legend, legend2  );
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

  void testHeaderFooterReplace()
  {
      QCOMPARE( m_chart->headerFooters().size(), 0 );
      HeaderFooter * h = new HeaderFooter();
      HeaderFooter * h1 = new HeaderFooter();
      m_chart->addHeaderFooter( h );
      QCOMPARE( m_chart->headerFooters().size(), 1 );
      m_chart->addHeaderFooter( h1 );
      QCOMPARE( m_chart->headerFooters().size(), 2 );
      m_chart->takeHeaderFooter( h );
      QCOMPARE( m_chart->headerFooters().size(), 1 );
      QCOMPARE( m_chart->headerFooter(), h1 );
      m_chart->replaceHeaderFooter( h,  h1 );
      QCOMPARE( m_chart->headerFooters().size(), 1 );
      delete h;
      QCOMPARE( m_chart->headerFooters().size(), 0 );
  }
*/
  void testWidgetDeletion()
  {
      delete m_widget;
  }

  void cleanupTestCase()
  {
  }

private:
  Widget *m_widget;

};

QTEST_MAIN(TestWidgetElementOwnership)

#include "main.moc"
