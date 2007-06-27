/****************************************************************************
 ** Copyright (C) 2007 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTPRODUCT_H
#define KDCHARTPRODUCT_H

#include "kdabstractsourcepanel.h"

#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartBarDiagram>
#include <KDChartPolarDiagram>
#include <KDChartWidget>
#include <KDChartPieDiagram>
#include <KDChartPieAttributes>
#include <KDChartThreeDPieAttributes>
#include <KDChartThreeDLineAttributes>
#include <KDChartDataValueAttributes>

#include "ui_kdchartdemo.h"

class KDChartDemo : public QWidget, public Ui::KDChartDemo
{
    Q_OBJECT

public:
    KDChartDemo( QWidget* parent = 0);

private Q_SLOTS:
    void setClassicMode(bool toggled);
    void setFreshMode(bool toggled);
    void tabHook(int index);

private:
    // setup the data model
    void setModel( QAbstractTableModel *model );

    // setup different chart types
    void setupLineChart();
    void setupBarChart();
    void setupPolarDiagram();
    void setupPieChart();

    KDAbstractSourcePanel *m_sourcePanel;

    KDChart::Chart* m_lineChart;
    KDChart::LineDiagram* m_lines;

    KDChart::Chart* m_barChart;
    KDChart::BarDiagram* m_bar;
    QPalette m_showyWhitePalette;

    KDChart::Chart* m_polarChart;
    KDChart::PolarDiagram* m_polar;
    KDChart::PolarCoordinatePlane* m_polarPlane;

    KDChart::Chart* m_pieChart;
    KDChart::PieDiagram* m_pie;

    QTabBar* m_tabBar;

    //! color stuff:

    enum Style {
            DecentBlue,
            DecentRed,
            DecentBrown,
            Pastel,
            Rainbow,
            LightBlue
    };

   /*!
    * computes a color with the given parameter
    * which allowes one to get different colors for
    * every chart line or area
    */
    QColor getDataColor(int parameter, KDChartDemo::Style );

    void setupColors();
    QColor m_rainbowColors[10];
    QColor m_lightBlueColors[10];

};

#endif
