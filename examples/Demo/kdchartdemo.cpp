/****************************************************************************
 ** Copyright (C) 2007 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#include "kdchartdemo.h"

#include <QtCore/QDebug>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollBar>
#include <QStyle>

#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartTextAttributes>
#include <KDChartFrameAttributes>
#include <KDChartLegend>

#include "kdbasicsourcepanel.h"

using namespace KDChart;

KDChartDemo::KDChartDemo( QWidget* parent )
    : QWidget( parent )
{
    setupUi(this);

    m_sourcePanel = new KDBasicSourcePanel;

    // attach sourcePanel
    QHBoxLayout* sourcePanelLayout = new QHBoxLayout( sourcePanelFrame );
    sourcePanelLayout->addWidget((KDBasicSourcePanel*) m_sourcePanel);

    // set the color schemes for charts
    setupColors();

    // setup charts
    setupLineChart();
    setupBarChart();
    setupPolarDiagram();
    setupPieChart();

    setModel(m_sourcePanel->table());

    connect(classicStyleButton,SIGNAL(toggled(bool)),this,SLOT(setClassicMode(bool)));
    connect(freshStyleButton,SIGNAL(toggled(bool)),this,SLOT(setFreshMode(bool)));

    setClassicMode(true);
    classicStyleButton->setChecked(true);

    connect(tabWidget,SIGNAL(currentChanged(int)), this, SLOT(tabHook(int)));
}

void KDChartDemo::setModel( QAbstractTableModel *model )
{

    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(tableView->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), m_sourcePanel,
            SLOT(currentCellChanged(const QModelIndex &)));

    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_lines->setModel( model );
    m_bar->setModel( model );
    m_polar->setModel( model );
    m_pie->setModel( model );

    m_pieChart->coordinatePlane()->replaceDiagram( m_pie );
    m_polarChart->coordinatePlane()->replaceDiagram( m_polar );
    tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void KDChartDemo::setupLineChart()
{
    QHBoxLayout* lineChartLayout = new QHBoxLayout( LineDiagramTab );
    m_lineChart = new Chart();
    m_lineChart->setGlobalLeading( 10,  10,  10,  10 );
    lineChartLayout->addWidget( m_lineChart );

    // hide the frame:
    KDChart::FrameAttributes frameAttributes ( m_lineChart->frameAttributes() );
    frameAttributes.setVisible(false);
    m_lineChart->setFrameAttributes ( frameAttributes );

    // Set up the diagram
    m_lines = new LineDiagram();

    // create and position axis
    CartesianAxis *topAxis = new CartesianAxis( m_lines );
    CartesianAxis *rightAxis = new CartesianAxis ( m_lines );
    topAxis->setPosition ( CartesianAxis::Top );
    rightAxis->setPosition ( CartesianAxis::Right );

    // configure labels text attributes
    TextAttributes taLabels( topAxis->textAttributes() );
    taLabels.setPen(  QPen( Qt::darkBlue ) );
    taLabels.setRotation( 45 );
    taLabels.setFontSize( 20 );
    topAxis->setTextAttributes( taLabels );
    rightAxis->setTextAttributes( taLabels );

    // add axis
    m_lines->addAxis( topAxis );
    m_lines->addAxis( rightAxis );

    // assign diagram to chart view
    m_lineChart->coordinatePlane()->replaceDiagram( m_lines );

    for(int i=0; i < ((KDBasicSourcePanel*) m_sourcePanel)->table()->columnCount(); i++){
        QPen p( Qt::SolidLine ); p.setWidth( 5 );
        p.setColor( getDataColor( i, KDChartDemo::DecentBlue ) );
        m_lines->setPen( i, p );
    }
    m_lineChart->update();

}

void KDChartDemo::setupBarChart()
{
    m_bar = new BarDiagram();
    QHBoxLayout* barChartLayout = new QHBoxLayout( BarDiagramTab );
    m_barChart = new Chart();
    m_barChart->setGlobalLeading( 10,  10,  10,  10 );
    barChartLayout->addWidget( m_barChart );

    // hide the Frame:
    KDChart::FrameAttributes frameAttributes ( m_barChart->frameAttributes() );
    frameAttributes.setVisible(false);
    m_barChart->setFrameAttributes ( frameAttributes );

    m_barChart->coordinatePlane()->replaceDiagram( m_bar );

    for(int i=0; i < ((KDBasicSourcePanel*) m_sourcePanel)->table()->columnCount(); i++){
        QPen p( Qt::SolidLine ); p.setWidth( 0 );
        p.setColor( getDataColor(i, KDChartDemo::DecentBlue) );
        m_bar->setPen( i, p );

        QRadialGradient rg;
        QBrush b( rg );
        b.setColor( getDataColor(i, KDChartDemo::DecentBlue) );
        m_bar->setBrush( b );
    }
    m_barChart->update();
}

void KDChartDemo::setupPolarDiagram()
{
    QHBoxLayout* chartLayout = new QHBoxLayout( PolarDiagramTab );
    m_polarChart      = new Chart();
    m_polarChart->setGlobalLeadingLeft( 5 );
    m_polarChart->setGlobalLeadingRight( 5 );
    chartLayout->addWidget( m_polarChart );

    m_polar      = new PolarDiagram();
    m_polarPlane = new PolarCoordinatePlane();

    // hide the frame:
    KDChart::FrameAttributes frameAttributes ( m_polarChart->frameAttributes() );
    frameAttributes.setVisible(false);
    m_polarChart->setFrameAttributes ( frameAttributes );

    m_polarChart->replaceCoordinatePlane( m_polarPlane );
}

void KDChartDemo::setupPieChart()
{
    QHBoxLayout* chartLayout = new QHBoxLayout( PieChartTab );
    m_pieChart = new Chart();
    m_pieChart->setGlobalLeadingLeft( 50 );
    m_pieChart->setGlobalLeadingRight( 50 );
    chartLayout->addWidget( m_pieChart );

    // Set up the diagram
    PolarCoordinatePlane* polarPlane = new PolarCoordinatePlane( m_pieChart );
    m_pie = new PieDiagram();
    m_pieChart->replaceCoordinatePlane( polarPlane );

    // hide the frame:
    KDChart::FrameAttributes frameAttributes ( m_pieChart->frameAttributes() );
    frameAttributes.setVisible(false);
    m_pieChart->setFrameAttributes ( frameAttributes );

    // 3D
    ThreeDPieAttributes attrs( m_pie->threeDPieAttributes() );
    attrs.setEnabled( true );
    attrs.setDepth( 10 );
    attrs.setUseShadowColors( true );
    m_pie->setThreeDPieAttributes( attrs );
    m_pieChart->update();

    QColor c(Qt::lightGray);
    c.setAlpha(70);
    QPen p(c);
    p.setWidth(0);
    m_pie->setPen(p);
    m_pie->setAntiAliasing(true);
}

QColor KDChartDemo::getDataColor( int parameter, KDChartDemo::Style style )
{

    switch(style){
        case KDChartDemo::DecentBlue:
            return QColor( parameter*20, parameter*30,parameter*40 );	// blueish
        case KDChartDemo::DecentRed:
            return QColor( parameter*40, parameter*10,parameter*20 ); // reddish
        case KDChartDemo::DecentBrown:
            return QColor( parameter*60, parameter*40,parameter*20 ); // brownish
        case KDChartDemo::Pastel:
            return QColor( parameter*60, 255-(parameter*40), 255-(parameter*20) ); // pastell
        case KDChartDemo::Rainbow:
            return KDChartDemo::m_rainbowColors[parameter];
        case KDChartDemo::LightBlue:
            return KDChartDemo::m_lightBlueColors[parameter];
            break;
        default:
            break;
    }
    return Qt::black;
}

void KDChartDemo::setupColors()
{

    m_rainbowColors[0] = QColor(255,0,196);
    m_rainbowColors[1] = QColor(255,  0, 96);
    m_rainbowColors[2] = QColor(255,128,64);
    m_rainbowColors[3] = QColor( Qt::yellow );
    m_rainbowColors[4] = QColor( Qt::green );
    m_rainbowColors[5] = QColor( Qt::cyan );
    m_rainbowColors[6] = QColor(96,96,255);
    m_rainbowColors[7] = QColor(160,0,255);

    m_lightBlueColors[1] = QColor(9,0,255);
    m_lightBlueColors[2] = QColor(144,141,255);
    m_lightBlueColors[3] = QColor(5,0,144);
    m_lightBlueColors[4] = QColor(185,185,185);
    m_lightBlueColors[5] = QColor(82,77,255);
    m_lightBlueColors[6] = QColor(27,40,120);
    m_lightBlueColors[7] = QColor(53,80,172);
}

void KDChartDemo::setClassicMode(bool toggled)
{
    if( !toggled )
        return;

    // paint LineChart
    for(int i=0; i < ((KDBasicSourcePanel*) m_sourcePanel)->table()->columnCount(); i++){
        QPen p( Qt::SolidLine ); p.setWidth( 5 );
        p.setColor( getDataColor( i, KDChartDemo::DecentBlue ) );
        m_lines->setPen( i, p );
    }
    // no transparency, no fill
    LineAttributes l(m_lines->lineAttributes());
    l.setDisplayArea( false );
    m_lines->setLineAttributes(l);

    m_lineChart->update();

    // paint PieChart
    // set 3d
    ThreeDPieAttributes attrs( m_pie->threeDPieAttributes() );
    attrs.setEnabled( true );
    m_pie->setThreeDPieAttributes( attrs );
    m_pieChart->update();

    // no pen
    QPen piePen;
    piePen.setStyle(Qt::NoPen);
    m_pie->setPen(2, piePen);

    // not exploded
    PieAttributes pa( m_pie->pieAttributes() );
    pa.setExplodeFactor( 0.0 );
    m_pie->setPieAttributes( pa );

    // decent colors
    for(int i=0; i < ((KDBasicSourcePanel*) m_sourcePanel)->table()->columnCount(); i++){
        m_pie->setBrush(i,getDataColor(i,KDChartDemo::LightBlue));
    }
    // paint PolarDiagram
}

void KDChartDemo::setFreshMode(bool toggled)
{
    if( !toggled )
        return;

    // paint LineChart
    for(int i=0; i < ((KDBasicSourcePanel*) m_sourcePanel)->table()->columnCount(); i++){
        QPen p( Qt::SolidLine ); p.setWidth( 5 );
        p.setColor( getDataColor( i, KDChartDemo::Rainbow ) );
        m_lines->setPen( i, p );

    }
    // set transparency, fill
    LineAttributes l(m_lines->lineAttributes());
    l.setDisplayArea( true );
    l.setTransparency( 50 );
    m_lines->setLineAttributes(l);

    m_lineChart->update();


    // paint Pie Chart
    ThreeDPieAttributes attrs( m_pie->threeDPieAttributes() );
    attrs.setEnabled( true );
    attrs.setDepth( 10 );
    attrs.setUseShadowColors( true );
    m_pie->setThreeDPieAttributes( attrs );
    m_pieChart->update();

    QPen piePen;
    piePen.setStyle(Qt::NoPen);
    m_pie->setPen(2, piePen);

    PieAttributes pa( m_pie->pieAttributes() );
    pa.setExplodeFactor( 0.1 );
    m_pie->setPieAttributes( pa );

    for(int i=0; i < ((KDBasicSourcePanel*) m_sourcePanel)->table()->columnCount(); i++){
        m_pie->setBrush(i,getDataColor(i+1,KDChartDemo::Rainbow));
    }
}

void KDChartDemo::tabHook(int index)
{
    if(index == 3)
    {
        m_sourcePanel->reset();
//        setupPieChart();
    }
}

