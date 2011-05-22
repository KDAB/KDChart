/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include <KDChartCartesianAxis>

#include <KDChartSerializer>
#include <KDChartAxesSerializer>
#include <KDChartSerializeCollector>
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

        // Note: We do not instantiate a Serializer object, so we
        //       must register the built-in factories explicitly:
        Serializer::registerBuiltInSerializerFactories();
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

        QList<const KDChart::AbstractAxis*> axes;
        axes.append( &orgAxis );

        mAxesS->saveAxes( mDoc, savedElement,
                                   axes,
                                   "kdchart:axes" );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );
        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";


        // prepare parsing
        QVERIFY( SerializeCollector::initializeGlobalPointers( mDocRoot ) );

        AbstractAxis* parsedAxis=0;
        QVERIFY( mAxesS->parseAxis(
                        mDocRoot, savedElement.firstChild().firstChild(), parsedAxis ) );

        QVERIFY( qobject_cast< CartesianAxis * >( parsedAxis ) != 0 );
        QVERIFY( orgAxis.compare( qobject_cast< CartesianAxis * >( parsedAxis ) ) );
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
