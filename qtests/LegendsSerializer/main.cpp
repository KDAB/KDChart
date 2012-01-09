/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
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
#include <TableModel.h>
#include <KDChartGlobal>

#include <KDChartBackgroundAttributes>
#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartPosition>

#include <KDChartSerializer>
#include <KDChartLegendsSerializer>
#include <KDChartSerializeCollector>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartLegendsSerializer : public QObject {
  Q_OBJECT
private slots:

    void initTestCase()
    {
        mDocstart = "<kdchart:kdchart/>";
        resetDoc();

        m_chart = new Chart(0);
        TableModel *tableModel = new TableModel( this );
        tableModel->loadFromCSV( ":/data" );
        tableModel->setSupplyHeaderData( false );

        m_lines = new LineDiagram();
        m_lines->setModel( tableModel );

        m_legend = new Legend( m_lines, m_chart );
        m_legend->setVisible( true );
        m_legend->setReferenceArea( m_lines );
        m_legend->setPosition( Position::NorthEast );
        m_legend->setAlignment( Qt::AlignCenter );
        m_legend->setShowLines( false );
        m_legend->setTitleText( tr( "Thee Larch" ) );
        m_legend->setOrientation( Qt::Vertical );
        m_chart->addLegend( m_legend );

        MarkerAttributes lma ( m_legend->markerAttributes( 0 ) );
        lma.setMarkerStyle( MarkerAttributes::MarkerDiamond );
        m_legend->setMarkerAttributes( 0,  lma );
        lma.setMarkerStyle( MarkerAttributes::MarkerCircle );
        m_legend->setMarkerAttributes( 1,  lma );

        // Configure Legend Title and labels
        m_legend->setTitleText( "Lines" );
        m_legend->setText( 0,  "Series 1" );
        m_legend->setText( 1,  "Series 2" );
        m_legend->setText( 2,  "Series 3" );

        // adjust the legend item's font:
        TextAttributes lta( m_legend->textAttributes() );
        lta.setPen( QPen( Qt::darkGray ) );
        Measure me( lta.fontSize() );
        me.setValue( me.value() * 1.5 );
        me.setReferenceArea( 0 );
        lta.setFontSize( me );
        m_legend->setTextAttributes(  lta );

        // adjust the legend title's font:
        lta = m_legend->titleTextAttributes();
        lta.setPen( QPen( Qt::darkGray ) );
        me = lta.fontSize();
        me.setValue( me.value() * 1.5 );
        me.setReferenceArea( 0 );
        lta.setFontSize( me );
        m_legend->setTitleTextAttributes(  lta );

        // Configure a pen to surround
        // one of the markers with a border
        QPen markerPen;
        markerPen.setColor(  Qt::darkGray );
        markerPen.setWidth( 2 );
        m_legend->setPen( 1, markerPen );

        // Add a background to your legend ...
        BackgroundAttributes ba;
        ba.setBrush(  Qt::white );
        ba.setVisible( true );
        m_legend->setBackgroundAttributes(  ba );

        // ... and add a frame:
        FrameAttributes fa;
        fa.setPen( markerPen );
        fa.setPadding( 5 );
        fa.setVisible( true );
        m_legend->setFrameAttributes(  fa );
    }

    void testLegend()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );


        // Note: We do not instantiate a Serializer object, so we
        //       must register the built-in factories explicitly:
        Serializer::registerBuiltInSerializerFactories();


        LegendsSerializer::saveLegends(
                mDoc,
                savedElement,
                m_chart->legends(),
                "kdchart:legends" );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );

        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";


        // prepare parsing
        QVERIFY( SerializeCollector::initializeGlobalPointers( mDocRoot ) );

        Legend* parsedLegend=0;
        QVERIFY( LegendsSerializer::parseLegend(
                        mDocRoot, savedElement.firstChild().firstChild(), parsedLegend ) );

        QVERIFY( m_legend->compare( parsedLegend ) );
    }


    void cleanupTestCase()
    {
        delete m_chart;
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
    Legend *m_legend;
    LineDiagram *m_lines;
};

QTEST_MAIN(TestKDChartLegendsSerializer)

#include "main.moc"
