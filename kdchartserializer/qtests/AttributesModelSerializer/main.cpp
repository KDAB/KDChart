#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartChart>

#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartPieDiagram>

#include <KDChartCartesianCoordinatePlane>
#include <KDChartDataValueAttributes>

#include <KDChartSerializeCollector>
#include <KDChartAttributesSerializer>
#include <KDChartAttributesModelSerializer>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartAttributesModelSerializer : public QObject {
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
        m_model = tableModel;

        m_lines = new LineDiagram();
        m_lines->setModel( m_model );

        DataValueAttributes da( m_lines->dataValueAttributes( 2 ) );
        RelativePosition negPos( da.negativePosition() );
        negPos.setReferenceArea( 0 );
        RelativePosition posPos( da.positivePosition() );
        posPos.setReferenceArea( 0 );
        TextAttributes ta( da.textAttributes() );
        Measure me( ta.fontSize() );
        me.setReferenceArea( 0 );
        ta.setFontSize( me );
        ta.setMinimalFontSize( me );
        MarkerAttributes ma( da.markerAttributes() );
        ma.setMarkerStyle( MarkerAttributes::MarkerRing );

        da.setNegativePosition( negPos );
        da.setPositivePosition( posPos );
        da.setTextAttributes( ta );
        da.setMarkerAttributes( ma );
        m_lines->setDataValueAttributes( 2, da );

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        da = m_lines->dataValueAttributes( idx );
        ma.setMarkerStyle( MarkerAttributes::MarkerFastCross );

        da.setNegativePosition( negPos );
        da.setPositivePosition( posPos );
        da.setTextAttributes( ta );
        da.setMarkerAttributes( ma );
        m_lines->setDataValueAttributes( idx, da );


        DataValueAttributes dva( m_lines->dataValueAttributes() );

        ta = dva.textAttributes();
        dva.setDecimalDigits( 2 );
        dva.setSuffix( " Ohm" );
        ta.setFont( QFont( "Comic") );
        ta .setPen( QPen( QColor( Qt::darkGreen ) ) );
        ta.setVisible( true );
        me = ta.fontSize();
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


        m_attrsModel = m_lines->attributesModel();
        mAttrModelS = new AttributesModelSerializer();
    }

    void testAttributesModel()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        AttributesModel orgAttrsModel(0,0);

        orgAttrsModel.initFrom( m_attrsModel );

        mAttrModelS->saveAttributesModel(
                mDoc,
                savedElement,
                &orgAttrsModel,
                true );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";


        // prepare parsing
        QVERIFY( SerializeCollector::initializeParsedGlobalPointers( mDocRoot ) );


        bool bFoundSavedAttributesModel = false;
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
                            if( tagName2 == "kdchart:attribute-models" ) {
                                QDomNode node3 = ele2.firstChild();
                                while( !node3.isNull() ) {
                                    QDomElement ele3 = node3.toElement();
                                    if( !ele3.isNull() ) { // was really an element
                                        QString tagName3 = ele3.tagName();
                                        if( tagName3 == "kdchart:attribute-model:1" ) {
                                            parsedElement = ele3;
                                            bFoundSavedAttributesModel = true;
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
        QVERIFY( bFoundSavedAttributesModel );

        AttributesModel* parsedAttrsModel=0;
        QVERIFY( mAttrModelS->parseAttributesModel( parsedElement, parsedAttrsModel ) );

        /* for a manual test comparing the original with the parsed node:
        QDomElement savedElement2 =
                mDoc.createElement( "TESTING-2" );
        mDocRoot.appendChild( savedElement2 );
        mAttrModelS->saveAttributesModel(
                mDoc,
                savedElement2,
                parsedAttrsModel );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );
        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";
        */

        QVERIFY( orgAttrsModel.compare( parsedAttrsModel ) );
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
    QAbstractItemModel *m_model;
    AttributesModel *m_attrsModel;
    LineDiagram *m_lines;

    AttributesModelSerializer *mAttrModelS;
};

QTEST_MAIN(TestKDChartAttributesModelSerializer)

#include "main.moc"
