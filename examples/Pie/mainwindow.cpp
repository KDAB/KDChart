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

void MainWindow::on_startPositionSB_valueChanged( double pos )
{
    int intValue = static_cast<int>( pos );
    startPositionSL->blockSignals( true );
    startPositionSL->setValue( intValue );
    startPositionSL->blockSignals( false );
    m_pie->setStartPosition( pos );
    update();
}

void MainWindow::on_startPositionSL_valueChanged( int pos )
{
    double doubleValue = static_cast<double>( pos );
    startPositionSB->blockSignals( true );
    startPositionSB->setValue( doubleValue  );
    startPositionSB->blockSignals( false );
    m_pie->setStartPosition( doubleValue );
    update();
}

