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

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLineDiagram>
#include <KDChartLineAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDLineAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartLegend>

#include <QDebug>
#include <QPainter>

#include "benchmarkwindow.h"

using namespace KDChart;

const int BenchmarkWindow::m_stepWidth = 10;

BenchmarkWindow::BenchmarkWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    QVBoxLayout* chartLayout = new QVBoxLayout( chartFrame );
    m_chart = new Chart();
    m_chart->setGlobalLeading( 20,  20,  20,  20 );
    chartLayout->addWidget( m_chart );
    hSBar->setVisible( false );
    vSBar->setVisible( false );

    // draw and set up the diagram
    m_lines = new LineDiagram();
		m_count = DEFAULT;
		m_xAxis = new CartesianAxis( m_lines );

		runTest();

    CartesianAxis *yAxis = new CartesianAxis ( m_lines );
    m_xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition ( KDChart::CartesianAxis::Left );

		QString xAxisTitle;
		xAxisTitle.setNum(TEST_VALUE);
		xAxisTitle.prepend("count of QVariant(");
		xAxisTitle += QString(")");
    m_xAxis->setTitleText ( xAxisTitle );
    yAxis->setTitleText ( "milliseconds" );

    m_lines->addAxis( m_xAxis );
    m_lines->addAxis( yAxis );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );
    m_chart->setGlobalLeadingRight( 20 );

    // Set up the legend
    m_legend = new Legend( m_lines, m_chart );
		m_legend->setText( 0,  "model::setData()" );
		m_legend->setText( 1,  "drawIntoPixmap()" );
		m_legend->setText( 2,  "setModel()" );
		m_legend->setText( 3,  "show()" );
		KDChart::RelativePosition relativePosition;
    relativePosition.setReferenceArea( m_chart->coordinatePlane() );
    relativePosition.setReferencePosition( Position::NorthEast );
    relativePosition.setAlignment( Qt::AlignTop | Qt::AlignRight );
    m_legend->setFloatingPosition( relativePosition );
		m_legend->setAutoFillBackground(true);
		m_legend->setBackgroundRole(QPalette::Base);
    m_chart->addLegend( m_legend );

		QHBoxLayout* updateLayout = new QHBoxLayout;

		QPushButton* updateButton = new QPushButton(tr("update"));

		QSpinBox* dataCount = new QSpinBox();
		dataCount->setRange(m_stepWidth, DEFAULT*100);
		dataCount->setSingleStep(m_stepWidth);
		dataCount->setValue(DEFAULT);

		updateLayout->addStretch(100);
		updateLayout->addWidget( dataCount );
		updateLayout->addWidget( updateButton );
		chartLayout->addLayout( updateLayout );

		connect(dataCount, SIGNAL(valueChanged(int)), this, SLOT(saveCount(int)));
		connect(updateButton, SIGNAL(clicked()), this, SLOT(runTest()));

}

void BenchmarkWindow::runTest()
{
		m_model.reset();
		m_xNotation.clear();

 		for(int i = 0 ; i < m_count; i += m_stepWidth)
		{
			m_mainWindow.testLab( i, i / m_stepWidth, &m_model );
			QString nr;
			nr.setNum(i, 10);
  		m_xNotation << nr;
		}

 		m_xAxis->setLabels( m_xNotation );
		m_lines->setModel( &m_model );
}

