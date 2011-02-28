/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "ui_mainwindow.h"


class QStandardItemModel;

namespace KDChart {
    class Chart;
    class LineDiagram;
}


class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );

private slots:

    void initializeDataModels();

    void on_pushButtonSave_clicked();
    void saveAs();
    void on_pushButtonClear_clicked();
    void removeTheChart();
    void on_pushButtonLoad_clicked();
    void load();

private:
    QHBoxLayout* m_chartLayout;

    KDChart::Chart* m_chart;

    QStandardItemModel* m_model1;
    QStandardItemModel* m_model2;
};


#endif /* MAINWINDOW_H */

