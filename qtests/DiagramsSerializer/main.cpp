#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartBackgroundAttributes>
#include <KDChartCartesianAxis>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartFrameAttributes>
#include <KDChartLineDiagram>

#include <KDChartSerializer>
#include <KDChartCoordPlanesSerializer>
#include <KDChartDiagramsSerializer>
#include <KDChartAttributesModelSerializer>
#include <KDChartSerializeCollector>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartDiagramsSerializer : public QObject {
  Q_OBJECT
private slots:

    void initTestCase()
    {
        mDocstart = "<kdchart:kdchart/>";
        resetDoc();

        m_coordsS = new CoordPlanesSerializer();
        m_diagsS = new DiagramsSerializer( m_coordsS );


        // Note: We do not instantiate a Serializer object, so we
        //       must register the built-in factories explicitely:
        Serializer::registerBuiltInSerializerFactories();


        m_chart = new Chart(0);
        TableModel *tableModel = new TableModel( this );
        tableModel->loadFromCSV( ":/data" );
        tableModel->setSupplyHeaderData( false );

        m_lines = new LineDiagram();
        m_lines->setModel( tableModel );

        CartesianAxis *xAxis = new CartesianAxis( m_lines );
        CartesianAxis *yAxis = new CartesianAxis ( m_lines );
        xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
        yAxis->setPosition ( KDChart::CartesianAxis::Left );
        xAxis->setTitleText ( "Abscissa axis at the bottom" );
        yAxis->setTitleText ( "Ordinate axis at the left side" );
        m_lines->addAxis( xAxis );
        m_lines->addAxis( yAxis );

        DataValueAttributes dva( m_lines->dataValueAttributes() );

        TextAttributes ta( dva.textAttributes() );
        dva.setDecimalDigits( 2 );
        dva.setSuffix( " Ohm" );
        ta.setFont( QFont( "Comic") );
        ta .setPen( QPen( QColor( Qt::darkGreen ) ) );
        ta.setVisible( true );
        Measure me( ta.fontSize() );
        me.setValue( me.value() * 0.25 );
        ta.setFontSize( me );
        dva.setTextAttributes( ta );
        dva.setVisible( true );
        m_lines->setDataValueAttributes( dva );
        for( int i=0; i<tableModel->columnCount(); ++i ){
            QPen pen( m_lines->pen( i ) );
            pen.setWidth( 17 );
            m_lines->setPen( i, pen );
        }
        QPen linePen( m_lines->pen( 1 ) );
        linePen.setColor( Qt::yellow );
        linePen.setWidth( 7 );
        linePen.setStyle( Qt::DashLine );
        m_lines->setPen( tableModel->index( 1, 1, QModelIndex() ), linePen );
        m_chart->coordinatePlane()->replaceDiagram( m_lines );

        mAttrModelS = new AttributesModelSerializer();
    }

    void testDiagram()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        qDebug() << "\n\n  NOTE : Please ignore the comments following below   --- CoordPlanesSerializer ---\n";
        m_diagsS->saveDiagrams(
                mDoc,
                savedElement,
                const_cast<const AbstractCoordinatePlane*>(m_chart->coordinatePlane())->diagrams(),
                "kdchart:diagrams" );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";


        // the created tree should look like this:
        // <kdchart:kdchart>
        //   <TESTING>
        //     <kdchart:diagrams:pointers>
        //        <kdchart:pointer name="kdchart:diagram:1" />
        //     </kdchart:diagrams:pointers>
        //   </TESTING>
        //   <kdchart:global-objects>
        //     ..
        //   </kdchart:global-objects>
        // </kdchart:kdchart>

        // prepare parsing
        QVERIFY( SerializeCollector::initializeGlobalPointers( mDocRoot ) );


        AbstractDiagram* parsedDiagram=0;
        QVERIFY( m_diagsS->parseDiagram(
                 mDocRoot,
                 // first child is the ptr-list
                 // the list's first node is our diagram-ptr
                 savedElement.firstChild().firstChild(),
                 parsedDiagram ) );
        LineDiagram* parsedLineDiagram = dynamic_cast<LineDiagram*>(parsedDiagram);
        QVERIFY( parsedLineDiagram != 0 );
        QVERIFY( m_lines->compare( parsedLineDiagram ) );
    }


    void cleanupTestCase()
    {
        delete m_diagsS;
        delete m_chart;
    }

private:
    void resetDoc()
    {
        mDoc = QDomDocument( "KDChart" );
        mDoc.setContent( mDocstart );
        mDocRoot = mDoc.documentElement();
    }

    QString mDocstart;
    QDomDocument mDoc;
    QDomElement mDocRoot;

    DiagramsSerializer *m_diagsS;
    CoordPlanesSerializer *m_coordsS;
    AttributesModelSerializer *mAttrModelS;

    Chart *m_chart;
    LineDiagram *m_lines;
};

QTEST_MAIN(TestKDChartDiagramsSerializer)

#include "main.moc"
