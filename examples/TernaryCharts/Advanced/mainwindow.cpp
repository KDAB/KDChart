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

    setupModel();
    m_diagram->setModel( &m_model );
}

void MainWindow::setupModel()
{
    m_model.insertRows( 0, 5 );
    m_model.insertColumns(  0,  3 );

    const float column0Share = 1.0/3.0;
    const float column1Share = 1.0/3.0;
    const float column2Share = 1.0/3.0;

    for ( int row = 0; row < m_model.rowCount(); ++row ) {
        const double SkewX = column0Share * ( 1 - 1.0/( 5 * row*row*row + 1 ) );
        const double SkewY = SkewX;
        QModelIndex index;

        index = m_model.index( row, 0 );
        m_model.setData( index, QVariant( column0Share - SkewX ) );
        index = m_model.index( row, 1 );
        m_model.setData( index, QVariant( column1Share + SkewX - SkewY) );
        index = m_model.index( row, 2 );
        m_model.setData( index, QVariant( column2Share + SkewY ) );
    }
}