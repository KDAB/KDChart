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

#include <KDXMLTools>

#include <QtTest/QtTest>
#include <QVariant>
#include <QRect>
#include <QSize>
#include <QPoint>

class XmlToolsTest : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase()
    {
    }

    void testBool()
    {
        runTests( QVariantList() << QVariant( true ) << QVariant( false ) );
    }

    void testQString()
    {
        runTests( QVariantList()
                  << QVariant( QString() ) // an empty one
                  << QVariant( QString( "some text" ) ) );
    }

    void testQRect()
    {
        runTests( QVariantList()
                  << QRect()
                  << QRect( 0, 0, 0, 0 )
                  << QRect( 0, 0, -10, 30 )
                  << QRect( 10, 20, 30, 40 ) );
    }

    void testInt()
    {
        runTests( QVariantList() << -1 << 0 << 1 << 2 << 1000 << 113445567 );
    }

    void testQPoint()
    {
        runTests( QVariantList() << QPoint() << QPoint( 0, 0 ) << QPoint( 1, 1 ) << QPoint( -1, 20 ) );
    }

    void testQSize()
    {
        runTests( QVariantList() << QSize() << QSize( 0, 0 ) << QSize( 1, 1 ) << QSize( -10, 10 ) << QSize( 23345, 345451 ) );
    }

    void cleanupTestCase()
    {
    }

private:
    void runTests( const QVariantList& variants )
    {
        static const QString name( "varianttests" );
        QDomDocument doc( "test" );
        QDomElement root = doc.createElement( "properties" );
        doc.appendChild( root );

        Q_FOREACH( const QVariant& value, variants ) {
            // serialize value, deserialize it, and compare the results
            KDXML::createQVariantNode( doc, root, name, value );
        }

        QDomNodeList elements = root.childNodes();
        QCOMPARE( variants.size(), elements.size() );

        int at = 0;
        Q_FOREACH( const QVariant& value, variants ) {
            QDomElement element = elements.at( at ).toElement();
            ++at;
            QVariant result;
            QString nameResult;
            bool ok = KDXML::readQVariantNode( element, result, nameResult );
            QVERIFY2( ok, "deserializing failed" );
            QCOMPARE( nameResult, name );
            QCOMPARE( value, result );
        }
    }

    QVariantList mTestcases;
    QString mDocstart;
};

QTEST_MAIN(XmlToolsTest)

#include "main.moc"
