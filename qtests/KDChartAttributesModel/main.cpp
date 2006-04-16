#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal.h>
#include <KDChartBarDiagram.h>
#include <KDChartLineDiagram.h>
#include <KDChartCartesianCoordinatePlane.h>
#include <KDChartDataValueAttributes.h>

using namespace KDChart;

class TestKDChartAttributesModel : public QObject {
  Q_OBJECT
private slots:

  void initTestCase()
  {
      TableModel *tableModel = new TableModel( this );
      tableModel->loadFromCSV( "../tools/modeldata/KDChart-Test-Datatables.csv" );
      tableModel->setSupplyHeaderData( false );
      m_model = tableModel;
      m_plane = new CartesianCoordinatePlane(0);
      m_bars = new BarDiagram( m_plane );
      m_bars->setModel( m_model );
      m_lines = new LineDiagram( m_plane );
      m_lines->setModel( m_model );
  }

  void testKDChartAttributesModelSetSimpleData()
  {
      DataValueAttributes a = m_bars->dataValueAttributes( 2 );
      QCOMPARE( a.isVisible(), false );
      a.setVisible( true );
      m_bars->setDataValueAttributes( 2, a );
      a = m_bars->dataValueAttributes( 2 );
      QCOMPARE( a.isVisible(), true );
  }

  void testKDChartAttributesModelTestSharedModel()
  {
      DataValueAttributes a = m_lines->dataValueAttributes( 2 );
      QCOMPARE( a.isVisible(), true );
  }

  void testKDChartAttributesModelTestPrivateModel()
  {
      m_lines->usePrivateAttributes( true );
      // now we should be getting defaults again
      DataValueAttributes a = m_lines->dataValueAttributes( 2 );
      QCOMPARE( a.isVisible(), false );
      // set the defaults on the other model and make sure they have
      // an effect
      m_bars->setDataValueAttributes( 2, a );
      DataValueAttributes b = m_bars->dataValueAttributes( 2 );
      QCOMPARE( b.isVisible(), false );
      // now change a, set it on the lines, check that the bars
      // didn't change
      a.setVisible( true );
      m_lines->setDataValueAttributes( 2, a );
      QCOMPARE( a.isVisible(), true );
      b = m_bars->dataValueAttributes( 2 );
      QCOMPARE( b.isVisible(), false );
  }

  void testKDChartAttributesModelTestPrivateFromStart()
  {
      delete m_lines;
      delete m_bars;
      delete m_plane;
      m_plane = new CartesianCoordinatePlane(0);
      m_bars = new BarDiagram( m_plane );
      m_bars->usePrivateAttributes( true );
      m_bars->setModel( m_model );
      m_lines = new LineDiagram( m_plane );
      m_lines->usePrivateAttributes( true );
      m_lines->setModel( m_model );

      DataValueAttributes a = m_lines->dataValueAttributes( 2 );
      QCOMPARE( a.isVisible(), false );
      a.setVisible( true );
      m_lines->setDataValueAttributes( 2, a );
      DataValueAttributes b = m_bars->dataValueAttributes( 2 );
      QCOMPARE( a.isVisible(), true );
      QCOMPARE( b.isVisible(), false );
  }

  void testKDChartAttributesModelTestPrivateToShared()
  {
      m_lines->usePrivateAttributes( false );
      m_bars->usePrivateAttributes( false );
      DataValueAttributes a = m_lines->dataValueAttributes( 2 );
      QCOMPARE( a.isVisible(), false ); // we got a default model again
      a.setVisible( true );
      m_lines->setDataValueAttributes( 2, a );
      // should now have propagated to the bars
      DataValueAttributes b = m_bars->dataValueAttributes( 2 );
      QCOMPARE( b.isVisible(), true );
  }


  void cleanupTestCase()
  {
      delete m_plane;
  }

private:
  QAbstractItemModel *m_model;
  CartesianCoordinatePlane* m_plane;
  BarDiagram *m_bars;
  LineDiagram *m_lines;

};

QTEST_MAIN(TestKDChartAttributesModel)

#include "main.moc"
