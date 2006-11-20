#include <KDChartChart>
#include <KDChartTernaryAxis>
#include <KDChartTernaryDiagram>
#include <KDChartTernaryCoordinatePlane>

#include "mainwindow.h"

MainWindow::MainWindow( QWidget* parent )
    : QDialog( parent )
    , m_chart( 0 )
    , m_diagram( 0 )
    , m_ternaryPlane( 0 )
{
    setupUi( this );
    m_chart = new KDChart::Chart;
    m_ternaryPlane = new KDChart::TernaryCoordinatePlane;
    m_chart->replaceCoordinatePlane( m_ternaryPlane );
    m_diagram = new KDChart::TernaryDiagram;
    m_ternaryPlane->replaceDiagram( m_diagram );

    KDChart::TernaryAxis* axisA = new KDChart::TernaryAxis( m_diagram );
    axisA->setPosition( KDChartEnums::PositionSouth );
    KDChart::TernaryAxis* axisB = new KDChart::TernaryAxis( m_diagram );
    axisB->setPosition( KDChartEnums::PositionWest );
    KDChart::TernaryAxis* axisC = new KDChart::TernaryAxis( m_diagram );
    axisC->setPosition( KDChartEnums::PositionEast );

    QHBoxLayout* chartLayout = new QHBoxLayout( frame );
    chartLayout->addWidget( m_chart );
}
