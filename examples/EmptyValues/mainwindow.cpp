#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartCartesianAxis>
#include <KDChartBarDiagram>


#include <QDebug>
#include <QPen>
#include <QHBoxLayout>

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    QHBoxLayout* chartLayout = new QHBoxLayout( this );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagram
    m_bars = new BarDiagram();
    m_bars->setModel( &m_model );
    m_bars->addAxis( new CartesianAxis( m_bars ) );

    m_chart->coordinatePlane()->replaceDiagram( m_bars );

    CartesianCoordinatePlane *plane 
            = dynamic_cast<CartesianCoordinatePlane*>( m_chart->coordinatePlane() );
    Q_ASSERT( plane );
    // The values in the model are all zero, so set the size of the plane 
    // to something that is non-zero manually
    plane->setVerticalRange( QPair<qreal, qreal>( -2.0, 2.0 ) );
}

