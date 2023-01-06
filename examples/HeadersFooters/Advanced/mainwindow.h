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

#include <QDialog>
#include <QMap>

#include "ui_addheaderdialog.h"
#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
class Chart;
class DatasetProxyModel;
class LineDiagram;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void on_addHeaderPB_clicked();
    void on_editHeaderPB_clicked();
    void on_removeHeaderPB_clicked();
    void on_headersTV_itemSelectionChanged();

private:
    void setupAddHeaderDialog(QDialog *dlg,
                              Ui::AddHeaderDialog &conf) const;

    KDChart::Chart *m_chart;
    TableModel m_model;
    KDChart::DatasetProxyModel *m_datasetProxy;
    KDChart::LineDiagram *m_lines;
};

#endif /* MAINWINDOW_H */
