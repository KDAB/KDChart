#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartHeaderFooter>

#include <KDChartSerializer>
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


        // Note: We do not instantiate a Serializer object, so we
        //       must register the built-in factories explicitely:
        Serializer::registerBuiltInSerializerFactories();


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
        QVERIFY( SerializeCollector::initializeGlobalPointers( mDocRoot ) );

        HeaderFooter* parsedFooter=0;
        QVERIFY( TextAreaSerializer::parseHeaderFooter(
                        mDocRoot, savedElement.firstChild().firstChild(), parsedFooter ) );

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
