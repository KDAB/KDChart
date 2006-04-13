#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartPieDiagram>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
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
    PolarCoordinatePlane* polarPlane = new PolarCoordinatePlane( m_chart );
    m_chart->replaceCoordinatePlane( polarPlane );
    m_pie = new PieDiagram();
    m_pie->setModel( &m_model );
    m_chart->coordinatePlane()->replaceDiagram( m_pie );
}

void MainWindow::on_startPositionSB_valueChanged( int pos )
{
    qDebug( "Sorry, not implemented: MainWindow::on_startPositionSB_valueChanged()" );
}

