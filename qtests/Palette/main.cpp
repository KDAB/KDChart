/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartPalette>
#include <QtTest/QtTest>

using namespace KDChart;

class TestKDChartPalette : public QObject
{
    Q_OBJECT
private slots:

    void testKDChartPaletteInit()
    {
        Palette p;
        QCOMPARE(p.isValid(), false);
        auto *p2 = new Palette();
        QCOMPARE(p2->isValid(), false);
        QCOMPARE(p2->size(), 0);
        delete p2;
    }

    void testKDChartPaletteAddBrush()
    {
        Palette p;
        p.addBrush(Qt::red);
        QCOMPARE(p.isValid(), true);
        QCOMPARE(p.size(), 1);
        // make sure adding a brush at an invalid position does the right thing
        p.addBrush(Qt::blue, 7);
        QCOMPARE(p.isValid(), true);
        QCOMPARE(p.size(), 2);
    }

    void testKDChartPaletteGetBrush()
    {
        Palette p;
        p.addBrush(Qt::red);
        p.addBrush(Qt::blue, 7);
        QCOMPARE(p.isValid(), true);
        QCOMPARE(p.size(), 2);
        QBrush brush = p.getBrush(1);
        // work around the lack of qCompare for QBrushes
        bool result = brush == Qt::blue;
        QCOMPARE(result, true);
        // test wrap around
        brush = p.getBrush(2);
        result = brush == Qt::red;
        QCOMPARE(result, true);
    }

    void testKDChartPaletteRemoveBrush()
    {
        Palette p;
        p.addBrush(Qt::red);
        p.removeBrush(2);
        QCOMPARE(p.size(), 1);
        QCOMPARE(p.isValid(), true);
        p.removeBrush(0);
        QCOMPARE(p.isValid(), false);
    }

    void testKDChartPaletteDefaultPalette()
    {
        const Palette def = Palette::defaultPalette();
        QBrush brush = def.getBrush(5);
        bool result = brush == Qt::yellow;
        QCOMPARE(result, true);
    }

    void testKDChartPaletteSuduedPalette()
    {
        const Palette def = Palette::subduedPalette();
        QBrush brush = def.getBrush(5);
        bool result = brush == QColor(0x86, 0xe0, 0x70);
        QCOMPARE(result, true);
    }

    void testKDChartPaletteRainbowPalette()
    {
        const Palette def = Palette::rainbowPalette();
        QBrush brush = def.getBrush(5);
        bool result = brush == Qt::cyan;
        QCOMPARE(result, true);
    }

    void testKDChartPaletteCopying()
    {
        const Palette def = Palette::rainbowPalette();
        const Palette other = def;
        QBrush brush = other.getBrush(5);
        bool result = brush == Qt::cyan;
        QCOMPARE(result, true);
        QCOMPARE(def.size(), other.size());
    }
};

QTEST_MAIN(TestKDChartPalette)

#include "main.moc"
