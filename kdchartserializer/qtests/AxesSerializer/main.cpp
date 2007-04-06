#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartCartesianAxis>

#include <KDChartAxesSerializer>
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

        mAxesS->saveCartesianAxis(
                mDoc,
                savedElement,
                orgAxis,
                "TestCartesianAxis" );
        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        CartesianAxis parsedAxis;
        QVERIFY( mAxesS->parseCartesianAxis( parsedElement, parsedAxis ) );
        QVERIFY( orgAxis.compare( &parsedAxis ) );
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
