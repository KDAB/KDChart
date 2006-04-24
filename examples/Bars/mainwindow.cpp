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
#include <KDChartDatasetProxyModel>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartBarDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartThreeDBarAttributes>


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

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_bars = new BarDiagram();
    m_bars->setModel( &m_model );
    QPen pen;
    pen.setColor( Qt::darkGray );
    pen.setWidth( 1 );
    m_bars->setPen( pen );
    m_chart->coordinatePlane()->replaceDiagram( m_bars );
}



void MainWindow::on_barTypeCB_currentIndexChanged( const QString & text )
{
    if ( text == "Normal" )
        m_bars->setType( BarDiagram::Normal );
    else if ( text == "Stacked" )
        m_bars->setType( BarDiagram::Stacked );
    else if ( text == "Percent" )
        m_bars->setType( BarDiagram::Percent );
    else
        qWarning (" Does not match any type");

    QPen pen;
    pen.setColor( Qt::black );
    m_bars->setPen(pen );
    m_chart->update();
}


void MainWindow::on_paintValuesCB_toggled( bool checked )
{
    //testing
        const int rowCount = m_bars->model()->rowCount();
        const int colCount = m_bars->model()->columnCount();
        for ( int i = 0; i<colCount; ++i ) {
            for ( int j=0; j< rowCount; ++j ) {
                QModelIndex index = m_bars->model()->index( j, i, QModelIndex() );
                //QBrush brush = qVariantValue<QBrush>( m_bars->model()->headerData( i, Qt::Vertical, DatasetBrushRole ) );
                //Make it nicer
                QBrush brush = m_bars->brush( index );
                DataValueAttributes a = m_bars->dataValueAttributes( index );
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
                m_bars->setDataValueAttributes( index, a);
            }
        }
        m_chart->update();
}

void MainWindow::on_paintThreeDBarsCB_toggled( bool checked )
{
    ThreeDBarAttributes td;
    int defaultDepth = td.depth();
    if ( checked ) {
        td.setEnabled( true );
        if ( threeDDepthCB->isChecked() )
            td.setDepth( depthSB->value() ); 
        else 
            td.setDepth( defaultDepth );
    } else {
        td.setEnabled( false );
    }
    m_bars->setThreeDBarAttributes( td );
    m_chart->update();
}

void MainWindow::on_markColumnCB_toggled( bool checked )
{
    QPen pen;
    if ( checked ) {
        pen.setColor( Qt::yellow );
        pen.setStyle( Qt::DashLine );
        pen.setWidth( 3 );
        m_bars->setPen( markColumnSB->value(), pen );
    }  else {
         pen.setColor( Qt::darkGray );
         pen.setWidth( 1 );
         m_bars->setPen( markColumnSB->value(), pen );
    }
  m_chart->update();
}

void MainWindow::on_depthSB_valueChanged( int /*i*/ )
{
    if ( threeDDepthCB->isChecked() && paintThreeDBarsCB->isChecked() )
         on_paintThreeDBarsCB_toggled( true );
}

void MainWindow::on_threeDDepthCB_toggled( bool /*checked*/ )
{
        if ( paintThreeDBarsCB->isChecked() )
            on_paintThreeDBarsCB_toggled( true );
}

void MainWindow::on_markColumnSB_valueChanged( int /*i*/ )
{
 if ( markColumnCB->isChecked() )
        on_markColumnCB_toggled( true );
    else
        on_markColumnCB_toggled( false);
}
