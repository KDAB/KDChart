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

    m_lines2->setReferenceDiagram( m_lines );

    plane = new CartesianCoordinatePlane( m_chart );

    CartesianAxis *xAxis = new CartesianAxis( m_lines );
    CartesianAxis *yAxis = new CartesianAxis ( m_lines );
    CartesianAxis *yAxis2 = new CartesianAxis ( m_lines2 );

    xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition ( KDChart::CartesianAxis::Right );
    yAxis2->setPosition ( KDChart::CartesianAxis::Left );

    m_lines->addAxes( xAxis );
    m_lines2->addAxes( xAxis );
    m_lines->addAxes( yAxis );
    m_lines2->addAxes( yAxis2 );

    m_chart->coordinatePlane()->replaceDiagram( m_lines );
    plane->replaceDiagram( m_lines2 );
    m_chart->addCoordinatePlane( plane );
}
