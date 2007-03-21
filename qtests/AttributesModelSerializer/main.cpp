#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>
#include <KDChartAttributesModel>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartDataValueAttributes>

#include <KDChartAttributesModelSerializer>

using namespace KDChart;

class TestKDChartAttributesModelSerializer : public QObject {
  Q_OBJECT
private slots:

    void initTestCase()
    {
        mDocstart = "<kdchart:kdchart/>";
        mDoc = QDomDocument( "KDChart" );
        mDoc.setContent( mDocstart );
        mDocRoot = mDoc.documentElement();

        m_chart = new Chart(0);
        TableModel *tableModel = new TableModel( this );
        tableModel->loadFromCSV( "../../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        tableModel->setSupplyHeaderData( false );
        m_model = tableModel;
        m_plane = new CartesianCoordinatePlane(0);

        m_bars = new BarDiagram();
        m_bars->setModel( m_model );
    }

    void testTextAttributes()
    {
        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        DataValueAttributes da = m_bars->dataValueAttributes( idx );

        QDomElement savedElement =
                mDoc.createElement( "kdchart:serializer-test:AttributesModel" );
        mDocRoot.appendChild( savedElement );

        TextAttributes orgAttrs = da.textAttributes();

        AttributesSerializer::saveTextAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestTextAttributes" );

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        TextAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseTextAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
    }


  void testChartDeletion()
  {
      delete m_chart;
  }

  void cleanupTestCase()
  {
      delete m_plane;
  }

private:
    QString mDocstart;
    QDomDocument mDoc;
    QDomElement mDocRoot;

    Chart *m_chart;
    QAbstractItemModel *m_model;
    CartesianCoordinatePlane* m_plane;
    BarDiagram *m_bars;

};

QTEST_MAIN(TestKDChartAttributesModelSerializer)

#include "main.moc"
