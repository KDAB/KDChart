#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartBackgroundAttributes>
#include <KDChartCartesianAxis>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartFrameAttributes>
#include <KDChartLineDiagram>

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

        m_diagsS = new DiagramsSerializer(0);

        m_chart = new Chart(0);
        TableModel *tableModel = new TableModel( this );
        tableModel->loadFromCSV( "../../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
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

        m_diagsS->saveDiagrams(
                mDoc,
                savedElement,
                const_cast<const AbstractCoordinatePlane*>(m_chart->coordinatePlane())->diagrams(),
                "kdchart:diagrams" );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";


        // prepare parsing
        QVERIFY( SerializeCollector::initializeParsedGlobalPointers( mDocRoot ) );


        bool bFoundSavedLegend = false;
        QDomElement parsedElement;
        QDomNode node = mDocRoot.firstChild();
        while( !node.isNull() ) {
            QDomElement element = node.toElement();
            if( !element.isNull() ) { // was really an element
                QString tagName = element.tagName();
                if( tagName == "kdchart:global-objects" ) {
                    QDomNode node2 = element.firstChild();
                    while( !node2.isNull() ) {
                        QDomElement ele2 = node2.toElement();
                        if( !ele2.isNull() ) { // was really an element
                            QString tagName2 = ele2.tagName();
                            if( tagName2 == "kdchart:diagrams" ) {
                                QDomNode node3 = ele2.firstChild();
                                while( !node3.isNull() ) {
                                    QDomElement ele3 = node3.toElement();
                                    if( !ele3.isNull() ) { // was really an element
                                        QString tagName3 = ele3.tagName();
                                        if( tagName3 == "kdchart:diagram:1" ) {
                                            parsedElement = ele3;
                                            bFoundSavedLegend = true;
                                        }
                                    }
                                    node3 = node3.nextSibling();
                                }
                            }
                        }
                        node2 = node2.nextSibling();
                    }
                }
            }
            node = node.nextSibling();
        }
        QVERIFY( bFoundSavedLegend );

        LineDiagram* parsedDiagram=0;
        QVERIFY( m_diagsS->parseLineDiagram( parsedElement, parsedDiagram ) );
        QVERIFY( m_lines->compare( parsedDiagram ) );
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
    AttributesModelSerializer *mAttrModelS;

    Chart *m_chart;
    LineDiagram *m_lines;
};

QTEST_MAIN(TestKDChartDiagramsSerializer)

#include "main.moc"
