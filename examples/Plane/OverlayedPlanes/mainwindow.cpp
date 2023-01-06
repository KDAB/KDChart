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
#include <KDChartGridAttributes>
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
    chartLayout->addWidget(m_chart);

    m_model.loadFromCSV(":/data");
    m_model2.loadFromCSV(":/data2");

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel(&m_model);

    m_lines2 = new LineDiagram();
    m_lines2->setModel(&m_model2);

    // We call this "plane2" just for remembering, that we use it
    // in addition to the plane, that's built-in by default.
    plane2 = new CartesianCoordinatePlane(m_chart);

    plane2->setReferenceCoordinatePlane(m_chart->coordinatePlane());

    auto *xAxis = new CartesianAxis(m_lines2);
    auto *yAxis = new CartesianAxis(m_lines);
    auto *yAxis2 = new CartesianAxis(m_lines2);

    xAxis->setPosition(KDChart::CartesianAxis::Top);
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    yAxis2->setPosition(KDChart::CartesianAxis::Left);

    m_lines->addAxis(yAxis);

    m_lines2->addAxis(xAxis);
    m_lines2->addAxis(yAxis2);

    m_chart->coordinatePlane()->replaceDiagram(m_lines);
    plane2->replaceDiagram(m_lines2);
    m_chart->addCoordinatePlane(plane2 /*, 1*/);
    m_chart->setGlobalLeading(20, 20, 20, 20);
    init();
}

void MainWindow::init()
{
    on_displayGrid2CB_toggled(false);
}

void MainWindow::on_displayGrid1CB_toggled(bool checked)
{
    auto *plane = qobject_cast<CartesianCoordinatePlane *>(m_chart->coordinatePlane());

    // disable grid for plane
    GridAttributes gv(plane->gridAttributes(Qt::Vertical));
    gv.setGridVisible(checked);
    plane->setGridAttributes(Qt::Vertical, gv);
    plane->setGridAttributes(Qt::Horizontal, gv);
}

void MainWindow::on_displayGrid2CB_toggled(bool checked)
{
    // disable grid for plane2
    GridAttributes gv(plane2->gridAttributes(Qt::Vertical));
    gv.setGridVisible(checked);
    plane2->setGridAttributes(Qt::Vertical, gv);
    plane2->setGridAttributes(Qt::Horizontal, gv);
}
