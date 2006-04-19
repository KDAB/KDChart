#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartDefaultsModel.h>
#include <KDChartGlobal.h>

using namespace KDChart;

class TestKDChartDefaultsModel : public QObject {
  Q_OBJECT
private slots:

  void initTestCase()
  {
      TableModel *tableModel = new TableModel( this );
      tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
      tableModel->setSupplyHeaderData( false );

      DefaultsModel *defaultsModel = new DefaultsModel( this );
      defaultsModel->setSourceModel( tableModel );

      m_model = defaultsModel;
  }

  void testKDChartDefaultsModelData()
  {
      QVariant value = m_model->data( m_model->index( 0, 0, QModelIndex() ) );
      QCOMPARE( value.isValid(), true );
  }

  void testKDChartDefaultsModelHeaderDataGeneral()
  {
      QVariant value = m_model->headerData( 2, Qt::Vertical );
      QCOMPARE( value.isValid(), true );
      QCOMPARE( value.toString(), QString("Series 2") );
      value = m_model->headerData( 2, Qt::Horizontal );
      QCOMPARE( value.isValid(), true );
      QCOMPARE( value.toString(), QString("Item 2") );
  }

  void testKDChartDefaultsModelHeaderDataDatasetColor()
  {
      QVariant value = m_model->headerData( 0, Qt::Vertical, DatasetBrushRole );
      QCOMPARE( value.isValid(), true );
      QBrush brush = qVariantValue<QBrush>( value );
      bool isItTheSame = brush == Qt::red;
      QCOMPARE( isItTheSame, true );
  }

private:
  QAbstractItemModel *m_model;

};

QTEST_MAIN(TestKDChartDefaultsModel)

#include "main.moc"
