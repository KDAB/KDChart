#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>
#include <../../src/KDChartAttributesModel.h> /* Bah! */
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartDataValueAttributes>

using namespace KDChart;

class TestKDChartAttributesModel : public QObject {
  Q_OBJECT
private slots:

  void initTestCase()
  {
      TableModel *tableModel = new TableModel( this );
      tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
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
      QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
      DataValueAttributes a = m_bars->dataValueAttributes( idx );
      QCOMPARE( a.isVisible(), false );
      a.setVisible( true );
      m_bars->setDataValueAttributes( 2, a );
      a = m_bars->dataValueAttributes( idx );
      QCOMPARE( a.isVisible(), true );
  }

  void testKDChartAttributesModelTestPrivateModel()
  {
      // Private is now default
      //m_lines->usePrivateAttributes( true );
      // now we should be getting defaults again
      QModelIndex idx = m_bars->model()->index( 0, 2, QModelIndex() );
      DataValueAttributes a = m_lines->dataValueAttributes( idx );
      QCOMPARE( a.isVisible(), false );
      // set the defaults on the other model and make sure they have
      // an effect
      m_bars->setDataValueAttributes( 2, a );
      DataValueAttributes b = m_bars->dataValueAttributes( idx );
      QCOMPARE( b.isVisible(), false );
      // now change a, set it on the lines, check that the bars
      // didn't change
      a.setVisible( true );
      m_lines->setDataValueAttributes( 2, a );
      QCOMPARE( a.isVisible(), true );
      b = m_bars->dataValueAttributes( idx );
      QCOMPARE( b.isVisible(), false );
  }

  void testKDChartAttributesModelTestSharedModel()
  {
      AttributesModel* attrsmodel = m_lines->attributesModel();
      m_bars->setAttributesModel(attrsmodel);
      QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
      DataValueAttributes a = m_lines->dataValueAttributes( idx );
      QCOMPARE( a.isVisible(), true );
  }

  void testKDChartAttributesModelTestSharedFromStart()
  {
      delete m_lines;
      delete m_bars;
      delete m_plane;
      m_plane = new CartesianCoordinatePlane(0);
      m_bars = new BarDiagram( m_plane );
      m_bars->setModel( m_model );
      m_lines = new LineDiagram( m_plane );
      m_lines->setModel( m_model );

      AttributesModel* attrsmodel = new AttributesModel( m_model, m_plane );
      m_lines->setAttributesModel(attrsmodel);
      m_bars->setAttributesModel(attrsmodel);
      
      QModelIndex idx = m_bars->model()->index( 0, 2, QModelIndex() );
      DataValueAttributes a = m_lines->dataValueAttributes( idx );
      DataValueAttributes b = m_bars->dataValueAttributes( idx );
      QCOMPARE( a.isVisible(), false );
      QCOMPARE( b.isVisible(), false );
      a.setVisible( true );
      QCOMPARE( a.isVisible(), true );
      m_lines->setDataValueAttributes( 2, a );
      b = m_bars->dataValueAttributes( idx );
      QCOMPARE( b.isVisible(), true ); // Should be true by sharing
  }

  void testKDChartAttributesModelTestPrivate()
  {
      m_lines->setAttributesModel( new AttributesModel(m_model,m_lines) );
      m_bars->setAttributesModel( new AttributesModel(m_model,m_bars) );
      QModelIndex idx = m_lines->model()->index( 0, 2, QModelIndex() );
      DataValueAttributes a = m_lines->dataValueAttributes( idx );
      QCOMPARE( a.isVisible(), false ); // we got a default model again
      a.setVisible( true );
      m_lines->setDataValueAttributes( 2, a );
      // should now have propagated to the bars
      DataValueAttributes b = m_bars->dataValueAttributes( idx );
      QCOMPARE( b.isVisible(), false ); // No sharing
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
