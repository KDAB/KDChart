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

#include "mainwindow.h"
#include "AdjustedCartesianAxis.h"

#include <KDChartAbstractCoordinatePlane>
#include <KDChartBackgroundAttributes>
#include <KDChartChart>
#include <KDChartLegend>
#include <KDChartLineDiagram>
#include <KDChartLineAttributes>
#include <KDChartTextAttributes>
#include <KDChartThreeDLineAttributes>
#include <KDChartMarkerAttributes>

#include <QDebug>
#include <QPainter>
#include <QLinearGradient>
#include <QTextDocument>

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new KDChart::Chart;
    chartLayout->addWidget( m_chart );
    hSBar->setVisible( false );
    vSBar->setVisible( false );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_lines = new KDChart::LineDiagram();
    m_lines->setModel( &m_model );

    KDChart::CartesianAxis *xAxis = new KDChart::CartesianAxis( m_lines );
    KDChart::TextAttributes ta( xAxis->textAttributes() );

    AdjustedCartesianAxis *yAxis = new AdjustedCartesianAxis( m_lines );
    yAxis->setBounds( 3, 6 );
    xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition ( KDChart::CartesianAxis::Left );

/*
// set the following to 0 to have only one of the axes with background
#if 1
    // colourize the axes' backgrounds
    KDChart::BackgroundAttributes ba = yAxis->backgroundAttributes();
    ba.setVisible( true );
    ba.setBrush( QBrush( QColor( 0xff, 0xff, 0x40 ) ) );
    yAxis->setBackgroundAttributes( ba );
    xAxis->setBackgroundAttributes( ba );
#else
    // colourize the Ordinate axis' background
    QLinearGradient linearGrad( QPointF( 0, 100 ), QPointF( 0, 400 ) );
    linearGrad.setColorAt( 0.0, QColor( 0xff, 0xff, 0xc0 ) );
    linearGrad.setColorAt( 1.0, QColor( 0xa0, 0xc0, 0xff ) );
    ba = yAxis->backgroundAttributes();
    ba.setVisible( true );
    ba.setBrush( linearGrad );
    yAxis->setBackgroundAttributes( ba );
#endif
*/
    // add 1 pixel space at the left and at the top edge, because the
    // axis area would otherwise overwrite the left/top borders
    m_chart->setGlobalLeading( 1, 1, 0, 0 );

// set the following to 0, to see the default Abscissa labels (== X headers, as read from the data file)
#if 1
    QStringList daysOfWeek;
    daysOfWeek << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";
    xAxis->setLabels( daysOfWeek );

    //QStringList shortDays;
    //shortDays << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    //xAxis->setShortLabels( shortDays );
#endif

// Use HTML for drawing the text in the axis labels.
#if 0
    QStringList htmlStyles;
    htmlStyles << "<b>Bold</b>" << "<i>Italic</i>" << "<u>Underline</u>" << "<font color='red'>Red</font>";
    xAxis->setLabels( htmlStyles );
    ta.setTextDocument(new QTextDocument);
#endif


// set custom axis labels at custom positions
#if 0
    QMap< qreal, QString > annotations;
    annotations[ 0.5 ] = "Left";
    annotations[ 3.5 ] = "Center";
    annotations[ 4.2 ] = "After Center";
    annotations[ 6.5 ] = "Right";
    xAxis->setAnnotations(annotations);
#endif

    // Illustration of custom ticks
    QList<qreal> ticks;
    ticks.append( 0.5 );
    ticks.append( 3.5 );
    ticks.append( 4.2 );
    ticks.append( 6.5 );
    xAxis->setCustomTicks(ticks);
    yAxis->setCustomTicks(ticks);

    // rotate abscissa labels by -60 degrees:
    ta.setRotation( -60 );

    xAxis->setTextAttributes( ta );
    m_lines->addAxis( xAxis );
    m_lines->addAxis( yAxis );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );
    // Set up the legend
    xAxis->setCustomTickLength( 11 );
    yAxis->setCustomTickLength( 11 );
    m_legend = new KDChart::Legend( m_lines, m_chart );
    m_legend->setPosition( KDChart::Position::East );
    m_legend->setAlignment( Qt::AlignTop );
    m_chart->addLegend( m_legend );
}
