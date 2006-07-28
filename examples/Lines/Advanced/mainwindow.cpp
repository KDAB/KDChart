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
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>


#include <QDebug>
#include <QPainter>
#include <QTimer>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    m_curColumn = -1;
    m_curOpacity = 0;

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel( &m_model );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );

    // Instantiate the timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_timerFired()));
    timer->start(40);
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
    qDebug() << "MainWindow::on_paintValuesCB_toggled("<<checked<<")";
    const int colCount = m_lines->model()->columnCount(m_lines->rootIndex());
    for ( int iColumn = 0; iColumn<colCount; ++iColumn ) {
        DataValueAttributes a( m_lines->dataValueAttributes( iColumn ) );
        QBrush brush( m_lines->brush( iColumn ) );
        TextAttributes ta( a.textAttributes() );
        ta.setRotation( 0 );
        ta.setFont( QFont( "Comic", 10 ) );
        ta.setPen( QPen( brush.color() ) );

        if ( checked )
            ta.setVisible( true );
        else
            ta.setVisible( false );
        a.setVisible( true );
        a.setTextAttributes( ta );
        m_lines->setDataValueAttributes( iColumn, a );
    }
    m_chart->update();
}

void MainWindow::on_animateAreasCB_toggled( bool checked )
{
    if( checked ){
        highlightAreaCB->setCheckState( Qt::Unchecked );
        m_curColumn = 0;
    }else{
        m_curColumn = -1;
    }
    highlightAreaCB->setEnabled( ! checked );
    highlightAreaSB->setEnabled( ! checked );
    // un-highlight all previously highlighted columns
    const int colCount = m_lines->model()->columnCount();
    for ( int iColumn = 0; iColumn<colCount; ++iColumn )
        setHighlightArea( iColumn, 127, false, false );
    m_chart->update();
    m_curOpacity = 0;
}

void MainWindow::slot_timerFired()
{
    if( m_curColumn < 0 ) return;
    m_curOpacity += 5;
    if( m_curOpacity > 255 ){
        setHighlightArea( m_curColumn, 127, false, false );
        m_curOpacity = 0;
        ++m_curColumn;
        if( m_curColumn >= m_lines->model()->columnCount(m_lines->rootIndex()) )
            m_curColumn = 0;
    }
    setHighlightArea( m_curColumn, m_curOpacity, true, true );
}

void MainWindow::setHighlightArea( int column, int opacity, bool checked, bool doUpdate )
{
    LineAttributes la = m_lines->lineAttributes( m_lines->model()->index( 0, column, m_lines->rootIndex() ) );
    if ( checked ) {
        la.setDisplayArea( true );
        la.setTransparency( opacity );
    }  else {
        la.setDisplayArea( false );
    }
    m_lines->setLineAttributes( column, la );
    if( doUpdate )
        m_chart->update();
}

void MainWindow::on_highlightAreaCB_toggled( bool checked )
{
    setHighlightArea( highlightAreaSB->value(), 127, checked, true );
}

void MainWindow::on_highlightAreaSB_valueChanged( int i )
{
    Q_UNUSED( i );
    if ( highlightAreaCB->isChecked() )
        on_highlightAreaCB_toggled( true );
    else
        on_highlightAreaCB_toggled( false);
}
