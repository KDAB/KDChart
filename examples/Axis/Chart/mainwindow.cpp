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
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>

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

    CartesianAxis *topAxis = new CartesianAxis( m_lines );
    CartesianAxis *leftAxis = new CartesianAxis ( m_lines );
    CartesianAxis *bottomAxis = new CartesianAxis ( m_lines );
    topAxis->setPosition ( CartesianAxis::Top );
    leftAxis->setPosition ( CartesianAxis::Left );
    bottomAxis->setPosition ( CartesianAxis::Bottom );

    topAxis->setTitleText ( "Abscissa color configured top position" );
    leftAxis->setTitleText ( "Ordinate font configured" );
    bottomAxis->setTitleText ( "Abscissa Bottom" );

    TextAttributes taTop ( topAxis->titleTextAttributes () );
    taTop.setPen( QPen( Qt::red ) );
    topAxis->setTitleTextAttributes ( taTop );

    TextAttributes taLeft ( leftAxis->titleTextAttributes () );
    Measure me( taLeft.fontSize() );
    me.setValue( me.value() * 1.5 );
    taLeft.setFontSize( me );
    leftAxis->setTitleTextAttributes ( taLeft );

    TextAttributes taBottom ( bottomAxis->titleTextAttributes () );
    taBottom.setPen(  QPen( Qt::blue ) );
    bottomAxis->setTitleTextAttributes ( taBottom );

// set the following to 0, to see the default Abscissa labels (== X headers, as read from the data file)
#if 1
    QStringList daysOfWeek;
    daysOfWeek << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" ;
    topAxis->setLabels( daysOfWeek );
    QStringList shortDays;
    shortDays << "Mon" << "Tue" << "Wed" << "Thu" << "Fri";
    topAxis->setShortLabels( shortDays );

    QStringList bottomLabels;
    bottomLabels << "Day1" << "Day2" << "Day 3" << "Day 4" << "Day 5";
    bottomAxis->setLabels( bottomLabels );
#endif

    m_lines->addAxis( topAxis );
    m_lines->addAxis( leftAxis );
    m_lines->addAxis( bottomAxis );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );
}
