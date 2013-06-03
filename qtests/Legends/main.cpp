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
#include <KDChartBarDiagram>
#include <KDChartLineDiagram>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLegend>

#include <TableModel.h>

using namespace KDChart;

class TestLegends: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_lines = new LineDiagram();
        m_bars = new BarDiagram();
        m_tableModel = new TableModel( this );
        m_tableModel->loadFromCSV( "../../examples/tools/modeldata/KDChart-Test-Datatables.csv" );
        m_lines->setModel( m_tableModel );
        m_bars->setModel( m_tableModel );
        m_chart->coordinatePlane()->replaceDiagram( m_lines );
    }

    void testIntialOwnership()
    {
        Legend* l = new Legend( m_chart );
        m_chart->addLegend( l );
        QCOMPARE( m_chart->legend(), l );
        QCOMPARE( l->diagram(), (AbstractDiagram*)0);
        l->setDiagram( m_lines );
        QCOMPARE( dynamic_cast< LineDiagram * >(l->diagram()), m_lines );
    }

    void testIntialOwnershipFromCtor()
    {
        Legend* l = new Legend( m_lines, m_chart );
        m_chart->replaceLegend( l );
        QCOMPARE( m_chart->legend(), l );
        QCOMPARE( dynamic_cast< LineDiagram * >(l->diagram()), m_lines );
    }

    void testReplacing()
    {
        Legend* l = new Legend( m_chart );
        QPointer<Legend> oldLegend = m_chart->legend();
        QCOMPARE( dynamic_cast< LineDiagram * >(oldLegend->diagram()), m_lines );
        m_chart->replaceLegend( l, oldLegend );
        QVERIFY( oldLegend.isNull() );
        QCOMPARE( l->diagram(), (AbstractDiagram*)0 );
        l->setDiagram( m_lines );
        QCOMPARE( dynamic_cast< LineDiagram * >(l->diagram()), m_lines );
    }

    void testReferenceArea()
    {
         Legend* l = new Legend( );
         QCOMPARE( l->referenceArea(), ( const QWidget* )0 );
         l->setReferenceArea( m_chart );
         QCOMPARE( dynamic_cast< const Chart * >(l->referenceArea()), const_cast< const Chart * >(m_chart) );
         Legend* l1 = new Legend( m_chart );
         QCOMPARE( dynamic_cast< const Chart * >(l1->referenceArea()), const_cast< const Chart * >(m_chart) );
         Legend* l2 = new Legend( m_lines,  m_chart );
         QCOMPARE( dynamic_cast< const Chart * >(l2->referenceArea()), const_cast< const Chart * >(m_chart) );
    }

    void testDiagramOwnership()
    {
        Legend* l = new Legend( m_chart );
        QVERIFY( l->diagrams().size() == 0 );
        l->addDiagram( m_lines );
        QVERIFY( l->diagrams().size() == 1 );
        l->addDiagram( m_bars );
        QVERIFY( l->diagrams().size() == 2 );
        QCOMPARE( dynamic_cast< LineDiagram * >(l->diagram()),  m_lines );
        l->removeDiagram( m_lines );
        QVERIFY( l->diagrams().size() == 1 );
        QCOMPARE( dynamic_cast< BarDiagram * >(l->diagram()),  m_bars );
        l->replaceDiagram( m_lines, m_bars );
        QVERIFY( l->diagrams().size() == 1 );
        QCOMPARE( dynamic_cast< LineDiagram * >(l->diagram()),  m_lines );
    }

    void testLegendSettings()
    {
       Legend* l = new Legend( m_lines,  m_chart );
       QVERIFY( l->position() == Position::NorthEast );
       QVERIFY( l->alignment() == Qt::AlignCenter );
       QVERIFY( l->orientation() == Qt::Vertical );
       QVERIFY( l->showLines() == false );
       QVERIFY( !l->titleText().isEmpty() );
       QVERIFY( l->spacing() == 1 );
       QVERIFY( l->legendStyle() == Legend::MarkersOnly );
       l->setPosition( Position::North );
       l->setAlignment( Qt::AlignLeft );
       l->setOrientation( Qt::Horizontal );
       l->setShowLines( true );
       l->setTitleText( QString( tr( "Lines" ) ) );
       l->setSpacing( 2 );
       l->setLegendStyle( Legend::LinesOnly );
       QVERIFY( l->position() == Position::North );
       QVERIFY( l->alignment() == Qt::AlignLeft );
       QVERIFY( l->orientation() == Qt::Horizontal );
       QVERIFY( l->showLines() == true );
       QVERIFY( l->titleText() == QString( tr( "Lines" ) ) );
       QVERIFY( l->spacing() == 2 );
       QVERIFY( l->legendStyle() == Legend::LinesOnly );
    }


    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    BarDiagram *m_bars;
    LineDiagram *m_lines;
    Legend* m_legend;
    TableModel *m_tableModel;

};

QTEST_MAIN(TestLegends)

#include "main.moc"
