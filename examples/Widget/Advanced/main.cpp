/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartWidget>
#include <QApplication>

#include <QDebug>
#include <QVector>

#include "mainwindow.h"

using namespace KDChart;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow window;

    window.resize(800, 600);

    QVector<qreal> vec0;
    vec0.append(-5);
    vec0.append(-4);
    vec0.append(-3);
    vec0.append(-2);
    vec0.append(-1);
    vec0.append(0);
    vec0.append(1);
    vec0.append(2);
    vec0.append(3);
    vec0.append(4);
    vec0.append(5);
    window.widget->setDataset(0, vec0, "Linear");

    QVector<qreal> vec1;
    vec1.append(25);
    vec1.append(16);
    vec1.append(9);
    vec1.append(4);
    vec1.append(1);
    vec1.append(0);
    vec1.append(1);
    vec1.append(4);
    vec1.append(9);
    vec1.append(16);
    vec1.append(25);
    window.widget->setDataset(1, vec1, "Quadratic");

    QVector<qreal> vec2;
    vec2.append(-125);
    vec2.append(-64);
    vec2.append(-27);
    vec2.append(-8);
    vec2.append(-1);
    vec2.append(0);
    vec2.append(1);
    vec2.append(8);
    vec2.append(27);
    vec2.append(64);
    vec2.append(125);
    window.widget->setDataset(2, vec2, "Cubic");

    window.widget->addHeaderFooter("Header West", HeaderFooter::Header, Position::West);
    window.widget->addHeaderFooter("Header Center", HeaderFooter::Header, Position::Center);
    window.widget->addHeaderFooter("Header East", HeaderFooter::Header, Position::East);
    window.widget->addHeaderFooter("Footer West", HeaderFooter::Footer, Position::West);
    window.widget->addHeaderFooter("Footer Center", HeaderFooter::Footer, Position::Center);
    window.widget->addHeaderFooter("Footer East", HeaderFooter::Footer, Position::East);

    window.widget->addLegend(Position::East);
    window.show();

    return app.exec();
}
