/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#include "mainwindow.h"

#include <QElapsedTimer>
#include <QtTest/QtTest>

class TestParamVsParam : public QObject
{
    Q_OBJECT
private slots:

    void testMainWindow()
    {
        QElapsedTimer t;
        t.start();

        MainWindow mainWindow;

        qDebug() << "time constructor:" << t.elapsed() << "ms";

        mainWindow.show();
        QTimer::singleShot(0, qApp, SLOT(quit()));

        qDebug() << "time show():" << t.elapsed() << "ms";
        // uncomment to see it blink:
        // QTest::qWait( 10000 );
    }
};

QTEST_MAIN(TestParamVsParam)

#include "main.moc"
