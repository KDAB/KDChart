/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
class CartesianAxis;
class Legend;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void annotationsToggled(bool);
    void gridLinesOnAnnotationsToggled(bool);

private:
    KDChart::Chart *m_chart;
    KDChart::CartesianAxis *m_xAxis;
    TableModel m_model;
    KDChart::DatasetProxyModel *m_datasetProxy;
    KDChart::LineDiagram *m_lines;
    KDChart::Legend *m_legend;
};

#endif /* MAINWINDOW_H */
