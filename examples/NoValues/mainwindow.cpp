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
#include <KDChartCartesianAxis>
#include <KDChartChart>
#include <KDChartLegend>
#include <KDChartLineDiagram>

#include <QDebug>
#include <QHBoxLayout>
#include <QPen>
#include <QStandardItemModel>

using namespace KDChart;

class EmptyModel : public QAbstractItemModel
{
public:
    EmptyModel(QObject *parent = nullptr)
        : QAbstractItemModel(parent)
    {
        // qDebug() << "EmptyModel::EmptyModel()";
    }

    ~EmptyModel() override
    {
        // qDebug() << "EmptyModel::~EmptyModel()";
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::columnCount(...)";
        return 0;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::rowCount(...)";
        return 0;
    }

    // NOTE: The following method will not be called by KD Chart,
    //       because the model is returning 0 for columnCount() / rowCount().
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        Q_UNUSED(role);
        qDebug() << "EmptyModel::data(" << index.row() << index.column() << ")";
        Q_ASSERT_X(false, "EmptyModel::data", "We should not end here...");
        return QVariant();
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(row);
        Q_UNUSED(column);
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::index(" << row << column << ")";
        return QModelIndex();
    }

    QModelIndex parent(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent);
        // qDebug() << "EmptyModel::parent(...)";
        return QModelIndex();
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    auto *chartLayout = new QHBoxLayout(this);
    m_chart = new Chart();
    m_chart->setGlobalLeading(5, 5, 5, 5);
    chartLayout->addWidget(m_chart);

    m_model = new EmptyModel(this); // model contains no data at all

    // Set up the diagram
    m_bars = new LineDiagram();
    m_bars->setModel(m_model);
    auto *xAxis = new CartesianAxis(m_bars);
    auto *yAxis = new CartesianAxis(m_bars);
    xAxis->setPosition(KDChart::CartesianAxis::Bottom);
    yAxis->setPosition(KDChart::CartesianAxis::Left);
    xAxis->setTitleText("Abscissa axis at the bottom");
    yAxis->setTitleText("Ordinate axis at the left side");
    m_bars->addAxis(xAxis);
    m_bars->addAxis(yAxis);

    m_chart->coordinatePlane()->replaceDiagram(m_bars);

    auto *legend = new Legend(m_bars, m_chart);
    legend->setPosition(Position::South);
    legend->setAlignment(Qt::AlignCenter);
    legend->setShowLines(true);
    legend->setTitleText("This is the legend - showing no data either");
    legend->setOrientation(Qt::Horizontal);
    legend->addDiagram(m_bars);
    m_chart->addLegend(legend);
}
