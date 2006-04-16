#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLineDiagram>

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
    m_model2.loadFromCSV( ":/data" );

    // Set up the diagram
    m_lines = new LineDiagram();
    m_lines->setModel( &m_model );

    m_lines2 = new LineDiagram();
    m_lines2->setModel( &m_model2 );

    plane = new CartesianCoordinatePlane( m_chart );

    CartesianAxis *xAxis = new CartesianAxis( m_lines );
    CartesianAxis *yAxis = new CartesianAxis ( m_lines );
    CartesianAxis *yAxis2 = new CartesianAxis ( m_lines2 );
    
    xAxis->setPosition ( KDChart::CartesianAxis::Top );
    yAxis->setPosition ( KDChart::CartesianAxis::Left );
    yAxis2->setPosition ( KDChart::CartesianAxis::Right );

    m_lines2->addAxes( xAxis );

    m_chart->coordinatePlane()->replaceDiagram( m_lines );
    plane->replaceDiagram( m_lines2 );
    m_chart->addCoordinatePlane( plane/*, 1*/);
}
