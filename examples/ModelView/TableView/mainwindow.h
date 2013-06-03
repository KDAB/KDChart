/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

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
    void selectionChanged( const QItemSelection & selected, const QItemSelection & deselected );

private slots:
    void initializeData();
    void openFile(const QString &path = QString());
    void saveFile();

    void itemClicked( const QModelIndex& index );

private:
    void setupModel();
    void setupViews();

    KDChart::Chart* m_chart;
    KDChart::DatasetProxyModel* m_datasetProxy;

    QStandardItemModel  *m_model;
    KDChart::BarDiagram *m_diagramView;
    QTableView          *m_tableView;
    QItemSelectionModel *m_selectionModel;
};

#endif
