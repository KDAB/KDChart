/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <QtGui>

#include <KDChartBackgroundAttributes>
#include <KDChartChart>
#include <KDChartDataValueAttributes>
#include <KDChartFrameAttributes>
#include <KDChartGridAttributes>
#include <KDChartLegend>
#include <KDChartMarkerAttributes>
#include <KDChartPlotter>
#include <KDChartTextAttributes>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KDChart::Chart chart;

    QStandardItemModel model;

    const double offset = 600;
    model.setRowCount(100);
    model.setColumnCount(2); // 1 data set
    for (int i = 0; i < 100; ++i) {
        double t = i + offset;
        double v = i + offset;
        QModelIndex index = model.index(i, 0);
        model.setData(index, t);
        index = model.index(i, 1);
        model.setData(index, v);
    }

    model.setHeaderData(0, Qt::Horizontal, "Dataset 1");

    // general chart layout
    KDChart::FrameAttributes fm = chart.frameAttributes();
    fm.setVisible(true);
    fm.setPen(QPen(Qt::black));
    chart.setFrameAttributes(fm);
    chart.setGlobalLeading(10, 0, 10, 10);

    KDChart::BackgroundAttributes chart_bg;
    chart_bg.setBrush(Qt::white);
    chart_bg.setVisible(true);
    chart.setBackgroundAttributes(chart_bg);

    // coordinate plane setup
    KDChart::AbstractCoordinatePlane *plane1 = chart.coordinatePlane();
    plane1->setRubberBandZoomingEnabled(true);

    // create cartesian diagrams
    auto *plotter = new KDChart::Plotter;
    plotter->setAntiAliasing(false);
    plotter->setModel(&model);
    plane1->replaceDiagram(plotter);

    // customize grids
    auto *cp1 = static_cast<KDChart::CartesianCoordinatePlane *>(plane1);
    KDChart::GridAttributes gv = cp1->gridAttributes(Qt::Vertical);
    QPen gridPen(QColor(200, 100, 100));
    gridPen.setStyle(Qt::DashLine);
    gv.setGridPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    gridPen.setColor(QColor(255, 155, 155));
    gv.setSubGridPen(gridPen);
    cp1->setGridAttributes(Qt::Vertical, gv);

    // Enable isometric scaling
    cp1->setIsometricScaling(true);

    // axis
    auto *xAxis = new KDChart::CartesianAxis(plotter);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    xAxis->setTitleText("X-Title");

    KDChart::TextAttributes att = xAxis->titleTextAttributes();
    QFont f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize(KDChart::Measure(16));
    xAxis->setTitleTextAttributes(att);

    auto *y1Axis = new KDChart::CartesianAxis(plotter);
    y1Axis->setPosition(KDChart::CartesianAxis::Left);
    y1Axis->setTitleText("Y-Title");

    att = y1Axis->titleTextAttributes();
    f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize(KDChart::Measure(16));
    y1Axis->setTitleTextAttributes(att);

    // add the axis to the plotter
    plotter->addAxis(xAxis);
    plotter->addAxis(y1Axis);

    // create legend
    auto *legend = new KDChart::Legend(plotter, &chart);
    chart.addLegend(legend);
    att = legend->textAttributes();
    f = att.font();
    f.setBold(false);
    att.setFont(f);
    att.setAutoShrink(true);
    legend->setTextAttributes(att);

    legend->setPosition(KDChart::Position::East);
    legend->setAlignment(Qt::AlignCenter);
    legend->setTitleText("Curves");
    att = legend->titleTextAttributes();
    f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize(KDChart::Measure(16));
    legend->setTitleTextAttributes(att);

    KDChart::BackgroundAttributes legend_bg;
    legend_bg.setBrush(Qt::white);
    legend_bg.setVisible(true);
    legend->setBackgroundAttributes(legend_bg);

    KDChart::DataValueAttributes attr = plotter->dataValueAttributes();
    KDChart::TextAttributes tattr = attr.textAttributes();
    tattr.setFontSize(KDChart::Measure(12, KDChartEnums::MeasureCalculationModeAbsolute));
    tattr.setRotation(0);
    attr.setTextAttributes(tattr);
    plotter->setDataValueAttributes(attr);

    // customize marker properties

    // Dataset 1 : green, MarkerRing, no line
    QColor SERIES_1_OUTLINE = QColor(0, 128, 0);
    attr = plotter->dataValueAttributes(0);
    KDChart::MarkerAttributes mattr = attr.markerAttributes();
    mattr.setMarkerColor(SERIES_1_OUTLINE);
    //        mattr.setMarkerStyle( KDChart::MarkerAttributes::MarkerRing );
    mattr.setMarkerSize(QSizeF(6.0, 6.0));
    mattr.setVisible(true);
    attr.setMarkerAttributes(mattr);
    attr.setVisible(true);
    plotter->setDataValueAttributes(0, attr);
    plotter->setPen(0, Qt::NoPen);

    chart.show();
    return a.exec();
}
