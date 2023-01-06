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
class LineDiagram;
class Legend;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void on_zoomFactorSB_valueChanged(double factor);
    void on_adjustGridCB_toggled(bool checked);
    void on_rubberBandZoomCB_toggled(bool checked);
    void on_hSBar_valueChanged(int value);
    void on_vSBar_valueChanged(int value);

    void applyNewZoomParameters();

private:
    KDChart::Chart *m_chart;
    TableModel m_model;
    KDChart::LineDiagram *m_lines;
    KDChart::Legend *m_legend;
};

#endif /* MAINWINDOW_H */
