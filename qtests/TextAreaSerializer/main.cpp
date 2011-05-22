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

#include <KDChartHeaderFooter>

#include <KDChartSerializer>
#include <KDChartTextAreaSerializer>
#include <KDChartSerializeCollector>
#include <KDXMLTools>

#include <iostream>

using namespace KDChart;

class TestKDChartTextAreaSerializer : public QObject {
  Q_OBJECT
private slots:

    void initTestCase()
    {
        mDocstart = "<kdchart:kdchart/>";
        resetDoc();
    }

    void testHeaderFooter()
    {
        resetDoc();

        QDomElement savedElement =
                mDoc.createElement( "TESTING" );
        mDocRoot.appendChild( savedElement );

        HeaderFooter orgFooter( 0 );
        orgFooter.setType( HeaderFooter::Footer );
        orgFooter.setPosition( Position::SouthEast );
        orgFooter.setText( "Nix Nought Nothing" );

        HeaderFooterList areas;
        areas.append( &orgFooter );


        // Note: We do not instantiate a Serializer object, so we
        //       must register the built-in factories explicitly:
        Serializer::registerBuiltInSerializerFactories();


        TextAreaSerializer::saveHeadersFooters(
                mDoc,
                savedElement,
                areas,
                "kdchart:headers-footers" );
        SerializeCollector::instance()->appendDataToElement(
                mDoc, mDocRoot );
        // use cout rather that qDebug() to avoid the length limitation of the later
        //std::cout << "\n\n" << mDoc.toString(2).toLatin1().data() << "\n\n";


        // prepare parsing
        QVERIFY( SerializeCollector::initializeGlobalPointers( mDocRoot ) );

        HeaderFooter* parsedFooter=0;
        QVERIFY( TextAreaSerializer::parseHeaderFooter(
                        mDocRoot, savedElement.firstChild().firstChild(), parsedFooter ) );

        QVERIFY( orgFooter.compare( *parsedFooter ) );
    }


    void cleanupTestCase()
    {
        // this space left empty intentionally
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
};

QTEST_MAIN(TestKDChartTextAreaSerializer)

#include "main.moc"
