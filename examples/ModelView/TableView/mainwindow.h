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

#include <QMainWindow>
#include <QItemSelection>
#include <TableModel.h>

namespace KDChart {
class Chart;
class BarDiagram;
class DatasetProxyModel;
}

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QStandardItemModel;
class QTableView;
class QItemSelectionModel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private slots:
    void initializeData();
    void openFile(const QString &path = QString());
    void saveFile();

    void itemClicked(const QModelIndex &index);

private:
    void setupModel();
    void setupViews();

    KDChart::Chart *m_chart;
    KDChart::DatasetProxyModel *m_datasetProxy;

    QStandardItemModel *m_model;
    KDChart::BarDiagram *m_diagramView;
    QTableView *m_tableView;
    QItemSelectionModel *m_selectionModel;
};

#endif
