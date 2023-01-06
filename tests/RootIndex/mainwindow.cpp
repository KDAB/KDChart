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
#include <KDChartBarDiagram>
#include <KDChartChart>
#include <KDChartLineDiagram>

#include <QDebug>
#include <QPainter>

using namespace KDChart;

#define USE_ROOTINDEX 1

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart();
    chartLayout->addWidget(m_chart);

#if USE_ROOTINDEX
    m_model.insertRows(0, 2, QModelIndex());
    m_model.insertColumns(0, 1, QModelIndex());
    QModelIndex idx1 = m_model.index(0, 0, QModelIndex());
    QModelIndex idx2 = m_model.index(1, 0, QModelIndex());

    m_model.setData(idx1, 3.14);
    m_model.setData(idx2, 2 * 3.14);

    m_model.insertRows(0, 5, idx1);
    m_model.insertColumns(0, 5, idx1);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            m_model.setData(m_model.index(i, j, idx1), ( qreal )i * j);
        }
    }

    m_model.insertRows(0, 2, idx2);
    m_model.insertColumns(0, 2, idx2);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            m_model.setData(m_model.index(i, j, idx2), 10. * (i + 1.) / (j + 1.));
        }
    }
#else
    QModelIndex idx1 = QModelIndex();
    m_model.insertRows(0, 5, idx1);
    m_model.insertColumns(0, 5, idx1);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            m_model.setData(m_model.index(i, j, idx1), ( qreal )i * j);
        }
    }
    QModelIndex idx2 = idx1;

    m_model2.insertRows(0, 2, idx2);
    m_model2.insertColumns(0, 2, idx2);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            m_model2.setData(m_model2.index(i, j, idx2), 10. * (i + 1.) / (j + 1.));
        }
    }

#endif

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel(&m_model);
#if USE_ROOTINDEX
    m_lines->setRootIndex(idx1);
#endif

    m_bars = new BarDiagram();
#if USE_ROOTINDEX
    m_bars->setModel(&m_model);
    m_bars->setRootIndex(idx2);
#else
    m_bars->setModel(&m_model2);
#endif

    plane = new CartesianCoordinatePlane(m_chart);

    auto *xAxis = new CartesianAxis(m_lines);
    auto *yAxis = new CartesianAxis(m_lines);
    auto *yAxis3 = new CartesianAxis(m_lines);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    yAxis3->setPosition(KDChart::CartesianAxis::Left);

    auto *yAxis2 = new CartesianAxis(m_bars);
    yAxis2->setPosition(KDChart::CartesianAxis::Right);

    // explicitly add it to the second diagram, we want to share it
    m_bars->addAxis(xAxis);

    m_chart->coordinatePlane()->replaceDiagram(m_lines);
    plane->replaceDiagram(m_bars);
    // We want both planes to use the same space.
    plane->setReferenceCoordinatePlane(m_chart->coordinatePlane());
    m_chart->addCoordinatePlane(plane /*, 1*/);
}
