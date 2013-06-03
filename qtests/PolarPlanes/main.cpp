/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include <QtTest/QtTest>
#include <QStandardItemModel>

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartPolarCoordinatePlane>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLegend>
#include <KDChartGridAttributes>

#include <TableModel.h>

using namespace KDChart;

#if QT_VERSION < 0x040200
namespace QTest{
template <> inline char *toString(const QPointF &p) {
    return qstrdup(QString::fromLatin1("QPointF(%1,%2)").arg(p.x()).arg(p.y()).toLatin1().constData());
 }
}
#endif

class TestPolarPlanes: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_tableModel = new TableModel( this );
        m_tableModel->loadFromCSV( ":/data" );
        m_pie = new PieDiagram();
        m_pie->setModel( m_tableModel );
        m_polar = new PolarDiagram();
        m_polar->setModel( m_tableModel );
        m_plane = new PolarCoordinatePlane();
        m_chart->addCoordinatePlane( m_plane );
        m_plane->setReferenceCoordinatePlane( m_chart->coordinatePlane() );
    }

    void testIntialOwnership()
    {
        AbstractCoordinatePlane *plane = m_chart->coordinatePlane();
        QCOMPARE( m_plane->referenceCoordinatePlane(), m_chart->coordinatePlane() );
        m_chart->takeCoordinatePlane( 0 );
        delete plane;
        QCOMPARE( m_plane->referenceCoordinatePlane(), (AbstractCoordinatePlane*)0 );
    }

    void testStartPositionSettings()
    {
        m_plane->addDiagram(  m_pie );
        QVERIFY( m_plane->startPosition() ==  0.0 );
        qreal pos = 45;
        m_plane->addDiagram(  m_pie );
        m_plane->setStartPosition( pos );
        QVERIFY( m_plane->startPosition() ==  pos );
        m_plane->takeDiagram(  m_pie );
    }

      void testZoomFactorsSettings()
    {
        m_plane->addDiagram(  m_pie );
        QCOMPARE( m_plane->zoomFactorX(),  1.0 );
        QCOMPARE( m_plane->zoomFactorY(),  1.0 );
        QCOMPARE( m_plane->zoomCenter(), QPointF( 0.5, 0.5 ) );
        m_plane->setZoomFactorX( 1.5 );
        m_plane->setZoomFactorY( 1.5 );
        m_plane->setZoomCenter( QPointF ( 1.0, 1.0 ) );
        QCOMPARE( m_plane->zoomFactorX(),  1.5 );
        QCOMPARE( m_plane->zoomFactorY(),  1.5 );
        QCOMPARE( m_plane->zoomCenter(),  QPointF( 1.0, 1.0 ) );
        m_plane->takeDiagram(  m_pie );
    }

    void testDiagramOwnership()
    {

        QCOMPARE( m_plane->diagrams().size(),  1 );
        m_plane->addDiagram(  m_polar );
        QCOMPARE( m_plane->diagrams().size(),  2 );
        QCOMPARE( dynamic_cast< PieDiagram * >(m_plane->diagram()),  m_pie );
        m_plane->takeDiagram( m_pie );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( dynamic_cast< PolarDiagram * >(m_plane->diagram()),  m_polar );
        m_plane->replaceDiagram( m_pie,  m_polar );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( dynamic_cast< PieDiagram * >(m_plane->diagram()),  m_pie );
        m_plane->takeDiagram( m_pie );
        QCOMPARE( m_plane->diagrams().size(),  0 );
        delete m_pie;
    }

    void testGlobalGridAttributesSettings()
    {
        GridAttributes ga = m_plane->globalGridAttributes();
        QVERIFY( ga.isGridVisible() == true );
        ga.setGridVisible(  false );
        m_plane->setGlobalGridAttributes(  ga );
        QVERIFY( m_plane->globalGridAttributes().isGridVisible() == false );
        //reset to normal
        ga.setGridVisible(  true );
        QVERIFY( m_plane->globalGridAttributes().isGridVisible() == false );
        m_plane->setGlobalGridAttributes(  ga );
        QVERIFY( m_plane->globalGridAttributes().isGridVisible() == true );
    }

      void testGridAttributesSettings()
    {
        GridAttributes gcircular = m_plane->gridAttributes( true );
        GridAttributes gsagittal = m_plane->gridAttributes( false );
        QVERIFY( gcircular.isGridVisible() == true );
        gcircular.setGridVisible( false );
        m_plane->setGridAttributes( true, gcircular );
        QVERIFY( m_plane->hasOwnGridAttributes( true ) == true );
        QVERIFY( m_plane->hasOwnGridAttributes( false ) == false );
        QVERIFY( m_plane->gridAttributes( true ).isGridVisible() == false );
        QVERIFY( m_plane->gridAttributes( false ).isGridVisible() == true );
        gsagittal.setGridVisible( false );
        m_plane->setGridAttributes( false, gsagittal );
        QVERIFY( m_plane->hasOwnGridAttributes( true ) == true );
        QVERIFY( m_plane->hasOwnGridAttributes( true ) == true );
        QVERIFY( m_plane->gridAttributes( true ).isGridVisible() == false );
        QVERIFY( m_plane->gridAttributes( false ).isGridVisible() == false );
        m_plane->resetGridAttributes( true );
        m_plane->resetGridAttributes( false );
        QVERIFY( m_plane->gridAttributes( true ).isGridVisible() == true );
        QVERIFY( m_plane->gridAttributes( false ).isGridVisible() == true );
        QVERIFY( m_plane->hasOwnGridAttributes( true ) == false );
        QVERIFY( m_plane->hasOwnGridAttributes( false ) == false );
    }

    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    PieDiagram *m_pie;
    PolarDiagram *m_polar;
    PolarCoordinatePlane *m_plane;
    TableModel *m_tableModel;

};

QTEST_MAIN(TestPolarPlanes)

#include "main.moc"
