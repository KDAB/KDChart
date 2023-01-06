/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartFrameAttributes>
#include <KDChartLineDiagram>
#include <QtGui>

#include <KDChartCartesianAxis>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLegend>
#include <KDChartPlotter>
#include <QStandardItemModel>

#include <QApplication>

using namespace KDChart;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const int points = 420;
    const double xMin = 0;
    const double xMax = 20;
    const double step = (xMax - xMin) / (points - 1);

    QStandardItemModel model(points, 6);

    double x = xMin;
    for (int n = 0; n < points; ++n, x += step) {
        QModelIndex index = model.index(n, 0);
        model.setData(index, x);
        index = model.index(n, 1);
        model.setData(index, sin(x) * 100.0);

        index = model.index(n, 2);
        model.setData(index, x);
        index = model.index(n, 3);
        model.setData(index, x);

        index = model.index(n, 4);
        model.setData(index, x);
        index = model.index(n, 5);
        model.setData(index, x * x * x);
    }

    model.setHeaderData(0, Qt::Horizontal, QString::fromLatin1("100 * sin(x)"));
    model.setHeaderData(2, Qt::Horizontal, QString::fromLatin1("x"));
    model.setHeaderData(4, Qt::Horizontal, QString::fromLatin1("x^3"));

    auto *chart = new KDChart::Chart();

    KDChart::AbstractCartesianDiagram *diagram = new KDChart::Plotter;
    diagram->setModel(&model);
    chart->coordinatePlane()->replaceDiagram(diagram);

    auto *xAxis = new KDChart::CartesianAxis(diagram);
    auto *yAxis = new KDChart::CartesianAxis(diagram);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    diagram->addAxis(xAxis);
    diagram->addAxis(yAxis);

    auto *legend = new KDChart::Legend(diagram, chart);
    KDChart::FrameAttributes legendAtt = legend->frameAttributes();
    legendAtt.setCornerRadius(9);
    legend->setFrameAttributes(legendAtt);
    legend->setPosition(KDChart::Position::East);
    legend->setAlignment(Qt::AlignCenter);
    legend->setTitleText("Legend");
    chart->addLegend(legend);

    auto *cart_plane = dynamic_cast<KDChart::CartesianCoordinatePlane *>(chart->coordinatePlane());
    Q_ASSERT(cart_plane);

    cart_plane->setAxesCalcModeX(KDChart::AbstractCoordinatePlane::Logarithmic);
    cart_plane->setAxesCalcModeY(KDChart::AbstractCoordinatePlane::Logarithmic);

    // Set the vertical range from 15 to 75 - with a logarithmic axis I actually get 1 to 100
    // cart_plane->setVerticalRange(QPair<qreal,qreal>( 0.005, 1000 ) );

    // Set the horizontal range from 1 to 9 - with a linear axis this works OK
    cart_plane->setHorizontalRange(QPair<qreal, qreal>(0.001, 100));

    chart->show();

    int ret = app.exec();

    delete chart;

    return ret;
}
