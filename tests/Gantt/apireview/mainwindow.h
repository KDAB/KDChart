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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );

private slots:
    void addNewEntry();
    void removeEntry();
    void addDemoEntry();
    void printPreview();
    void showContextMenu( const QPoint& );
    void enableActions( const QItemSelection& selected );
    void zoomIn();
    void zoomOut();

    void slotClicked( const QModelIndex& );
    void slotDoubleClicked( const QModelIndex& );

private:
    void initModel();
    void initActions();
    void initItemDelegate();
    void initGrid();

    void setReadOnly( const QModelIndex& index, bool readOnly );
    void addConstraint( const QModelIndex& index1, const QModelIndex& index2 );
    void addConstraint( const QStandardItem* item1, const QStandardItem* item2 );

    QStandardItemModel* model;
    KDGantt::ConstraintModel* constraintModel;
    KDGantt::DateTimeGrid* grid;
    int dayWidth;

    QAction* newEntryAction;
    QAction* removeEntryAction;
    QAction* demoAction;
    QAction* printAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;

    Ui::MainWindow* ui;
};

#endif /* MAINWINDOW_H */
