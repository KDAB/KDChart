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
#include <QPointF>
#include <QPair>
#include <QString>
#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartGridAttributes>
#include <KDChartLegend>

#include <TableModel.h>

using namespace KDChart;

#if QT_VERSION < 0x040200
namespace QTest{
template <> inline char *toString(const QPointF &p) {
    return qstrdup(QString::fromLatin1("QPointF(%1,%2)").arg(p.x()).arg(p.y()).toLatin1().constData());
 }
}
#endif

class TestCartesianPlanes: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_tableModel = new TableModel( this );
        m_tableModel->loadFromCSV( ":/data" );
        m_bars = new BarDiagram();
        m_bars->setModel( m_tableModel );
        m_lines = new LineDiagram();
        m_lines->setModel( m_tableModel );
        m_plane = new CartesianCoordinatePlane();
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

    void testDiagramOwnership()
    {
        m_plane->addDiagram(  m_bars );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        m_plane->addDiagram(  m_lines );
        QCOMPARE( m_plane->diagrams().size(),  2 );
        QCOMPARE( dynamic_cast< BarDiagram * >(m_plane->diagram()),  m_bars );
        m_plane->takeDiagram( m_bars );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( dynamic_cast< LineDiagram * >(m_plane->diagram()),  m_lines );
        m_plane->replaceDiagram( m_bars,  m_lines );
        QCOMPARE( m_plane->diagrams().size(),  1 );
        QCOMPARE( dynamic_cast< BarDiagram * >(m_plane->diagram()),  m_bars );
        m_plane->takeDiagram( m_bars );
        QCOMPARE( m_plane->diagrams().size(),  0 );
        delete m_bars;
    }

      void testIsometricScalingSettings()
    {
        QVERIFY( m_plane->doesIsometricScaling() == false );
        m_plane->setIsometricScaling( true );
        QVERIFY( m_plane->doesIsometricScaling() == true );
    }


    void testZoomFactorsSettings()
    {
        QCOMPARE( m_plane->zoomFactorX(),  1.0 );
        QCOMPARE( m_plane->zoomFactorY(),  1.0 );
        QCOMPARE( m_plane->zoomCenter(), QPointF( 0.5, 0.5 ) );
        m_plane->setZoomFactorX( 1.5 );
        m_plane->setZoomFactorY( 1.5 );
        m_plane->setZoomCenter( QPointF ( 1.0, 1.0 ) );
        QCOMPARE( m_plane->zoomFactorX(),  1.5 );
        QCOMPARE( m_plane->zoomFactorY(),  1.5 );
        QCOMPARE( m_plane->zoomCenter(),  QPointF( 1.0, 1.0 ) );
    }


    void testRangeSettings()
    {
        initTestCase ();
        const QPair< qreal,  qreal> expectedHrange( 0, 30 );
        const QPair< qreal,  qreal> expectedVrange( 34, 45 );
        m_plane->addDiagram(  m_bars );
        // note: the range is only Null, if auto-adjusting is turned off!
        m_plane->setAutoAdjustHorizontalRangeToData(100);
        m_plane->setAutoAdjustVerticalRangeToData(100);
        //qDebug() << m_plane->horizontalRange() << m_plane->verticalRange();
        const QPair<qreal, qreal> hrange = m_plane->horizontalRange();
        const QPair<qreal, qreal> vrange = m_plane->verticalRange();
        
        //use single-component QCOMPARE instead of QCOMPARE on the pairs, to make sure qFuzzyCompare is used
        QCOMPARE( hrange.first + 1, expectedHrange.first + 1 ); //avoid comparison against 0, which doesn't work with qFuzzyCompare
        QCOMPARE( hrange.second, expectedHrange.second );
        QCOMPARE( vrange.first, expectedVrange.first );
        QCOMPARE( vrange.second, expectedVrange.second );
        QPair< qreal,  qreal> hboundaries( m_bars->dataBoundaries().first.x(),
                                           m_bars->dataBoundaries().second.x() );
        QPair< qreal,  qreal> vboundaries( int(m_bars->dataBoundaries().first.y()+0.5),
                                           int(m_bars->dataBoundaries().second.y()+0.5) );
        m_plane->setHorizontalRange( hboundaries );
        m_plane->setVerticalRange( vboundaries );
        const QPair<qreal, qreal> newhb = m_plane->horizontalRange();
        const QPair<qreal, qreal> newvb = m_plane->verticalRange();
        QCOMPARE( newhb.first, hboundaries.first );
        QCOMPARE( newhb.second, hboundaries.second );
        QCOMPARE( newvb.first, vboundaries.first );
        QCOMPARE( newvb.second, vboundaries.second );
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
        GridAttributes gh = m_plane->gridAttributes( Qt::Horizontal );
        GridAttributes gv = m_plane->gridAttributes( Qt::Vertical );
        QVERIFY( gh.isGridVisible() == true );
        QVERIFY( gv.isGridVisible() == true );
        gh.setGridVisible( false );
        m_plane->setGridAttributes( Qt::Horizontal, gh );
        QVERIFY( m_plane->hasOwnGridAttributes( Qt::Horizontal ) == true );
        QVERIFY( m_plane->hasOwnGridAttributes( Qt::Vertical ) == false );
        QVERIFY( m_plane->gridAttributes( Qt::Horizontal ).isGridVisible() == false );
        QVERIFY( m_plane->gridAttributes( Qt::Vertical ).isGridVisible() == true );
        gv.setGridVisible( false );
        m_plane->setGridAttributes( Qt::Vertical, gv );
        QVERIFY( m_plane->hasOwnGridAttributes( Qt::Horizontal ) == true );
        QVERIFY( m_plane->hasOwnGridAttributes( Qt::Vertical ) == true );
        QVERIFY( m_plane->gridAttributes( Qt::Horizontal ).isGridVisible() == false );
        QVERIFY( m_plane->gridAttributes( Qt::Vertical ).isGridVisible() == false );
        m_plane->resetGridAttributes( Qt::Horizontal );
        m_plane->resetGridAttributes( Qt::Vertical );
        QVERIFY( m_plane->gridAttributes( Qt::Horizontal ).isGridVisible() == true );
        QVERIFY( m_plane->gridAttributes( Qt::Vertical ).isGridVisible() == true );
        QVERIFY( m_plane->hasOwnGridAttributes( Qt::Horizontal ) == false );
        QVERIFY( m_plane->hasOwnGridAttributes( Qt::Vertical ) == false );
    }

    void testAxesCalcModesSettings()
    {
        QCOMPARE( m_plane->axesCalcModeX(), AbstractCoordinatePlane::Linear );
        QCOMPARE( m_plane->axesCalcModeY(), AbstractCoordinatePlane::Linear );
        m_plane->setAxesCalcModes(  AbstractCoordinatePlane::Logarithmic );
        QCOMPARE( m_plane->axesCalcModeX(), AbstractCoordinatePlane::Logarithmic );
        QCOMPARE( m_plane->axesCalcModeY(), AbstractCoordinatePlane::Logarithmic );
        m_plane->setAxesCalcModeX(  AbstractCoordinatePlane::Linear );
        QCOMPARE( m_plane->axesCalcModeX(), AbstractCoordinatePlane::Linear );
        QCOMPARE( m_plane->axesCalcModeY(), AbstractCoordinatePlane::Logarithmic );
        m_plane->setAxesCalcModeY(  AbstractCoordinatePlane::Linear );
        QCOMPARE( m_plane->axesCalcModeX(), AbstractCoordinatePlane::Linear );
        QCOMPARE( m_plane->axesCalcModeY(), AbstractCoordinatePlane::Linear );
    }

    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    BarDiagram *m_bars;
    LineDiagram *m_lines;
    CartesianCoordinatePlane *m_plane;
    TableModel *m_tableModel;

};

QTEST_MAIN(TestCartesianPlanes)

#include "main.moc"
