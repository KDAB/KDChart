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
#include <KDChartLegend>
#include <KDChartCartesianAxis>
#include <QColorDialog>

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
    KDChart::Legend* legend  = new KDChart::Legend( &m_diagram, m_chart );
    m_chart->addLegend( legend );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    chartLayout->addWidget( m_chart );

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
    m_diagram.addAxis( bottomAxis );
    applyColor( QColor( "chartreuse" ) );
    const bool connected = connect( colorChooser, SIGNAL( clicked() ), SLOT( chooseColor() ) );
    Q_ASSERT( connected );
    Q_UNUSED( connected );

    // Initialize all values for the stock chart to sane defaults
    initValues();
}

void MainWindow::chooseColor()
{
    applyColor( QColorDialog::getColor( m_diagram.brush().color(), this ) );
}

void MainWindow::applyColor(const QColor &color)
{
    if ( color.isValid() ) {
        m_diagram.setPen( 0, QPen( color.darker( 130 ) ) );
        m_diagram.setBrush( 0, QBrush( color ) );
        QColor inverse( 255 - color.red(), 255 - color.green(), 255 - color.blue() );
        m_diagram.setPen( 1, QPen( inverse.darker( 130 ) ) );
        m_diagram.setBrush( 1, QBrush( inverse ) );
        QPalette pal = colorChooser->palette();
        pal.setBrush( QPalette::Button, QBrush( color ) );
        colorChooser->setPalette( pal );
    }
}

void MainWindow::initValues()
{
    m_threeDBarAttributes = m_diagram.threeDBarAttributes();
    m_threeDBarAttributes.setDepth( 10.0 );
    m_threeDBarAttributes.setUseShadowColors( false );
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

