#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartDataValueAttributes>

#include <KDChartAttributesSerializer>
#include <KDXMLTools>

using namespace KDChart;

class TestKDChartAttributesSerializer : public QObject {
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
        m_plane = new CartesianCoordinatePlane(0);

        m_bars = new BarDiagram();
        m_bars->setModel( m_model );
    }

    void testLeading()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        int savedLeft= 12;
        int savedTop  = 13;
        int savedRight = 14;
        int savedBottom = 15;

        AttributesSerializer::saveLeading(
                mDoc,
                savedElement,
                savedLeft, savedTop, savedRight, savedBottom,
                "TestTextLeading" );

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        int parsedLeft= 0;
        int parsedTop  = 0;
        int parsedRight = 0;
        int parsedBottom = 0;
        QVERIFY( AttributesSerializer::parseLeading(
                parsedElement, parsedLeft, parsedTop, parsedRight, parsedBottom ) );
        QCOMPARE( savedLeft,   parsedLeft );
        QCOMPARE( savedTop,    parsedTop );
        QCOMPARE( savedRight,  parsedRight );
        QCOMPARE( savedBottom, parsedBottom );
    }

    void testTextAttributes()
    {
        resetDoc();

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        DataValueAttributes da = m_bars->dataValueAttributes( idx );

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
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

    void testMeasure()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        Measure orgMe( 100.0 );
        orgMe.setCalculationMode( KDChartEnums::MeasureCalculationModeAbsolute );
        orgMe.setReferenceOrientation( KDChartEnums::MeasureOrientationHorizontal );

        AttributesSerializer::saveMeasure(
                mDoc,
                savedElement,
                orgMe,
                "TestMeasure" );

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        Measure parsedMe;
        QVERIFY( AttributesSerializer::parseMeasure( parsedElement, parsedMe ) );
        QCOMPARE( orgMe, parsedMe );
    }

    void testBrushAndPen()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        const QBrush br( QColor( Qt::white ) );
        const QPen orgPen( br, 20, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin );

        KDXML::createPenNode( mDoc, savedElement, "TestBrushAndPen",
                              orgPen );

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        QPen parsedPen;
        QVERIFY( KDXML::readPenNode( parsedElement, parsedPen ) );
        QCOMPARE( orgPen, parsedPen );
    }

    void testFrameAttributes()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        FrameAttributes orgAttrs;
        orgAttrs.setVisible( true );
        const QBrush br( QColor( Qt::white ) );
        orgAttrs.setPen( QPen( br, 20, Qt::DashDotLine, Qt::RoundCap,   Qt::RoundJoin ) );
        orgAttrs.setPadding( 4 );

        AttributesSerializer::saveFrameAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestFrameAttributes" );

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        FrameAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseFrameAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
    }

    void testBackgroundAttributes()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        BackgroundAttributes orgAttrs;
        orgAttrs.setVisible( true );
        const QBrush br( QColor( Qt::white ) );
        orgAttrs.setBrush( br );
        orgAttrs.setPixmapMode( BackgroundAttributes::BackgroundPixmapModeStretched );

        const int pixWidth =5;
        const int pixHeight=4;
        const QColor pixColor=Qt::blue;
        QPixmap orgPix( pixWidth, pixHeight );
        orgPix.fill( pixColor );
        orgAttrs.setPixmap( orgPix );
        /*
        qDebug() << "\n\n---------------------------------------------------------------------\n"
                +mDoc.toString(2);
        */
        AttributesSerializer::saveBackgroundAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestBackgroundAttributes" );
        /*
        qDebug() << "\n\n---------------------------------------------------------------------\n"
                +mDoc.toString(2);
        */
        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        BackgroundAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseBackgroundAttributes( parsedElement, parsedAttrs ) );

        // carefully now: first ignore the pixmap
        QVERIFY( orgAttrs.isEqualTo( parsedAttrs, true ) );
        // then compare the pixmap manually
        const QImage orgImg(    orgAttrs.pixmap().toImage() );
        const QImage parsedImg( parsedAttrs.pixmap().toImage() );
        QCOMPARE( orgImg.size(), parsedImg.size() );
        for( int x=0; x<pixWidth; ++x )
            for( int y=0; y<pixHeight; ++y )
                QCOMPARE( orgImg.pixel(x,y), orgImg.pixel(x,y) );
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
    CartesianCoordinatePlane* m_plane;
    BarDiagram *m_bars;

};

QTEST_MAIN(TestKDChartAttributesSerializer)

#include "main.moc"
