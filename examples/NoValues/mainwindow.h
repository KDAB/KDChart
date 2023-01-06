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

#include <QWidget>
#include <TableModel.h>

namespace KDChart {
class Chart;
class BarDiagram;
class LineDiagram;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override
    {
    }

private:
    KDChart::Chart *m_chart;
    KDChart::LineDiagram *m_bars;
    QAbstractItemModel *m_model;
};

#endif /* MAINWINDOW_H */
