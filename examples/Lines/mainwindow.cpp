#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartDatasetProxyModel>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>


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
    m_lines = new LineDiagram();
    m_lines->setModel( &m_model );
    m_chart->coordinatePlane()->replaceDiagram( m_lines );
}



void MainWindow::on_lineTypeCB_currentIndexChanged( const QString & text )
{
    LineAttributes la = m_lines->lineAttributes( m_lines->model()->index( 0, 0, QModelIndex() ) );
    if ( text == "Normal" )
        la.setType( LineAttributes::Normal );
    else if ( text == "Stacked" )
        la.setType( LineAttributes::Stacked );
    else if ( text == "Percent" )   
        la.setType( LineAttributes::Percent );
    else 
        qWarning (" Does not match any type");   
    m_lines->setLineAttributes( la );

    m_chart->update();
}


void MainWindow::on_paintValuesCB_toggled( bool checked )
{
    const int rowCount = m_lines->model()->rowCount();
    const int colCount = m_lines->model()->columnCount();
    for ( int i = 0; i<colCount; ++i ) {
        for ( int j=0; j< rowCount; ++j ) {
            QModelIndex index = m_lines->model()->index( j, i, QModelIndex() );
            QBrush brush = m_lines->model()->headerData( i, Qt::Vertical, DatasetBrushRole ).value<QBrush>();
            DataValueAttributes a = m_lines->dataValueAttributes( index );
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
            m_lines->setDataValueAttributes( index, a);
        }
    }
    m_chart->update();
}

void MainWindow::on_animateAreasCB_toggled( bool checked )
{
}

void MainWindow::on_highlightAreaCB_toggled( bool checked )
{
    const int column = highlightAreaSB->value();
    LineAttributes la = m_lines->lineAttributes( m_lines->model()->index( 0, column, QModelIndex() ) );
    if ( checked ) {
        la.setDisplayArea( true );
    }  else {
        la.setDisplayArea( false );
    }
    m_lines->setLineAttributes( column, la );
    m_chart->update();
}

void MainWindow::on_highlightAreaSB_valueChanged( int /*i*/ )
{
 if ( highlightAreaCB->isChecked() )
        on_highlightAreaCB_toggled( true );
    else
        on_highlightAreaCB_toggled( false);
}
