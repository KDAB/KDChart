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

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

namespace KDChart {
class Chart;
class LineDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void on_showDataset1CB_toggled(bool checked);
    void on_showDataset2CB_toggled(bool checked);
    void on_showDataset3CB_toggled(bool checked);
    void setHidden(int dataset, bool hidden);
    void openFile(const QString &path);

private:
    KDChart::Chart *m_chart;
    KDChart::LineDiagram *m_lines;
    QAbstractItemModel *m_model;
};

#endif /* MAINWINDOW_H */
