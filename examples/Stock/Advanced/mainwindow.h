/****************************************************************************
 ** Copyright (C) 2008 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **    information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <TableModel.h>
#include <KDChartChart>
#include <KDChartStockDiagram>
#include <KDChartThreeDBarAttributes>

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = 0 );

private slots:
    void on_threeDProperties_toggled( bool checked );
    void on_perspectiveDepth_valueChanged( int value );
    void on_perspectiveAngle_valueChanged( int value );
    void on_useShadowColors_toggled( bool checked );
    void on_stockTypeCB_currentIndexChanged( const QString &text );

private:
    void initValues();

    KDChart::Chart *m_chart;
    TableModel m_HLCModel;
    TableModel m_OHLCModel;

    KDChart::StockDiagram m_diagram;
    KDChart::ThreeDBarAttributes m_threeDBarAttributes;
};

#endif // MAINWINDOW_H