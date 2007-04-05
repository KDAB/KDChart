#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartHeaderFooter>

#include <KDChartTextAreaSerializer>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartAttributesSerializer : public QObject {
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

        TextAreaSerializer::saveHeaderFooter(
                mDoc,
                savedElement,
                orgFooter );
        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement;
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        HeaderFooter parsedFooter;
        QVERIFY( TextAreaSerializer::parseHeaderFooter( parsedElement, parsedFooter ) );
        QVERIFY( orgFooter.compare( parsedFooter ) );
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

QTEST_MAIN(TestKDChartAttributesSerializer)

#include "main.moc"
