/****************************************************************************
 ** Copyright (C) 2008 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **    information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "mainwindow.h"
#include <KDChartChart>
#include <KDChartCartesianAxis>

using namespace KDChart;

MainWindow::MainWindow( QWidget *parent )
    : QWidget( parent )
    , m_chart( new Chart() )
    , m_diagram( m_chart )
{
    setupUi( this );

    m_HLCModel.loadFromCSV( ":/HLC" );
    m_OHLCModel.loadFromCSV( ":/OHLC" );

    m_diagram.setType( StockDiagram::HighLowClose );
    m_diagram.setModel( &m_HLCModel );
    m_chart->coordinatePlane()->replaceDiagram( &m_diagram );

    chartFrame->addWidget( m_chart );

    // Abscissa
    CartesianAxis *leftAxis = new CartesianAxis( &m_diagram );
    // Ordinate
    CartesianAxis *bottomAxis = new CartesianAxis( &m_diagram );

    leftAxis->setPosition( CartesianAxis::Left );

    TextAttributes attributes = bottomAxis->textAttributes();
    attributes.setRotation( 90 );
    attributes.setFontSize( Measure( 7.0, KDChartEnums::MeasureCalculationModeAbsolute ) );
    bottomAxis->setTextAttributes( attributes );
    bottomAxis->setPosition( CartesianAxis::Bottom );
    m_diagram.addAxis( leftAxis );
    m_diagram.addAxis( bottomAxis );

    // Initialize all values for the stock chart to sane defaults
    initValues();
}

void MainWindow::initValues() {
    m_threeDBarAttributes = m_diagram.threeDBarAttributes();
    m_threeDBarAttributes.setDepth( 10.0 );
    threeDProperties->setChecked( m_threeDBarAttributes.isEnabled() );
    perspectiveAngle->setValue( m_threeDBarAttributes.angle() );
    perspectiveDepth->setValue( (int)m_threeDBarAttributes.depth() );
    useShadowColors->setChecked( m_threeDBarAttributes.useShadowColors() );
    m_diagram.setThreeDBarAttributes( m_threeDBarAttributes );
}

void MainWindow::on_threeDProperties_toggled( bool checked )
{
    m_threeDBarAttributes.setEnabled( checked );
    m_diagram.setThreeDBarAttributes( m_threeDBarAttributes );
    m_chart->update();
}

void MainWindow::on_perspectiveAngle_valueChanged( int value )
{
    m_threeDBarAttributes.setAngle( value );
    m_diagram.setThreeDBarAttributes( m_threeDBarAttributes );
    m_chart->update();
}

void MainWindow::on_perspectiveDepth_valueChanged( int value )
{
    m_threeDBarAttributes.setDepth( value );
    m_diagram.setThreeDBarAttributes( m_threeDBarAttributes );
    m_chart->update();
}

void MainWindow::on_useShadowColors_toggled( bool checked )
{
    m_threeDBarAttributes.setUseShadowColors( checked );
    m_diagram.setThreeDBarAttributes( m_threeDBarAttributes );
    m_chart->update();
}

void MainWindow::on_stockTypeCB_currentIndexChanged( const QString &text )
{
    // FIXME: Workaround for disappearing diagram when setting new model
    m_chart->coordinatePlane()->takeDiagram( &m_diagram );

    if ( text == "High-Low-Close" ) {
        m_diagram.setType( StockDiagram::HighLowClose );
        m_diagram.setModel( &m_HLCModel );
    } else if ( text == "Open-High-Low-Close" ) {
        m_diagram.setType( StockDiagram::OpenHighLowClose );
        m_diagram.setModel( &m_OHLCModel );
    } else if ( text == "Candlestick" ) {
        m_diagram.setType( StockDiagram::Candlestick );
        m_diagram.setModel( &m_OHLCModel );
    }

    m_chart->coordinatePlane()->replaceDiagram( &m_diagram );
}

