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

#include <QtTest/QtTest>

class TestDrawIntoPainter : public QObject
{
    Q_OBJECT
private slots:

    void initDrawIntoPainter()
    {
        MainWindow mainWindow;
        mainWindow.show();
    }

    void testOriginal()
    {
        mainWindow.updateData(tr(":/original"));
    }

    void testTest()
    {
        mainWindow.updateData(tr(":/test"));
    }

private:
    MainWindow mainWindow;
};

QTEST_MAIN(TestDrawIntoPainter)

#include "main.moc"
