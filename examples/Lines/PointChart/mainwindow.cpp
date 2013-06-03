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

#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartMarkerAttributes>

#include <QtGui/QPainterPath>


using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );
    path = new QPainterPath();
    path->addEllipse( -2.0, -1.0, 4.0, 2.0 );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel( &m_model );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );
    m_chart->setGlobalLeading( 20, 20, 20, 20 );

    on_paintLinesCB_toggled( false );
    on_paintMarkersCB_toggled( true );

}

MainWindow::~MainWindow()
{
    delete path;
}

void MainWindow::on_lineTypeCB_currentIndexChanged( const QString & text )
{
    if ( text == "Normal" )
        m_lines->setType( LineDiagram::Normal );
    else if ( text == "Stacked" )
        m_lines->setType( LineDiagram::Stacked );
    else if ( text == "Percent" )
        m_lines->setType( LineDiagram::Percent );
    else
        qWarning (" Does not match any type");

    m_chart->update();
}

void MainWindow::on_paintValuesCB_toggled( bool checked )
{
    const int colCount = m_lines->model()->columnCount(m_lines->rootIndex());
    for ( int iColumn = 0; iColumn < colCount; ++iColumn ) {
        DataValueAttributes a( m_lines->dataValueAttributes( iColumn ) );
        QBrush brush( m_lines->brush( iColumn ) );
        TextAttributes ta( a.textAttributes() );
        ta.setRotation( 0 );
        ta.setFont( QFont( "Comic" ) );
        ta.setPen( QPen( brush.color() ) );
        ta.setVisible( checked );

        a.setVisible( true );
        a.setTextAttributes( ta );
        m_lines->setDataValueAttributes( iColumn, a );
    }
    m_chart->update();
}


void MainWindow::on_paintLinesCB_toggled(  bool checked )
{
    const int colCount = m_lines->model()->columnCount(m_lines->rootIndex());
    for ( int iColumn = 0; iColumn<colCount; ++iColumn ) {
        DataValueAttributes a( m_lines->dataValueAttributes( iColumn ) );
        QBrush lineBrush( m_lines->brush( iColumn ) );
        if ( checked ) {
            QPen linePen( lineBrush.color() );
            m_lines->setPen(  iColumn,  linePen );
        }
        else
            m_lines->setPen( iColumn,  Qt::NoPen );
    }
    m_chart->update();
}


void MainWindow::on_paintMarkersCB_toggled( bool checked )
{
    const int colCount = m_lines->model()->columnCount(m_lines->rootIndex());
    for ( int iColumn = 0; iColumn<colCount; ++iColumn ) {
        DataValueAttributes dva( m_lines->dataValueAttributes( iColumn ) );
        QBrush lineBrush( m_lines->brush( iColumn ) );
        TextAttributes ta ( dva.textAttributes() );
        ta.setVisible( paintValuesCB->isChecked() );
        MarkerAttributes ma( dva.markerAttributes() );
        ma.setMarkerSize( QSize( markersWidthSB->value(),
                                 markersHeightSB->value() ) );

        switch ( markersStyleCB->currentIndex() ) {
        case 0:
            ma.setMarkerStyle( MarkerAttributes::MarkerCircle );
            break;
        case 1:
            ma.setMarkerStyle( MarkerAttributes::MarkerSquare );
            break;
        case 2:
            ma.setMarkerStyle( MarkerAttributes::MarkerDiamond );
            break;
        case 3:
            ma.setMarkerStyle( MarkerAttributes::Marker1Pixel );
            break;
        case 4:
            ma.setMarkerStyle( MarkerAttributes::Marker4Pixels );
            break;
        case 5:
            ma.setMarkerStyle( MarkerAttributes::MarkerRing );
            break;
        case 6:
            ma.setMarkerStyle( MarkerAttributes::MarkerCross );
            break;
        case 7:
            ma.setMarkerStyle( MarkerAttributes::MarkerFastCross );
            break;
        case 8:
            ma.setMarkerStyle( MarkerAttributes::PainterPathMarker );
        }

        QPen markerPen( lineBrush.color() );
        ma.setPen( markerPen );
        ma.setVisible( checked );
        ma.setCustomMarkerPath( *path );
        dva.setTextAttributes( ta );
        dva.setMarkerAttributes( ma );
        dva.setVisible( true );

        m_lines->setDataValueAttributes( iColumn, dva );
    }

    m_chart->update();
}


void MainWindow::on_markersStyleCB_currentIndexChanged( const QString & text )
{
    Q_UNUSED( text );
    on_paintMarkersCB_toggled( paintMarkersCB->isChecked() );
}


void MainWindow::on_markersWidthSB_valueChanged( int i )
{
    markersHeightSB->setValue( i );
    on_paintMarkersCB_toggled( paintMarkersCB->isChecked() );
}

void MainWindow::on_markersHeightSB_valueChanged( int i )
{
    markersWidthSB->setValue( i );
    on_paintMarkersCB_toggled( paintMarkersCB->isChecked() );
}
