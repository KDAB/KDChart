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

#include <QItemSelection>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QStandardItem;
class QStandardItemModel;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace KDGantt {
class ConstraintModel;
class DateTimeGrid;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = {});

private slots:
    void addNewEntry();
    void removeEntry();
    void addDemoEntry();
    void printPreview();
    void showContextMenu(const QPoint &);
    void enableActions(const QItemSelection &selected);
    void zoomIn();
    void zoomOut();

    void slotClicked(const QModelIndex &);
    void slotDoubleClicked(const QModelIndex &);

private:
    void initModel();
    void initActions();
    void initItemDelegate();
    void initGrid();

    void setReadOnly(const QModelIndex &index, bool readOnly);
    void addConstraint(const QModelIndex &index1, const QModelIndex &index2);
    void addConstraint(const QStandardItem *item1, const QStandardItem *item2);

    QStandardItemModel *model;
    KDGantt::ConstraintModel *constraintModel;
    KDGantt::DateTimeGrid *grid;
    int dayWidth = 70;

    QAction *newEntryAction;
    QAction *removeEntryAction;
    QAction *demoAction;
    QAction *printAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;

    Ui::MainWindow *ui;
};

#endif /* MAINWINDOW_H */
