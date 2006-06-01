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
#include <KDChartBarDiagram>
#include <KDChartBarAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDBarAttributes>
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
    m_bars = new BarDiagram();
    m_bars->setModel( &m_model );
    //CartesianAxisList List = m_bars->axesList();

    CartesianAxis *xAxis = new CartesianAxis( m_bars );
    CartesianAxis *yAxis = new CartesianAxis ( m_bars );
    xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition ( KDChart::CartesianAxis::Left );
/*
    QStringList daysOfWeek;
    daysOfWeek << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";
    xAxis->setLabels( daysOfWeek );
    QStringList shortDays;
    shortDays << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    xAxis->setShortLabels( shortDays );
*/
    m_bars->addAxis( xAxis );
    m_bars->addAxis( yAxis );
    m_chart->coordinatePlane()->replaceDiagram( m_bars );

    // Set up the legend
    m_legend = new Legend( m_bars, m_chart );
    m_chart->addLegend( m_legend );
    m_legend->hide();
}
/*
void MainWindow::on_lineTypeCB_currentIndexChanged( const QString & text )
{
    if ( text == "Normal" )
        m_bars->setType( LineDiagram::Normal );
    else if ( text == "Stacked" )
        m_bars->setType( LineDiagram::Stacked );
    else if ( text == "Percent" )
        m_bars->setType( LineDiagram::Percent );
    else
        qWarning (" Does not match any type");

    m_chart->update();
}


void MainWindow::on_paintLegendCB_toggled( bool checked )
{
    m_legend->setVisible( checked );
    m_chart->update();
}

void MainWindow::on_paintValuesCB_toggled( bool checked )
{
    //testing
    const int colCount = m_bars->model()->columnCount();
    for ( int i = 0; i<colCount; ++i ) {
        QModelIndex index = m_bars->model()->index( 0, i, QModelIndex() );
        QBrush brush = qVariantValue<QBrush>( m_bars->model()->headerData( i, Qt::Vertical, DatasetBrushRole ) );
        DataValueAttributes a = m_bars->dataValueAttributes( index );
        if ( !paintMarkersCB->isChecked() ) {
            MarkerAttributes ma = a.markerAttributes();
            ma.setVisible( false );
            a.setMarkerAttributes( ma );
        }
        TextAttributes ta = a.textAttributes();
        ta.setRotation( 0 );
        ta.setFont( QFont( "Comic", 10 ) );
        ta .setColor( brush.color() );
        if ( checked )
            ta.setVisible( true );
        else
            ta.setVisible( false );

        a.setTextAttributes( ta );
        a.setVisible( true );
        m_bars->setDataValueAttributes( i, a);
    }
    m_chart->update();
}


void MainWindow::on_paintMarkersCB_toggled( bool checked )
{
   //testing
    DataValueAttributes a;
    // dont paint the values
    if ( !paintValuesCB->isChecked() ) {
        TextAttributes ta = a.textAttributes();
        ta.setVisible( false );
        a.setTextAttributes( ta );
    }
    MarkerAttributes ma = a.markerAttributes();
    MarkerAttributes::MarkerStylesMap map;
    map.insert( 0, MarkerAttributes::MarkerSquare );
    map.insert( 1, MarkerAttributes::MarkerCircle );
    map.insert( 2, MarkerAttributes::MarkerRing );
    map.insert( 3, MarkerAttributes::MarkerCross );
    map.insert( 4, MarkerAttributes::MarkerDiamond );
    ma.setMarkerStylesMap( map );

    switch ( markersStyleCB->currentIndex() ) {
        case 0:{
                   break; }
        case 1:{
                   ma.setMarkerStyle( MarkerAttributes::MarkerCircle );
                   break; }
        case 2:{
                   ma.setMarkerStyle( MarkerAttributes::MarkerSquare );
                   break; }
        case 3:{
                   ma.setMarkerStyle( MarkerAttributes::MarkerDiamond );
                   break; }
        case 4:{
                   ma.setMarkerStyle( MarkerAttributes::Marker1Pixel );
                   break; }
        case 5:{
                   ma.setMarkerStyle( MarkerAttributes::Marker4Pixels );
                   break; }
        case 6:{
                   ma.setMarkerStyle( MarkerAttributes::MarkerRing );
                   break; }
        case 7:{
                   ma.setMarkerStyle( MarkerAttributes::MarkerCross );
                   break; }
        case 8:{
                   ma.setMarkerStyle( MarkerAttributes::MarkerFastCross );
                   break; }
    }
    ma.setMarkerSize( QSize( markersWidthSB->value(), markersHeightSB->value() ) );

    if ( checked )
        ma.setVisible( true );
    else
        ma.setVisible( false );

    a.setMarkerAttributes( ma );
    a.setVisible( true );

    // make a special one for certain values
    DataValueAttributes yellowAttributes(a);
    MarkerAttributes yellowMarker = yellowAttributes.markerAttributes();
    yellowMarker.setMarkerColor( Qt::yellow );
    yellowAttributes.setMarkerAttributes( yellowMarker );

    const int rowCount = m_bars->model()->rowCount();
    const int colCount = m_bars->model()->columnCount();
    for ( int i = 0; i<colCount; ++i ) {
        DataValueAttributes colAttributes(a);
        if ( markersStyleCB->currentIndex() == 0 ) {
            MarkerAttributes ma = colAttributes.markerAttributes();
            ma.setMarkerStyle( ma.markerStylesMap().value(i) );
            colAttributes.setMarkerAttributes( ma );
        }
        for ( int j=0; j< rowCount; ++j ) {
            QModelIndex index = m_bars->model()->index( j, i, QModelIndex() );
            QBrush brush = qVariantValue<QBrush>( m_bars->model()->headerData( i, Qt::Vertical, DatasetBrushRole ) );
            double value = m_bars->model()->data( index ).toDouble();
            // Set a specific color - marker for a specific value
            if ( value == 8 ) {
                m_bars->setDataValueAttributes( index, yellowAttributes );
            }
        }
        m_bars->setDataValueAttributes( i, colAttributes );
    }
    m_chart->update();
}

void MainWindow::on_markersStyleCB_currentIndexChanged( const QString & text )
{
    Q_UNUSED(text);
    if ( paintMarkersCB->isChecked() )
        on_paintMarkersCB_toggled( true );
}


void MainWindow::on_markersWidthSB_valueChanged( int i )
{
    Q_UNUSED(i);
    markersHeightSB->setValue( markersWidthSB->value() );
    if ( paintMarkersCB->isChecked() )
        on_paintMarkersCB_toggled( true );
}

void MainWindow::on_markersHeightSB_valueChanged( int i )
{
    Q_UNUSED(text);
    markersWidthSB->setValue( markersHeightSB->value() );
    if ( paintMarkersCB->isChecked() )
        on_paintMarkersCB_toggled( true );
}


void MainWindow::on_displayAreasCB_toggled( bool checked )
{
    const int rowCount = m_bars->model()->rowCount();
    const int colCount = m_bars->model()->columnCount();
     for ( int i = 0; i<colCount; ++i ) {
         QModelIndex index = m_bars->model()->index( 0, i, QModelIndex() );
         for ( int j=0; j< rowCount; ++j ) {
             LineAttributes la = m_bars->lineAttributes( index );
             if ( checked  ) {
                 la.setDisplayArea( true );
                 la.setTransparency( transparencySB->value() );
             } else
                 la.setDisplayArea( false );
             m_bars->setLineAttributes( i,  la );
         }
     }
     m_chart->update();
}

void MainWindow::on_transparencySB_valueChanged( int alpha )
{
    const int rowCount = m_bars->model()->rowCount();
    const int colCount = m_bars->model()->columnCount();
    for ( int i = 0; i<colCount; ++i ) {
        QModelIndex index = m_bars->model()->index( 0, i, QModelIndex() );
        for ( int j=0; j< rowCount; ++j ) {
            LineAttributes la = m_bars->lineAttributes( index );
            la.setTransparency( alpha );
            m_bars->setLineAttributes( la );
        }
    }
    on_displayAreasCB_toggled( true );
}

void MainWindow::on_zoomFactorSB_valueChanged( double factor )
{
    if ( factor > 1 ) {
        hSBar->setVisible( true );
        vSBar->setVisible( true );
    } else {
        hSBar->setValue( 500 );
        vSBar->setValue( 500 );
        hSBar->setVisible( false );
        vSBar->setVisible( false );
    }
    m_chart->coordinatePlane()->setZoomFactorX( factor );
    m_chart->coordinatePlane()->setZoomFactorY( factor );
    m_chart->update();
}

void MainWindow::on_hSBar_valueChanged( int hPos )
{
    m_chart->coordinatePlane()->setZoomCenter( QPointF(hPos/1000.0, vSBar->value()/1000.0) );
    m_chart->update();
}

void MainWindow::on_vSBar_valueChanged( int vPos )
{
    m_chart->coordinatePlane()->setZoomCenter( QPointF( hSBar->value()/1000.0, vPos/1000.0) );
    m_chart->update();
}
*/

