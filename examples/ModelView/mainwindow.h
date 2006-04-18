/****************************************************************************
**
** Copyright (C) 2004-2006 Trolltech AS. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <TableModel.h>

namespace KDChart {
    class Chart;
}

class QAbstractItemView;
class QStandardItemModel;
class QTableView;
class QItemSelectionModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void initializeData();
    void openFile(const QString &path = QString());
    void saveFile();

private:
    void setupModel();
    void setupViews();

    KDChart::Chart* m_chart;

    QStandardItemModel  *m_model;
    QAbstractItemView   *m_diagramView;
    QTableView          *m_tableView;
    QItemSelectionModel *m_selectionModel;
};

#endif
