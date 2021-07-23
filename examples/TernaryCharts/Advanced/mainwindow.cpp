/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#include <QLabel>
#include <QString>
#include <QTextStream>

#include <KDChartChart>
#include <KDChartTernaryAxis>
#include <KDChartTernaryLineDiagram>
#include <KDChartTernaryPointDiagram>
#include <KDChartTernaryCoordinatePlane>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
    , m_chart(nullptr)
    , m_diagram(nullptr)
    , m_ternaryPlane(nullptr)
{
    setupUi(this);
    // make a chart
    m_chart = new KDChart::Chart;
    // replace the default (cartesian) coordinate plane with a ternary one
    m_ternaryPlane = new KDChart::TernaryCoordinatePlane;
    m_chart->replaceCoordinatePlane(m_ternaryPlane);
    // make a ternary line diagram
    m_diagram = new KDChart::TernaryPointDiagram;
    // and replace the default diagram with it
    m_ternaryPlane->replaceDiagram(m_diagram);

    // add the three ternary axes, see the positions
    KDChart::TernaryAxis *axisA = new KDChart::TernaryAxis(m_diagram);
    axisA->setPosition(KDChartEnums::PositionSouth);
    KDChart::TernaryAxis *axisB = new KDChart::TernaryAxis(m_diagram);
    axisB->setPosition(KDChartEnums::PositionWest);
    KDChart::TernaryAxis *axisC = new KDChart::TernaryAxis(m_diagram);
    axisC->setPosition(KDChartEnums::PositionEast);

    QHBoxLayout *chartLayout = new QHBoxLayout(frame);
    chartLayout->addWidget(m_chart);

    setupModel();
    m_diagram->setModel(&m_model);
    connect(m_diagram, SIGNAL(clicked(QModelIndex)),
            SLOT(indexClicked(QModelIndex)));
}

void MainWindow::setupModel()
{
    m_model.insertRows(0, 5);
    m_model.insertColumns(0, 3);

    const float column0Share = 1.0f / 3.0f;
    const float column1Share = 1.0f / 3.0f;
    const float column2Share = 1.0f / 3.0f;

    for (int row = 0; row < m_model.rowCount(); ++row) {
        const qreal SkewX = column0Share * (1 - 1.0 / (5 * row * row * row + 1));
        const qreal SkewY = SkewX;
        QModelIndex index;

        index = m_model.index(row, 0);
        m_model.setData(index, QVariant(column0Share - SkewX));
        index = m_model.index(row, 1);
        m_model.setData(index, QVariant(column1Share + SkewX - SkewY));
        index = m_model.index(row, 2);
        m_model.setData(index, QVariant(column2Share + SkewY));
    }
}

void MainWindow::indexClicked(const QModelIndex &index)
{
    qDebug() << "MainWindow::indexClicked:" << index;
    if (index.isValid()) {
        QString text;
        QTextStream stream(&text);
        stream << "Row: " << index.row() << " - Column: " << index.column();
        label->setText(text);
    }
}
