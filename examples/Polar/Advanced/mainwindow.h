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
#include <QStandardItemModel>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class PolarDiagram;
class PolarCoordinatePlane;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // start position of circular grid
    void on_startPositionSB_valueChanged(double pos);
    void on_startPositionSL_valueChanged(int pos);

    // show grid
    void on_circularGridCB_toggled(bool toggle);
    void on_sagittalGridCB_toggled(bool toggle);

private:
    void initKDChartClasses();
    void wireUpKDChartClasses();
    void setItemModelData();

    // For a change we do not use our special TableModel here,
    // but we just use the standard model that comes with Qt.
    QStandardItemModel m_model;

    KDChart::Chart *m_chart;
    KDChart::PolarDiagram *m_diagram;
    KDChart::PolarCoordinatePlane *m_polarPlane;

    int m_currentFactor = 0;
    int m_currentDirection = 1;
    int m_currentSlice = 0;
};

#endif /* MAINWINDOW_H */
