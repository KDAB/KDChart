#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartHeaderFooter>

#include <KDChartTextAreaSerializer>
#include <KDChartSerializeCollector>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartTextAreaSerializer : public QObject {
  Q_OBJECT
private slots:

    void initTestCase()
    {
        mDocstart = "<kdchart:kdchart/>";
        resetDoc();
    }

    void testHeaderFooter()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        HeaderFooter orgFooter( 0 );
        orgFooter.setType( HeaderFooter::Footer );
        orgFooter.setPosition( Position::SouthEast );
        orgFooter.setText( "Nix Nought Nothing" );

        HeaderFooterList areas;
        areas.append( &orgFooter );

        TextAreaSerializer::saveHeadersFooters(
                mDoc,
                savedElement,
                areas,
                "kdchart:headers-footers" );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );
        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";


        // prepare parsing
        QVERIFY( SerializeCollector::initializeParsedGlobalPointers( mDocRoot ) );


        bool bFoundSavedAxis = false;
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
                            if( tagName2 == "kdchart:headers-footers" ) {
                                QDomNode node3 = ele2.firstChild();
                                while( !node3.isNull() ) {
                                    QDomElement ele3 = node3.toElement();
                                    if( !ele3.isNull() ) { // was really an element
                                        QString tagName3 = ele3.tagName();
                                        if( tagName3 == "kdchart:header-footer:1" ) {
                                            parsedElement = ele3;
                                            bFoundSavedAxis = true;
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
        QVERIFY( bFoundSavedAxis );

        HeaderFooter* parsedFooter=0;
        QVERIFY( TextAreaSerializer::parseHeaderFooter( parsedElement, parsedFooter ) );
        QVERIFY( orgFooter.compare( *parsedFooter ) );
    }


    void cleanupTestCase()
    {
        // this space left empty intentionally
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
};

QTEST_MAIN(TestKDChartTextAreaSerializer)

#include "main.moc"
