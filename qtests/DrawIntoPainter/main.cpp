/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
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
