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
    m_chart->setGlobalLeading( 20, 20, 20, 20 );
    chartLayout->addWidget( m_chart );
    hSBar->setVisible( false );
    vSBar->setVisible( false );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel( &m_model );
    //CartesianAxisList List = m_lines->axesList();
    CartesianAxis *xAxis = new CartesianAxis( m_lines );
    CartesianAxis *yAxis = new CartesianAxis( m_lines );
    CartesianAxis *axisTop = new CartesianAxis( m_lines );
    CartesianAxis *axisRight = new CartesianAxis( m_lines );
    xAxis->setPosition( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition( KDChart::CartesianAxis::Left );
    axisTop->setPosition( KDChart::CartesianAxis::Top );
    axisRight->setPosition( KDChart::CartesianAxis::Right );

    xAxis->setTitleText( "Abscissa axis at the bottom" );
    yAxis->setTitleText( "Ordinate axis at the left side" );
    axisTop->setTitleText( "Abscissa axis at the top" );
    axisRight->setTitleText( "Ordinate axis at the right side" );
    TextAttributes taTop( xAxis->titleTextAttributes () );
    taTop.setPen( QPen( Qt::red ) );
    axisTop->setTitleTextAttributes( taTop );
    TextAttributes taRight( xAxis->titleTextAttributes () );
    Measure me( taRight.fontSize() );
    me.setValue( me.value() * 1.5 );
    taRight.setFontSize( me );
    axisRight->setTitleTextAttributes( taRight );

    m_lines->addAxis( xAxis );
    m_lines->addAxis( yAxis );
    m_lines->addAxis( axisTop );
    m_lines->addAxis( axisRight );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );

    m_lines->setUnitSuffix( QString::fromLatin1( "s" ), Qt::Horizontal );
    m_lines->setUnitSuffix( QString::fromLatin1( "m" ), Qt::Vertical );

    // Set up the legend
    m_legend = new Legend( m_lines, m_chart );
    m_chart->addLegend( m_legend );

    // initialize attributes; this is necessary because we need to enable data value attributes before
    // any of them (e.g. only markers) can be displayed. but if we enable data value attributs, a default
    // data value text is included, even if we only wanted to set markers. so we enable DVA and then
    // individually disable the parts we don't want.
    on_paintValuesCB_toggled( false );
    on_paintMarkersCB_toggled( false );
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


void MainWindow::on_paintLegendCB_toggled( bool checked )
{
    m_legend->setVisible( checked );
    m_chart->update();
}

void MainWindow::on_paintValuesCB_toggled( bool checked )
{
    // We set the DataValueAttributes on a per-column basis here,
    // because we want the texts to be printed in different
    // colours - according to their respective dataset's colour.
    const QFont font( "Comic", 10 );
    const int colCount = m_lines->model()->columnCount();
    for ( int iColumn = 0; iColumn < colCount; ++iColumn ) {
        DataValueAttributes dva = m_lines->dataValueAttributes( iColumn );
        dva.setVisible( true );

        TextAttributes ta( dva.textAttributes() );
        ta.setRotation( 0 );
        ta.setFont( font );
        ta.setPen( QPen( m_lines->brush( iColumn ).color() ) );
        ta.setVisible( checked );

        dva.setTextAttributes( ta );
        m_lines->setDataValueAttributes( iColumn, dva );
    }
    m_chart->update();
}


void MainWindow::on_paintMarkersCB_toggled( bool checked )
{
    // build a map with different marker types
    MarkerAttributes::MarkerStylesMap map;
    map.insert( 0, MarkerAttributes::MarkerSquare );
    map.insert( 1, MarkerAttributes::MarkerCircle );
    map.insert( 2, MarkerAttributes::MarkerRing );
    map.insert( 3, MarkerAttributes::MarkerCross );
    map.insert( 4, MarkerAttributes::MarkerDiamond );

    // next: Specify column- / cell-specific attributes!
    const int rowCount = m_lines->model()->rowCount();
    const int colCount = m_lines->model()->columnCount();
    for ( int iColumn = 0; iColumn < colCount; ++iColumn ) {

        DataValueAttributes dva = m_lines->dataValueAttributes( iColumn );
        dva.setVisible( true );
        MarkerAttributes ma( dva.markerAttributes() );

        switch ( markersStyleCB->currentIndex() ) {
        case 0:
            ma.setMarkerStyle( MarkerAttributes::MarkerSquare );
            break;
        case 1:
            // Column-specific attributes
            ma.setMarkerStyle( map.value( iColumn ) );
            break;
        case 2:
            ma.setMarkerStyle( MarkerAttributes::MarkerCircle );
            break;
        case 3:
            ma.setMarkerStyle( MarkerAttributes::MarkerDiamond );
            break;
        case 4:
            ma.setMarkerStyle( MarkerAttributes::Marker1Pixel );
            break;
        case 5:
            ma.setMarkerStyle( MarkerAttributes::Marker4Pixels );
            break;
        case 6:
            ma.setMarkerStyle( MarkerAttributes::MarkerRing );
            break;
        case 7:
            ma.setMarkerStyle( MarkerAttributes::MarkerCross );
            break;
        case 8:
            ma.setMarkerStyle( MarkerAttributes::MarkerFastCross );
            break;
        default:
            Q_ASSERT( false );
        }
        ma.setVisible( checked );

        ma.setMarkerSize( QSize( markersWidthSB->value(), markersHeightSB->value() ) );
        dva.setMarkerAttributes( ma );
        m_lines->setDataValueAttributes( iColumn, dva );

        // Specify cell-specific attributes for some values!
        for ( int j = 0; j < rowCount; ++j ) {
            const QModelIndex index( m_lines->model()->index( j, iColumn, QModelIndex() ) );
            const qreal value = m_lines->model()->data( index ).toReal();
            /* Set a specific color - marker for a specific value */
            if ( value == 8 ) {
                // retrieve cell specific attributes
                // or fall back to column settings
                // or fall back to global settings:
                DataValueAttributes yellowAttributes( m_lines->dataValueAttributes( index ) );
                MarkerAttributes yellowMarker( yellowAttributes.markerAttributes() );
                yellowMarker.setMarkerColor( Qt::yellow );
                yellowMarker.setMarkerSize( QSize( markersWidthSB->value(), markersHeightSB->value() ) );
                yellowMarker.setVisible( checked );
                yellowAttributes.setMarkerAttributes( yellowMarker );
                yellowAttributes.setVisible( checked );
                //cell specific attributes:
                m_lines->setDataValueAttributes( index, yellowAttributes );
            }
        }
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
    Q_UNUSED( i );
    markersHeightSB->setValue( markersWidthSB->value() );
    if ( paintMarkersCB->isChecked() )
        on_paintMarkersCB_toggled( true );
}

void MainWindow::on_markersHeightSB_valueChanged( int /*i*/ )
{
    markersWidthSB->setValue( markersHeightSB->value() );
    if ( paintMarkersCB->isChecked() )
        on_paintMarkersCB_toggled( true );
}


void MainWindow::on_displayAreasCB_toggled( bool checked )
{
    LineAttributes la( m_lines->lineAttributes() );
    la.setDisplayArea( checked );
    if ( checked  ) {
        la.setTransparency( transparencySB->value() );
    }
    m_lines->setLineAttributes( la );
    m_chart->update();
}

void MainWindow::on_transparencySB_valueChanged( int alpha )
{
    LineAttributes la( m_lines->lineAttributes() );
    la.setTransparency( alpha );
    m_lines->setLineAttributes( la );
    on_displayAreasCB_toggled( true );
}

void MainWindow::on_zoomFactorSB_valueChanged( double factor )
{
    const bool isZoomedIn = factor > 1;
    hSBar->setVisible( isZoomedIn );
    vSBar->setVisible( isZoomedIn );
    if ( !isZoomedIn) {
        hSBar->setValue( 500 );
        vSBar->setValue( 500 );
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
