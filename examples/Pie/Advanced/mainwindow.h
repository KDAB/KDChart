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

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE
namespace KDChart {
class Chart;
class PieDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    // start position
    void on_startPositionSB_valueChanged(double pos);
    void on_startPositionSL_valueChanged(int pos);

    // explode
    void on_explodeSubmitPB_clicked();
    void on_animateExplosionCB_toggled(bool toggle);
    void setExplodeFactor(int column, qreal value);

    // animation
    void slotNextFrame();

    // 3D
    void on_threeDGB_toggled(bool toggle);
    void on_threeDFactorSB_valueChanged(int factor);

private:
    KDChart::Chart *m_chart;
    TableModel m_model;
    KDChart::PieDiagram *m_pie;
    QTimer *m_timer;

    int m_currentFactor = 0;
    int m_currentDirection = 1;
    int m_currentSlice = 0;
};

#endif /* MAINWINDOW_H */
