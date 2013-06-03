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
#include <KDChartPolarDiagram>
#include <KDChartPolarCoordinatePlane>

#include <TableModel.h>

using namespace KDChart;

class TestPolarDiagrams: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        PolarCoordinatePlane* polarPlane = new PolarCoordinatePlane( m_chart );
        m_chart->replaceCoordinatePlane( polarPlane );
        m_model = new TableModel( this );
        m_model->loadFromCSV( ":/data" );
        m_polar = new PolarDiagram();
        m_polar->setModel( m_model );
        m_chart->coordinatePlane()->replaceDiagram( m_polar );
    }

    void testPolarDiagramsSettings()
    {
        QVERIFY( m_polar->rotateCircularLabels() == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::NorthWest ) == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::North ) == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::NorthEast ) == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::West ) == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::East ) == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::SouthWest ) == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::South ) == false );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::SouthEast ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::NorthWest ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::North ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::NorthEast ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::West ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::East ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::SouthWest ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::South ) == false );
        QVERIFY( m_polar->showLabelsAtPosition( Position::SouthEast ) == false );
        qDebug() << "void PolarDiagram::init() is never called.....(?)";
        m_polar->setRotateCircularLabels(  true );
        m_polar->setShowDelimitersAtPosition( Position::North, true );
        m_polar->setShowDelimitersAtPosition( Position::South, true );
        m_polar->setShowLabelsAtPosition( Position::North, true );
        m_polar->setShowLabelsAtPosition( Position::South, true );
        QVERIFY( m_polar->rotateCircularLabels() == true );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::North ) == true );
        QVERIFY( m_polar->showDelimitersAtPosition( Position::South ) == true );
        QVERIFY( m_polar->showLabelsAtPosition( Position::North ) == true );
        QVERIFY( m_polar->showLabelsAtPosition( Position::South ) == true );
    }


    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    PolarDiagram *m_polar;
    TableModel *m_model;

};

QTEST_MAIN(TestPolarDiagrams)

#include "main.moc"
