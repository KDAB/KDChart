#include <QtTest/QtTest>
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartChart>

#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartPieDiagram>

#include <KDChartCartesianCoordinatePlane>
#include <KDChartDataValueAttributes>

#include <KDChartAttributesSerializer>
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

        m_chart = new Chart(0);
        TableModel *tableModel = new TableModel( this );
        tableModel->loadFromCSV( "../../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        tableModel->setSupplyHeaderData( false );
        m_model = tableModel;
        m_plane = new CartesianCoordinatePlane(0);

        m_bars = new BarDiagram();
        m_bars->setModel( m_model );
        m_lines = new LineDiagram();
        m_lines->setModel( m_model );
        m_pies = new PieDiagram();
        m_pies->setModel( m_model );
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

    void testMarkerAttributes()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        MarkerAttributes orgAttrs;

        AttributesSerializer::saveMarkerAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestMarkerAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        MarkerAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseMarkerAttributes( parsedElement, parsedAttrs ) );


        resetDoc();

        QDomElement savedElement2 =
                mDoc.createElement( "TESTING_#2" );
        mDocRoot.appendChild( savedElement2 );

        AttributesSerializer::saveMarkerAttributes(
                mDoc,
                savedElement2,
                parsedAttrs,
                "TestMarkerAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n testing saving parsed data:\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

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

    void testDataValueAttributes()
    {
        resetDoc();

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        DataValueAttributes orgAttrs = m_bars->dataValueAttributes( idx );

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        AttributesSerializer::saveDataValueAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestDataValueAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        DataValueAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseDataValueAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
    }

    void testLineAttributes()
    {
        resetDoc();

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        LineAttributes orgAttrs = m_lines->lineAttributes( idx );
        orgAttrs.setMissingValuesPolicy( LineAttributes::MissingValuesShownAsZero );
        orgAttrs.setDisplayArea( true );
        orgAttrs.setTransparency( 40 );

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        AttributesSerializer::saveLineAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestLineAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        LineAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseLineAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
    }

    void testBarAttributes()
    {
        resetDoc();

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        BarAttributes orgAttrs = m_bars->barAttributes( idx );
        orgAttrs.setFixedDataValueGap( 4.5 );
        orgAttrs.setUseFixedDataValueGap( true );
        orgAttrs.setFixedValueBlockGap( 5.5 );
        orgAttrs.setUseFixedValueBlockGap( true );
        orgAttrs.setFixedBarWidth( 40.5 );
        orgAttrs.setUseFixedBarWidth( true );
        orgAttrs.setGroupGapFactor( 1.25 );
        orgAttrs.setBarGapFactor(   1.5 );
        orgAttrs.setDrawSolidExcessArrows( true );

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        AttributesSerializer::saveBarAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestBarAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        BarAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseBarAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
    }

    void testPieAttributes()
    {
        resetDoc();

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        PieAttributes orgAttrs = m_pies->pieAttributes( idx );
        orgAttrs.setExplode( true );
        orgAttrs.setExplodeFactor( 7.5 );

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        AttributesSerializer::savePieAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "TestPieAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        PieAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parsePieAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
    }

    void testThreeDBarAttributes()
    {
        resetDoc();

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        ThreeDBarAttributes orgAttrs = m_bars->threeDBarAttributes( idx );
        // set the abstract part to test that one too:
        orgAttrs.setEnabled( true );
        orgAttrs.setDepth( 1.15 );
        // set the bar attrs part:
        orgAttrs.setUseShadowColors( true );
        orgAttrs.setAngle( 71 );

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        AttributesSerializer::saveThreeDBarAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "ThreeDBarAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        ThreeDBarAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseThreeDBarAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
    }

    void testThreeDLineAttributes()
    {
        resetDoc();

        QModelIndex idx = m_model->index( 0, 2, QModelIndex() );
        ThreeDLineAttributes orgAttrs = m_lines->threeDLineAttributes( idx );
        // set the abstract part to test that one too:
        orgAttrs.setEnabled( true );
        orgAttrs.setDepth( 1.15 );
        // set the bar attrs part:
        orgAttrs.setLineYRotation( 18 );
        orgAttrs.setLineYRotation( 33 );

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        AttributesSerializer::saveThreeDLineAttributes(
                mDoc,
                savedElement,
                orgAttrs,
                "ThreeDLineAttributes" );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";

        QDomNode parsedNode = savedElement.firstChild();
        QVERIFY( ! parsedNode.isNull() );

        QDomElement parsedElement = parsedNode.toElement();
        QVERIFY( ! parsedElement.isNull() );

        ThreeDLineAttributes parsedAttrs;
        QVERIFY( AttributesSerializer::parseThreeDLineAttributes( parsedElement, parsedAttrs ) );
        QCOMPARE( orgAttrs, parsedAttrs );
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
    LineDiagram *m_lines;
    PieDiagram *m_pies;
};

QTEST_MAIN(TestKDChartAttributesSerializer)

#include "main.moc"
