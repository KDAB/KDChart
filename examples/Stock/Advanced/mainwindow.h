/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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
#include <KDChartChart>
#include <KDChartStockDiagram>
#include <KDChartThreeDBarAttributes>

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void on_threeDProperties_toggled(bool checked);
    void on_perspectiveDepth_valueChanged(int value);
    void on_perspectiveAngle_valueChanged(int value);
    void on_useShadowColors_toggled(bool checked);
    void on_stockTypeCB_currentIndexChanged(int index);
    void chooseColor();

private:
    void initValues();
    void applyColor(const QColor &color);

    KDChart::Chart *m_chart;
    TableModel m_HLCModel;
    TableModel m_OHLCModel;

    KDChart::StockDiagram m_diagram;
    KDChart::ThreeDBarAttributes m_threeDBarAttributes;
};

#endif // MAINWINDOW_H
