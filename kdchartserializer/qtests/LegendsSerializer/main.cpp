#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartBackgroundAttributes>
#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartPosition>

#include <KDChartLegendsSerializer>
#include <KDChartSerializeCollector>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartLegendsSerializer : public QObject {
  Q_OBJECT
private slots:

    void initTestCase()
    {
        mDocstart = "<kdchart:kdchart/>";
        resetDoc();

        m_chart = new Chart(0);
        TableModel *tableModel = new TableModel( this );
        tableModel->loadFromCSV( "../../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        tableModel->setSupplyHeaderData( false );

        m_lines = new LineDiagram();
        m_lines->setModel( tableModel );

        m_legend = new Legend( m_lines, m_chart );
        m_legend->setVisible( true );
        m_legend->setReferenceArea( m_lines );
        m_legend->setPosition( Position::NorthEast );
        m_legend->setAlignment( Qt::AlignCenter );
        m_legend->setShowLines( false );
        m_legend->setTitleText( tr( "Thee Larch" ) );
        m_legend->setOrientation( Qt::Vertical );
        m_chart->addLegend( m_legend );

        MarkerAttributes lma ( m_legend->markerAttributes( 0 ) );
        lma.setMarkerStyle( MarkerAttributes::MarkerDiamond );
        m_legend->setMarkerAttributes( 0,  lma );
        lma.setMarkerStyle( MarkerAttributes::MarkerCircle );
        m_legend->setMarkerAttributes( 1,  lma );

        // Configure Legend Title and labels
        m_legend->setTitleText( "Lines" );
        m_legend->setText( 0,  "Series 1" );
        m_legend->setText( 1,  "Series 2" );
        m_legend->setText( 2,  "Series 3" );

        // adjust the legend item's font:
        TextAttributes lta( m_legend->textAttributes() );
        lta.setPen( QPen( Qt::darkGray ) );
        Measure me( lta.fontSize() );
        me.setValue( me.value() * 1.5 );
        me.setReferenceArea( 0 );
        lta.setFontSize( me );
        m_legend->setTextAttributes(  lta );

        // adjust the legend title's font:
        lta = m_legend->titleTextAttributes();
        lta.setPen( QPen( Qt::darkGray ) );
        me = lta.fontSize();
        me.setValue( me.value() * 1.5 );
        me.setReferenceArea( 0 );
        lta.setFontSize( me );
        m_legend->setTitleTextAttributes(  lta );

        // Configure a pen to surround
        // one of the markers with a border
        QPen markerPen;
        markerPen.setColor(  Qt::darkGray );
        markerPen.setWidth( 2 );
        m_legend->setPen( 1, markerPen );

        // Add a background to your legend ...
        BackgroundAttributes ba;
        ba.setBrush(  Qt::white );
        ba.setVisible( true );
        m_legend->setBackgroundAttributes(  ba );

        // ... and add a frame:
        FrameAttributes fa;
        fa.setPen( markerPen );
        fa.setPadding( 5 );
        fa.setVisible( true );
        m_legend->setFrameAttributes(  fa );
    }

    void testLegend()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        LegendsSerializer::saveLegends(
                mDoc,
                savedElement,
                m_chart->legends(),
                "kdchart:legends" );
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
                            if( tagName2 == "kdchart:legends" ) {
                                QDomNode node3 = ele2.firstChild();
                                while( !node3.isNull() ) {
                                    QDomElement ele3 = node3.toElement();
                                    if( !ele3.isNull() ) { // was really an element
                                        QString tagName3 = ele3.tagName();
                                        if( tagName3 == "kdchart:legend:1" ) {
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

        Legend* parsedLegend=0;
        QVERIFY( LegendsSerializer::parseLegend( parsedElement, parsedLegend ) );
        QVERIFY( m_legend->compare( parsedLegend ) );
    }


    void cleanupTestCase()
    {
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


    Chart *m_chart;
    Legend *m_legend;
    LineDiagram *m_lines;
};

QTEST_MAIN(TestKDChartLegendsSerializer)

#include "main.moc"
