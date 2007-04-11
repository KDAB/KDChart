#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartCartesianAxis>

#include <KDChartAxesSerializer>
#include <KDChartSerializeCollector>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartAxesSerializer : public QObject {
  Q_OBJECT
private slots:

    void initTestCase()
    {
        mDocstart = "<kdchart:kdchart/>";
        resetDoc();

        mAxesS = new AxesSerializer();
    }

    void testCartesianAxis()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        CartesianAxis orgAxis( 0 );
        orgAxis.setPosition( CartesianAxis::Right );
        orgAxis.setTitleText( "Nix Nought Nothing" );
        TextAttributes ta = orgAxis.textAttributes();
        ta.setFont( QFont("Courier", 17, QFont::Bold, true ) );
        orgAxis.setTitleTextAttributes( ta );

        ta.setFont( QFont("Helvetica", 19, QFont::Light, false ) );
        orgAxis.setTextAttributes( ta );
        QStringList list;
        list << "one" << "two" << "a-one-two-three";
        orgAxis.setLabels( list );
        QStringList shortList;
        shortList << "1" << "2" << "1-2-3";
        orgAxis.setShortLabels( shortList );

        KDChart::CartesianAxisList axes;
        axes.append( &orgAxis );

        mAxesS->saveCartesianAxes( mDoc, savedElement,
                                   axes,
                                   "kdchart:axes" );
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
                            if( tagName2 == "kdchart:axes" ) {
                                QDomNode node3 = ele2.firstChild();
                                while( !node3.isNull() ) {
                                    QDomElement ele3 = node3.toElement();
                                    if( !ele3.isNull() ) { // was really an element
                                        QString tagName3 = ele3.tagName();
                                        if( tagName3 == "kdchart:axis:1" ) {
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

        CartesianAxis* parsedAxis=0;
        QVERIFY( mAxesS->parseCartesianAxis( parsedElement, parsedAxis ) );
        QVERIFY( orgAxis.compare( parsedAxis ) );
    }


    void cleanupTestCase()
    {
        delete mAxesS;
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

    AxesSerializer *mAxesS;
};

QTEST_MAIN(TestKDChartAxesSerializer)

#include "main.moc"
