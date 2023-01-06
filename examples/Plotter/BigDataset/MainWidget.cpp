/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "MainWidget.h"

#include "KDChartAbstractDiagram_p.h"
#include "KDChartChart.h"
#include "KDChartPlotter.h"

#include <QHBoxLayout>

#include <cmath>

MainWidget::MainWidget()
    : m_controlsContainer(new QWidget(this))
{
    // set up the chart

    auto *topLayout = new QHBoxLayout(this);
    m_controls.setupUi(m_controlsContainer);
    topLayout->addWidget(m_controlsContainer);

    auto *chart = new KDChart::Chart;
    topLayout->addWidget(chart);

    m_plotter = new KDChart::Plotter;
    m_plotter->setModel(&m_model);
    KDChart::AbstractDiagram::Private::get(m_plotter)->doDumpPaintTime = true;
    chart->coordinatePlane()->replaceDiagram(m_plotter);

    auto *cPlane = qobject_cast<KDChart::CartesianCoordinatePlane *>(chart->coordinatePlane());
    Q_ASSERT(cPlane);
    cPlane->setVerticalRange(QPair<qreal, qreal>(-2, 2));

    auto *xAxis = new KDChart::CartesianAxis(m_plotter);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    xAxis->setTitleText("X");
    m_plotter->addAxis(xAxis);

    auto *yAxis = new KDChart::CartesianAxis(m_plotter);
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    yAxis->setTitleText("Y");
    m_plotter->addAxis(yAxis);

    // wire up the UI

    // index of elements in vector must match corresponding Model::Function enum values
    m_functionSelector << m_controls.sineRadio << m_controls.triangleRadio << m_controls.squareRadio
                       << m_controls.noiseRadio << m_controls.oneDivSineRadio
                       << m_controls.sineOneDivRadio;
    foreach (QRadioButton *r, m_functionSelector) {
        connect(r, SIGNAL(toggled(bool)), SLOT(functionToggled(bool)));
    }

    connect(m_controls.runButton, SIGNAL(toggled(bool)),
            &m_model, SLOT(setRunning(bool)));

    // order matters again
    m_addPointsButtons << m_controls.add1kButton << m_controls.add10kButton << m_controls.add100kButton;
    foreach (QPushButton *b, m_addPointsButtons) {
        connect(b, SIGNAL(clicked(bool)), SLOT(addPointsButtonClicked()));
    }
}

// slot
void MainWidget::functionToggled(bool checked)
{
    if (!checked) {
        return;
    }
    int idx = m_functionSelector.indexOf(qobject_cast<QRadioButton *>(sender()));
    Q_ASSERT(idx >= 0);
    m_model.setFunction(static_cast<Model::Function>(idx));
}

// slot
void MainWidget::addPointsButtonClicked()
{
    int idx = m_addPointsButtons.indexOf(qobject_cast<QPushButton *>(sender()));
    Q_ASSERT(idx >= 0);
    m_model.appendPoints(pow(qreal(10), qreal(idx + 3)));
}
