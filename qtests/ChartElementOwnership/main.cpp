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

#include <KDChartChart>
#include <KDChartGlobal>
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartPolarCoordinatePlane>
#include <KDChartLegend>
#include <KDChartHeaderFooter>

using namespace KDChart;

class TestChartElementOwnership: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
    }

    void testPlaneOwnership()
    {
        // check number plane
        AbstractCoordinatePlane*orig = m_chart->coordinatePlane();
        QCOMPARE( m_chart->coordinatePlanes().size(), 1 );

        // add and take
        CartesianCoordinatePlane *p = new CartesianCoordinatePlane();
        m_chart->addCoordinatePlane( p );
        QCOMPARE( m_chart->coordinatePlanes().size(), 2 );
        m_chart->takeCoordinatePlane( orig );
        QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
        QCOMPARE( dynamic_cast< CartesianCoordinatePlane * >(m_chart->coordinatePlane()), p );
        m_chart->addCoordinatePlane( orig );
        QCOMPARE( m_chart->coordinatePlanes().size(), 2 );

        // replace abstract by polar
        PolarCoordinatePlane *po = new PolarCoordinatePlane();
        m_chart->replaceCoordinatePlane( po, orig );
        QCOMPARE( m_chart->coordinatePlanes().size(), 2 );
        m_chart->takeCoordinatePlane( p );
        QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
        QCOMPARE( dynamic_cast< PolarCoordinatePlane * >(m_chart->coordinatePlane()), po );
        m_chart->addCoordinatePlane( p );
        QCOMPARE( m_chart->coordinatePlanes().size(), 2 );

        // delete
        delete po;
        QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
        QCOMPARE( dynamic_cast< CartesianCoordinatePlane * >(m_chart->coordinatePlane()), p );

        // replace cartesian by polar
        PolarCoordinatePlane*polast = new PolarCoordinatePlane();
        m_chart->replaceCoordinatePlane( polast );
        QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
        QCOMPARE( dynamic_cast< PolarCoordinatePlane * >(m_chart->coordinatePlane()), polast );

        // replace polar by cartesian
        CartesianCoordinatePlane* plast = new CartesianCoordinatePlane();
        m_chart->replaceCoordinatePlane( plast,  polast );
        QCOMPARE( m_chart->coordinatePlanes().size(), 1 );
        QCOMPARE( dynamic_cast< CartesianCoordinatePlane * >(m_chart->coordinatePlane()), plast );

    }

    void testHeaderFooterOwnership()
    {
        QCOMPARE( m_chart->headerFooters().size(), 0 );
        HeaderFooter * h = new HeaderFooter();
        m_chart->addHeaderFooter( h );
        QCOMPARE( m_chart->headerFooters().size(), 1 );
        m_chart->takeHeaderFooter( h );
        QCOMPARE( m_chart->headerFooters().size(), 0 );
        m_chart->addHeaderFooter( h );
        QCOMPARE( m_chart->headerFooters().size(), 1 );
        delete h;
        QCOMPARE( m_chart->headerFooters().size(), 0 );
    }

    void testHeaderFooterReplace()
    {
        QCOMPARE( m_chart->headerFooters().size(), 0 );
        HeaderFooter * h = new HeaderFooter();
        HeaderFooter * h1 = new HeaderFooter();
        m_chart->addHeaderFooter( h );
        QCOMPARE( m_chart->headerFooters().size(), 1 );
        m_chart->addHeaderFooter( h1 );
        QCOMPARE( m_chart->headerFooters().size(), 2 );
        m_chart->takeHeaderFooter( h );
        QCOMPARE( m_chart->headerFooters().size(), 1 );
        QCOMPARE( m_chart->headerFooter(), h1 );
        m_chart->replaceHeaderFooter( h,  h1 );
        QCOMPARE( m_chart->headerFooters().size(), 1 );
        delete h;
        QCOMPARE( m_chart->headerFooters().size(), 0 );
    }


    void testLegendOwnership()
    {
        // check no legend
        QCOMPARE( m_chart->legends().size(), 0 );

        // check add legend - take legend - delete legend
        Legend * legend = new Legend( m_chart->coordinatePlane()->diagram() );
        m_chart->addLegend( legend );
        QCOMPARE( m_chart->legends().size(), 1 );
        m_chart->takeLegend( legend );
        QCOMPARE( m_chart->legends().size(), 0 );
        m_chart->addLegend( legend );
        QCOMPARE( m_chart->legends().size(), 1 );
        delete legend;
        QCOMPARE( m_chart->legends().size(), 0 );
    }

    void testLegendReplace()
    {

        // check no legend
        QCOMPARE( m_chart->legends().size(), 0 );
        // check add several legends - take legend
        // replace legend - delete legend
        Legend * legend = new Legend( m_chart->coordinatePlane()->diagram() );
        Legend * legend2 = new Legend( m_chart->coordinatePlane()->diagram() );
        m_chart->addLegend( legend );
        QCOMPARE( m_chart->legends().size(), 1 );
        m_chart->addLegend( legend2 );
        QCOMPARE( m_chart->legends().size(), 2 );
        m_chart->takeLegend( legend );
        QCOMPARE( m_chart->legends().size(), 1 );
        QCOMPARE( m_chart->legend(), legend2 );
        m_chart->replaceLegend( legend, legend2  );
        QCOMPARE( m_chart->legends().size(), 1 );
        delete legend;
        QCOMPARE( m_chart->legends().size(), 0 );
    }

    void testPadding()
    {
      QVERIFY( m_chart->globalLeadingLeft() == false );
      m_chart->setGlobalLeading( 2, 2, 2, 2 );
      QCOMPARE( m_chart->globalLeadingLeft(), 2 );
      QCOMPARE( m_chart->globalLeadingTop(), 2 );
      QCOMPARE( m_chart->globalLeadingRight(), 2 );
      QCOMPARE( m_chart->globalLeadingBottom(), 2 );
      m_chart->setGlobalLeadingLeft( 5 );
      QCOMPARE( m_chart->globalLeadingLeft(), 5 );
      QCOMPARE( m_chart->globalLeadingTop(), 2 );
      QCOMPARE( m_chart->globalLeadingRight(), 2 );
      QCOMPARE( m_chart->globalLeadingBottom(), 2 );
      m_chart->setGlobalLeadingTop( 5 );
      QCOMPARE( m_chart->globalLeadingLeft(), 5 );
      QCOMPARE( m_chart->globalLeadingTop(), 5 );
      QCOMPARE( m_chart->globalLeadingRight(), 2 );
      QCOMPARE( m_chart->globalLeadingBottom(), 2 );
      m_chart->setGlobalLeadingRight( 5 );
      QCOMPARE( m_chart->globalLeadingLeft(), 5 );
      QCOMPARE( m_chart->globalLeadingTop(), 5 );
      QCOMPARE( m_chart->globalLeadingRight(), 5 );
      QCOMPARE( m_chart->globalLeadingBottom(), 2 );
      m_chart->setGlobalLeadingBottom( 5 );
      QCOMPARE( m_chart->globalLeadingLeft(), 5 );
      QCOMPARE( m_chart->globalLeadingTop(), 5 );
      QCOMPARE( m_chart->globalLeadingRight(), 5 );
      QCOMPARE( m_chart->globalLeadingBottom(), 5 );
      m_chart->setGlobalLeading( 2, 2, 2, 2 );
      QCOMPARE( m_chart->globalLeadingLeft(), 2 );
      QCOMPARE( m_chart->globalLeadingTop(), 2 );
      QCOMPARE( m_chart->globalLeadingRight(), 2 );
      QCOMPARE( m_chart->globalLeadingBottom(), 2 );
    }


    void testChartDeletion()
    {
        delete m_chart;
    }

    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;

};

QTEST_MAIN(TestChartElementOwnership)

#include "main.moc"
