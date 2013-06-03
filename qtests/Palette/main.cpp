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

#include <KDChartPalette>
#include <QtTest/QtTest>

using namespace KDChart;

class TestKDChartPalette: public QObject {
  Q_OBJECT
private slots:

  void testKDChartPaletteInit()
  {
    Palette p;
    QCOMPARE( p.isValid(), false );
    Palette *p2 = new Palette();
    QCOMPARE( p2->isValid(), false );
    QCOMPARE( p2->size(), 0 );
    delete p2;
  }

  void testKDChartPaletteAddBrush()
  {
    Palette p;
    p.addBrush( Qt::red );
    QCOMPARE( p.isValid(), true );
    QCOMPARE( p.size(), 1 );
    // make sure adding a brush at an invalid position does the right thing
    p.addBrush( Qt::blue, 7 );
    QCOMPARE( p.isValid(), true );
    QCOMPARE( p.size(), 2 );
  }

  void testKDChartPaletteGetBrush()
  {
    Palette p;
    p.addBrush( Qt::red );
    p.addBrush( Qt::blue, 7 );
    QCOMPARE( p.isValid(), true );
    QCOMPARE( p.size(), 2 );
    QBrush brush = p.getBrush( 1 );
    // work around the lack of qCompare for QBrushes
    bool result = brush == Qt::blue;
    QCOMPARE( result, true );
    // test wrap around
    brush = p.getBrush( 2 );
    result = brush == Qt::red;
    QCOMPARE( result, true );
  }

  void testKDChartPaletteRemoveBrush()
  {
    Palette p;
    p.addBrush( Qt::red );
    p.removeBrush( 2 );
    QCOMPARE( p.size(), 1 );
    QCOMPARE( p.isValid(), true );
    p.removeBrush( 0 );
    QCOMPARE( p.isValid(), false );
  }

  void testKDChartPaletteDefaultPalette()
  {
    const Palette def = Palette::defaultPalette();
    QBrush brush = def.getBrush( 5 );
    bool result = brush == Qt::yellow;
    QCOMPARE( result, true );
  }

  void testKDChartPaletteSuduedPalette()
  {
    const Palette def = Palette::subduedPalette();
    QBrush brush = def.getBrush( 5 );
    bool result = brush == QColor( 0x86,0xe0,0x70 );
    QCOMPARE( result, true );
  }

  void testKDChartPaletteRainbowPalette()
  {
    const Palette def = Palette::rainbowPalette();
    QBrush brush = def.getBrush( 5 );
    bool result = brush == Qt::cyan;
    QCOMPARE( result, true );
  }

  void testKDChartPaletteCopying()
  {
    const Palette def = Palette::rainbowPalette();
    const Palette other = def;
    QBrush brush = other.getBrush( 5 );
    bool result = brush == Qt::cyan;
    QCOMPARE( result, true );
    QCOMPARE( def.size(), other.size() );
  }

};

QTEST_MAIN(TestKDChartPalette)

#include "main.moc"
