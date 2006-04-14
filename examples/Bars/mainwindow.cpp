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

    // now configure the dataset:
    m_datasetProxy = new DatasetProxyModel ( this );
    m_datasetProxy->setSourceModel ( &m_model );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    KDChart::CartesianCoordinatePlane* plane =
        dynamic_cast<CartesianCoordinatePlane* >( m_chart->coordinatePlane() );
    Q_ASSERT ( plane );
    if ( !plane ) return;
    m_bars = new BarDiagram( plane );
    m_bars->setModel( m_datasetProxy );
    QPen pen;
    pen.setColor( Qt::darkGray );
    pen.setWidth( 1 );
    m_bars->setPen( pen );
    m_chart->coordinatePlane()->replaceDiagram( m_bars );
}



void MainWindow::on_barTypeCB_currentIndexChanged( const QString & text )
{
    BarAttributes ba = m_bars->barAttributes( m_bars->model()->index( 0, 0, QModelIndex() ) );
    if ( text == "Normal" )
        ba.setType( BarAttributes::Normal );
    else if ( text == "Stacked" )
        ba.setType( BarAttributes::Stacked );
    else if ( text == "Percent" )   
        ba.setType( BarAttributes::Percent );
    else 
        qWarning (" Does not match any type");   
    m_bars->setBarAttributes( ba );
    QPen pen;
    pen.setColor( Qt::black );
     m_bars->setPen(pen );
     m_chart->coordinatePlane()->replaceDiagram( m_bars );
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
                QBrush brush = m_bars->model()->headerData( i, Qt::Vertical, DatasetBrushRole ).value<QBrush>();
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
    const int rowCount = m_bars->model()->rowCount();
    const int colCount = m_bars->model()->columnCount();
    for ( int i = 0; i<colCount; ++i ) {
        for ( int j=0; j< rowCount; ++j ) {
            QModelIndex index = m_bars->model()->index( j, i, QModelIndex() );
            QBrush brush = m_bars->model()->headerData( i, Qt::Vertical, DatasetBrushRole ).value<QBrush>();
            QPen pen( Qt::black );
            ThreeDBarAttributes td = m_bars->threeDBarAttributes( index );
            if ( checked ) {
                td.setEnabled( true );
                m_bars->setPen( index, pen );
            } else {
                pen.setColor( brush.color() ); 
                td.setEnabled( false );
                m_bars->setPen( index, pen );
            }
            m_bars->setThreeDBarAttributes( index, td); 
        } 
    }
    m_chart->coordinatePlane()->replaceDiagram( m_bars );
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

void MainWindow::on_markColumnSB_valueChanged( int /*i*/ )
{
 if ( markColumnCB->isChecked() )
        on_markColumnCB_toggled( true );
    else
        on_markColumnCB_toggled( false);
}
