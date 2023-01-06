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
class QStandardItemModel;
class QCloseEvent;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace KDGantt {
class ConstraintModel;
class DateTimeGrid;
class Legend;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});
    ~MainWindow() override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void addNewEntry();
    void removeEntry();
    void showContextMenu(const QPoint &);
    void enableActions(const QItemSelection &selected);
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void scaleAuto();
    void scaleHour();
    void scaleDay();
    void scaleWeek();
    void scaleMonth();

private:
    void initModel();
    void initActions();
    void initItemDelegate();
    void initGrid();

    void setReadOnly(const QModelIndex &index, bool readOnly);
    void addConstraint(const QModelIndex &index1, const QModelIndex &index2);

    QStandardItemModel *model;
    KDGantt::ConstraintModel *constraintModel;
    KDGantt::DateTimeGrid *grid;
    KDGantt::Legend *smallLegend = nullptr;
    KDGantt::Legend *detailedLegend = nullptr;

    QAction *newEntryAction;
    QAction *removeEntryAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomFitAction;

    Ui::MainWindow *ui;
};

#endif /* MAINWINDOW_H */
