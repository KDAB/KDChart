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

#include <QtDebug>
#include <QObject>
#include <QtTest>

#include "KDChartSignalCompressor.h"

class SignalCompressorTest : public QObject
{
    Q_OBJECT

public:
    SignalCompressorTest()
        : QObject()
        , m_count( 0 )
        , m_compressor( this, SIGNAL( theSignal() ), this )
    {   // receive our own signal, once it is emitted
        connect( this, SIGNAL( theSignal() ),
                 SLOT( receiveIt() ) );
    }

private:
    int m_count;
    KDChart::SignalCompressor m_compressor;

signals:
    void theSignal();

public slots: // no tests!
    void receiveIt()
    {
        ++m_count;
    }

private slots:
    void cleanup()
    {
        m_count = 0;
    }

    void compressSignalOnceTest()
    {
        QVERIFY( m_count == 0 );
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        QTest::qWait( 100 );
        QVERIFY( m_count == 1 );
    }

    void compressSignalTwiceTest()
    {
        QVERIFY( m_count == 0 );
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        QVERIFY( m_count == 0 );
        QTest::qWait( 100 );
        QVERIFY( m_count == 1 );
        m_compressor.emitSignal();
        m_compressor.emitSignal();
        QVERIFY( m_count == 1 );
        QTest::qWait( 100 );
        QVERIFY( m_count == 2 );
    }
};

QTEST_MAIN( SignalCompressorTest );

#include "main.moc"
