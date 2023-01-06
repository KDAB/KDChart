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

#include <KDChartAbstractCoordinatePlane>
#include <KDChartChart>
#include <KDChartLegend>
#include <KDChartLineDiagram>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart();
    m_chart->setGlobalLeadingTop(10);
    m_chart->setGlobalLeadingRight(10);
    chartLayout->addWidget(m_chart);
    hSBar->setVisible(false);
    vSBar->setVisible(false);

    m_model.loadFromCSV(":/data");

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel(&m_model);

    auto *xAxis = new CartesianAxis(m_lines);
    auto *yAxis = new CartesianAxis(m_lines);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    yAxis->setPosition(KDChart::CartesianAxis::Left);

    xAxis->setTitleText("Abscissa axis at the bottom");
    yAxis->setTitleText("Ordinate axis at the left side");

    m_lines->addAxis(xAxis);
    m_lines->addAxis(yAxis);
    m_chart->coordinatePlane()->replaceDiagram(m_lines);

    connect(m_chart, SIGNAL(propertiesChanged()), SLOT(applyNewZoomParameters()));

    // Set up the legend
    m_legend = new Legend(m_lines, m_chart);
    m_chart->addLegend(m_legend);
    m_legend->setPosition(KDChartEnums::PositionEast);
    m_legend->setAlignment(Qt::AlignTop);
}

void MainWindow::on_zoomFactorSB_valueChanged(double factor)
{
    if (factor > 1) {
        hSBar->setVisible(true);
        vSBar->setVisible(true);
    } else {
        hSBar->setValue(500);
        vSBar->setValue(500);
        hSBar->setVisible(false);
        vSBar->setVisible(false);
    }
    m_chart->coordinatePlane()->setZoomFactorX(factor);
    m_chart->coordinatePlane()->setZoomFactorY(factor);
    m_chart->update();
}

void MainWindow::on_adjustGridCB_toggled(bool checked)
{
    static_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane())
        ->setAutoAdjustGridToZoom(checked);
    m_chart->update();
}

void MainWindow::on_rubberBandZoomCB_toggled(bool checked)
{
    m_chart->coordinatePlane()->setRubberBandZoomingEnabled(checked);
}

void MainWindow::on_hSBar_valueChanged(int hPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hPos / 1000.0, vSBar->value() / 1000.0));
    m_chart->update();
}

void MainWindow::on_vSBar_valueChanged(int vPos)
{
    m_chart->coordinatePlane()->setZoomCenter(QPointF(hSBar->value() / 1000.0, vPos / 1000.0));
    m_chart->update();
}

void MainWindow::applyNewZoomParameters()
{
    hSBar->blockSignals(true);
    vSBar->blockSignals(true);

    hSBar->setValue(qRound(m_chart->coordinatePlane()->zoomCenter().x() * 1000));
    vSBar->setValue(qRound(m_chart->coordinatePlane()->zoomCenter().y() * 1000));
    zoomFactorSB->setValue(m_chart->coordinatePlane()->zoomFactorX());

    hSBar->blockSignals(false);
    vSBar->blockSignals(false);
}
