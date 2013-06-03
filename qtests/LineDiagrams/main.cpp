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
#include <KDChartLineDiagram>
#include <KDChartThreeDLineAttributes>
#include <KDChartCartesianCoordinatePlane>

#include <TableModel.h>

using namespace KDChart;

class TestLineDiagrams: public QObject {
    Q_OBJECT
private slots:

    void initTestCase()
    {
        m_chart = new Chart(0);
        m_model = new TableModel( this );
        m_model->loadFromCSV( ":/data" );
        m_lines = new LineDiagram();
        m_lines->setModel( m_model );
        m_chart->coordinatePlane()->replaceDiagram( m_lines );
    }

    void testLineDiagramType()
    {
        QCOMPARE( m_lines->type(), LineDiagram::Normal );
        m_lines->setType( LineDiagram::Stacked );
        QCOMPARE(  m_lines->type(),  LineDiagram::Stacked );
        m_lines->setType( LineDiagram::Percent );
        QCOMPARE(  m_lines->type(),  LineDiagram::Percent );
        //reset to normal
        m_lines->setType( LineDiagram::Normal );
    }

    void testLineAttributesLevelSettings()
    {
        //check segments
        const int rows = m_lines->model()->rowCount();
        QCOMPARE( m_lines->numberOfAbscissaSegments(), rows );
        const int cols = m_lines->model()->columnCount();
        QCOMPARE( m_lines->numberOfOrdinateSegments(), cols );
        QModelIndex idx = m_model->index(rows-3, cols-3, QModelIndex());
        // create attribut
        LineAttributes la( m_lines->lineAttributes() );
        LineAttributes laCol( m_lines->lineAttributes() );
        LineAttributes laIndex( m_lines->lineAttributes() );

        // modify at different level and compare
        laCol.setMissingValuesPolicy ( LineAttributes::MissingValuesHideSegments );
        laIndex.setMissingValuesPolicy ( LineAttributes::MissingValuesShownAsZero );
        m_lines->setLineAttributes( la );
        m_lines->setLineAttributes( cols-2,  laCol );
        m_lines->setLineAttributes( idx,  laIndex );
        QVERIFY( m_lines->lineAttributes() != m_lines->lineAttributes(cols-2) );
        QVERIFY( m_lines->lineAttributes() != m_lines->lineAttributes(idx) );
        QVERIFY( m_lines->lineAttributes(cols-2) != m_lines->lineAttributes(idx) );
        QCOMPARE( m_lines->lineAttributes(),  la );
        QCOMPARE( m_lines->lineAttributes( cols - 2 ),  laCol );
        QCOMPARE( m_lines->lineAttributes( idx ),  laIndex );
        // try and override the cols and index level - should not work
        m_lines->setLineAttributes( la );
        QVERIFY( m_lines->lineAttributes().missingValuesPolicy() ==
                 LineAttributes::MissingValuesAreBridged );
        QVERIFY( m_lines->lineAttributes( cols-2 ).missingValuesPolicy() ==
                 LineAttributes::MissingValuesHideSegments );
        QVERIFY( m_lines->lineAttributes( idx ).missingValuesPolicy() ==
                 LineAttributes::MissingValuesShownAsZero );
    }

    void testLineAttributesValueSettings()
    {
        LineAttributes la( m_lines->lineAttributes() );

        // check default values
        QVERIFY( la.missingValuesPolicy() == LineAttributes::MissingValuesAreBridged  );
        QVERIFY( la.displayArea() == false );
        QVERIFY( la.transparency() ==  255 );
        //change settings
        la.setMissingValuesPolicy ( LineAttributes::MissingValuesShownAsZero );
        la.setDisplayArea( true );
        la.setTransparency( 100 );
        m_lines->setLineAttributes(  la );
        // get new values
        QVERIFY( m_lines->lineAttributes().missingValuesPolicy() ==
                 LineAttributes::MissingValuesShownAsZero  );
        QVERIFY( m_lines->lineAttributes().displayArea() == true );
        QVERIFY( m_lines->lineAttributes().transparency() == 100 );
    }

    void testThreeDLineAttributesLevelSettings()
    {
        //check segments
        const int rows = m_lines->model()->rowCount();
        QCOMPARE( m_lines->numberOfAbscissaSegments(), rows );
        const int cols = m_lines->model()->columnCount();
        QCOMPARE( m_lines->numberOfOrdinateSegments(), cols );
        QModelIndex idx = m_model->index(rows-3, cols-3, QModelIndex());
        // create attribut
        ThreeDLineAttributes td( m_lines->threeDLineAttributes() );
        ThreeDLineAttributes tdCol( m_lines->threeDLineAttributes() );
        ThreeDLineAttributes tdIndex( m_lines->threeDLineAttributes() );
        // modify at different level and compare
        tdCol.setDepth(25 );
        tdIndex.setDepth( 30 );
        m_lines->setThreeDLineAttributes( td  );
        m_lines->setThreeDLineAttributes( cols-2,  tdCol );
        m_lines->setThreeDLineAttributes( idx,  tdIndex );
        QVERIFY( m_lines->threeDLineAttributes() !=
                 m_lines->threeDLineAttributes(cols-2) );
        QVERIFY( m_lines->threeDLineAttributes() !=
                 m_lines->threeDLineAttributes(idx) );
        QVERIFY( m_lines->threeDLineAttributes(cols-2) !=
                 m_lines->threeDLineAttributes(idx) );
        QCOMPARE( m_lines->threeDLineAttributes(),  td );
        QCOMPARE( m_lines->threeDLineAttributes( cols - 2 ),  tdCol );
        QCOMPARE( m_lines->threeDLineAttributes( idx ),  tdIndex );
        // try and override the cols and index level - should not work
        m_lines->setThreeDLineAttributes( td );
        QVERIFY( m_lines->threeDLineAttributes().depth() == 20 );
        QVERIFY( m_lines->threeDLineAttributes( cols-2 ).depth() == 25 );
        QVERIFY( m_lines->threeDLineAttributes( idx ).depth() == 30 );
    }

    void testThreeDLineAttributesValueSettings()
    {
        ThreeDLineAttributes td( m_lines->threeDLineAttributes() );

        //check default values
        //generics == AbstractThreeD
        QVERIFY( td.isEnabled() == false );
        QVERIFY( td.depth() == 20 );
        QVERIFY( td.validDepth() == 0.0 );
        //bars specifics
        QVERIFY( td.lineXRotation() == 15 );
        QVERIFY( td.lineYRotation() == 15 );

        //set new values
        td.setEnabled(  true );
        td.setDepth( 40 );
        td.setLineXRotation( 20 );
        td.setLineYRotation( 25 );
        m_lines->setThreeDLineAttributes( td );

        //get new values
        QVERIFY( m_lines->threeDLineAttributes().isEnabled() == true );
        QVERIFY( m_lines->threeDLineAttributes().depth() == 40 );
        QVERIFY( m_lines->threeDLineAttributes().validDepth() == 40 );
        QVERIFY( m_lines->threeDLineAttributes().lineXRotation() == 20 );
        QVERIFY( m_lines->threeDLineAttributes().lineYRotation() == 25 );
    }

    void cleanupTestCase()
    {
    }

private:
    Chart *m_chart;
    LineDiagram *m_lines;
    TableModel *m_model;

};

QTEST_MAIN(TestLineDiagrams)

#include "main.moc"
