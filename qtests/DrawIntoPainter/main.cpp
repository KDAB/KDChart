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

#include "mainwindow.h"

#include <QtTest/QtTest>

class TestDrawIntoPainter: public QObject {
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

QTEST_MAIN(TestDrawIntoPainter);

#include "main.moc"
