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

#include "ui_mainwindow.h"
#include <TableModel.h>

namespace KDChart {
    class Chart;
    class DatasetProxyModel;
    class LineDiagram;
    class LineAttributes;
    class CartesianAxis;
    class Legend;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0 );



private slots:

    void on_lineTypeCB_currentIndexChanged( const QString & text );
    void on_paintLegendCB_toggled( bool checked );
    void on_paintValuesCB_toggled( bool checked );
    void on_paintMarkersCB_toggled( bool checked );
    void on_markersStyleCB_currentIndexChanged( const QString & text );
    void on_markersWidthSB_valueChanged( int i );
    void on_markersHeightSB_valueChanged( int i);
    void on_displayAreasCB_toggled( bool checked );
    void on_transparencySB_valueChanged( int i );
    void on_zoomFactorSB_valueChanged( double factor );
    void on_hSBar_valueChanged( int value );
    void on_vSBar_valueChanged( int value );


private:
    KDChart::Chart* m_chart;
    TableModel m_model;
    KDChart::DatasetProxyModel* m_datasetProxy;
    KDChart::LineDiagram* m_lines;
    KDChart::Legend* m_legend;
    // mutable KDChart::CartesianAxis xAxis;
    //mutable KDChart::CartesianAxis yAxis;

};


#endif /* MAINWINDOW_H */

