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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class BarDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:

    void on_barTypeCB_currentIndexChanged(const QString &text);
    void on_barOrientationCB_currentIndexChanged(const QString &text);
    void on_paintValuesCB_toggled(bool checked);
    void on_paintThreeDBarsCB_toggled(bool checked);
    void on_markColumnCB_toggled(bool checked);
    void on_markColumnSB_valueChanged(int i);
    void on_threeDDepthCB_toggled(bool checked);
    void on_depthSB_valueChanged(int i);
    void on_widthCB_toggled(bool checked);
    void on_widthSB_valueChanged(int i);
    void on_fixPlaneSizeCB_toggled(bool checked);

private:
    KDChart::Chart *m_chart;
    KDChart::BarDiagram *m_bars;
    TableModel m_model;
};

#endif /* MAINWINDOW_H */
