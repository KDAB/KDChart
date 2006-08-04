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

#include "mainwindow.h"

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

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );
    hSBar->setVisible( false );
    vSBar->setVisible( false );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel( &m_model );
    //CartesianAxisList List = m_lines->axesList();

    CartesianAxis *xAxis = new CartesianAxis( m_lines );
    CartesianAxis *yAxis = new CartesianAxis ( m_lines );
    CartesianAxis *bAxis = new CartesianAxis ( m_lines );
    xAxis->setPosition ( CartesianAxis::Top );
    yAxis->setPosition ( CartesianAxis::Left );
    bAxis->setPosition ( CartesianAxis::Bottom );

    xAxis->setTitleText ( "Abscissa color configured top position" );
    yAxis->setTitleText ( "Ordinate font configured" );
    bAxis->setTitleText ( "Abscissa Bottom" );

    TextAttributes tax ( xAxis->titleTextAttributes () );
    tax.setPen( QPen( Qt::red ) );
    xAxis->setTitleTextAttributes ( tax );

    TextAttributes tay ( yAxis->titleTextAttributes () );
    Measure me( tay.fontSize() );
    me.setValue( me.value() * 1.5 );
    tay.setFontSize( me );
    yAxis->setTitleTextAttributes ( tay );

    TextAttributes tab ( bAxis->titleTextAttributes () );
    tab.setPen(  QPen( Qt::blue ) );
    bAxis->setTitleTextAttributes ( tab );

// set the following to 0, to see the default Abscissa labels (== X headers, as read from the data file)
#if 1
    QStringList daysOfWeek;
    daysOfWeek << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" ;
    xAxis->setLabels( daysOfWeek );
    QStringList shortDays;
    shortDays << "Mon" << "Tue" << "Wed" << "Thu" << "Fri";
    xAxis->setShortLabels( shortDays );

    QStringList bottomLabels;
    bottomLabels << "Day1" << "Day2" << "Day 3" << "Day 4" << "Day 5";
    bAxis->setLabels( bottomLabels );
#endif

    m_lines->addAxis( xAxis );
    m_lines->addAxis( yAxis );
    m_lines->addAxis( bAxis );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );

    // Set up the legend
    m_legend = new Legend( m_lines, m_chart );
    m_chart->addLegend( m_legend );
    m_legend->hide();
}
