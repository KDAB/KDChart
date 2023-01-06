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

#include <KDChartChart>
#include <KDChartPieAttributes>
#include <KDChartPieDiagram>
#include <KDChartThreeDPieAttributes>

#include <QDebug>
#include <QTimer>

using namespace KDChart;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    auto *chartLayout = new QHBoxLayout(chartFrame);
    m_chart = new Chart();
    m_chart->setGlobalLeadingLeft(5);
    m_chart->setGlobalLeadingRight(5);
    chartLayout->addWidget(m_chart);

    m_model.loadFromCSV(":/data");
    explodeDatasetSB->setMaximum(m_model.columnCount() - 1);

    // Set up the diagram
    auto *polarPlane = new PolarCoordinatePlane(m_chart);
    m_chart->replaceCoordinatePlane(polarPlane);
    m_pie = new PieDiagram();
    m_pie->setModel(&m_model);
    m_chart->coordinatePlane()->replaceDiagram(m_pie);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotNextFrame()));
}

void MainWindow::on_startPositionSB_valueChanged(double pos)
{
    const int intValue = static_cast<int>(pos);
    startPositionSL->blockSignals(true);
    startPositionSL->setValue(intValue);
    startPositionSL->blockSignals(false);
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos);
    m_chart->update();
}

void MainWindow::on_startPositionSL_valueChanged(int pos)
{
    auto qrealValue = static_cast<qreal>(pos);
    startPositionSB->blockSignals(true);
    startPositionSB->setValue(qrealValue);
    startPositionSB->blockSignals(false);
    static_cast<PolarCoordinatePlane *>(m_chart->coordinatePlane())->setStartPosition(pos);
    m_chart->update();
}

void MainWindow::on_explodeSubmitPB_clicked()
{
    setExplodeFactor(explodeDatasetSB->value(), explodeFactorSB->value());
    m_chart->update();
}

void MainWindow::setExplodeFactor(int column, qreal value)
{
    // Note:
    // We use the per-column getter method here, it will fall back
    // automatically to return the global (or even the default) settings.
    PieAttributes attrs(m_pie->pieAttributes(column));
    attrs.setExplodeFactor(value);
    m_pie->setPieAttributes(column, attrs);
    m_chart->update();
}

void MainWindow::on_animateExplosionCB_toggled(bool toggle)
{
    if (toggle)
        m_timer->start(100);
    else
        m_timer->stop();
}

void MainWindow::slotNextFrame()
{
    m_currentFactor += (1 * m_currentDirection);
    if (m_currentFactor == 0 || m_currentFactor == 5)
        m_currentDirection = -m_currentDirection;

    if (m_currentFactor == 0) {
        setExplodeFactor(m_currentSlice, 0.0);
        m_currentSlice++;
        if (m_currentSlice >= m_model.columnCount())
            m_currentSlice = 0;
    }

    setExplodeFactor(
        m_currentSlice,
        static_cast<qreal>(m_currentFactor) / 10.0);
    m_chart->update();
}

void MainWindow::on_threeDGB_toggled(bool toggle)
{
    // note: We use the global getter method here, it will fall back
    //       automatically to return the default settings.
    ThreeDPieAttributes attrs(m_pie->threeDPieAttributes());
    attrs.setEnabled(toggle);
    attrs.setDepth(threeDFactorSB->value());
    m_pie->setThreeDPieAttributes(attrs);
    m_chart->update();
}

void MainWindow::on_threeDFactorSB_valueChanged(int factor)
{
    // note: We use the global getter method here, it will fall back
    //       automatically to return the default settings.
    ThreeDPieAttributes attrs(m_pie->threeDPieAttributes());
    attrs.setEnabled(threeDGB->isChecked());
    attrs.setDepth(factor);
    m_pie->setThreeDPieAttributes(attrs);
    m_chart->update();
}
