/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#ifndef BENCHMARKWINDOW_H
#define BENCHMARKWINDOW_H

#include <KDChartCartesianAxis>
#include "ui_benchmarkwindow.h"
#include "testmainwindow.h"
#include "DynamicTableModel.h"

#define DEFAULT 100000

namespace KDChart {
    class Chart;
    class DatasetProxyModel;
    class LineDiagram;
    class LineAttributes;
    class CartesianAxis;
    class Legend;
}

class BenchmarkWindow : public QWidget, private Ui::BenchmarkWindow
{
    Q_OBJECT

public:
    BenchmarkWindow( QWidget* parent = 0 );

private slots:
		void runTest();
		void saveCount(int count){ m_count=count; }

private:
    KDChart::Chart* m_chart;
    DynamicTableModel m_model;
    KDChart::DatasetProxyModel* m_datasetProxy;
    KDChart::LineDiagram* m_lines;
    KDChart::Legend* m_legend;
		KDChart::CartesianAxis *m_xAxis;

		TestMainWindow m_mainWindow;
		QStringList m_xNotation;

		static const int m_stepWidth = 10000;
		int m_count;

};


#endif /* MAINWINDOW_H */

