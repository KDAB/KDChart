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

#include <QApplication>
#include <KDChartWidget>
#include <KDChartLineDiagram>
#include <KDChartCartesianAxis>
#include <KDChartDataValueAttributes>

using namespace KDChart;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Widget widget;
    widget.resize(600, 600);

    QVector<QPair<qreal, qreal>> vec0;
    QVector<qreal> vec1, vec2;

    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;

    // assign your datasets
    // while setting the legend
    // items text
    widget.setDataset(0, vec1, "vec1");

    // add and position
    widget.addLegend(Position::North);

    // configure the axes
    auto *xAxis = new CartesianAxis(widget.lineDiagram());
    auto *yAxis = new CartesianAxis(widget.lineDiagram());
    xAxis->setPosition(CartesianAxis::Bottom);
    yAxis->setPosition(CartesianAxis::Left);
    xAxis->setTitleText("Abscissa bottom position");
    yAxis->setTitleText("Ordinate left position");

    // configure Xaxis labels
    // no need to re-write labels it iterates until all
    // labels are written
    QStringList daysOfWeek;
    daysOfWeek << "Monday"
               << "Tuesday"
               << "Wednesday";
    QStringList shortDays;
    shortDays << "Mon"
              << "Tue"
              << "Wed";

    // set user configured x axis labels
    xAxis->setLabels(daysOfWeek);
    xAxis->setShortLabels(shortDays);

    widget.lineDiagram()->addAxis(xAxis);
    widget.lineDiagram()->addAxis(yAxis);

    // show data values
    DataValueAttributes a(widget.lineDiagram()->dataValueAttributes());
    a.setVisible(true);
    widget.lineDiagram()->setDataValueAttributes(a);

    // make sure data values are displayed at the borders
    widget.setGlobalLeading(20, 20, 20, 20);

    widget.show();

    return app.exec();
}
