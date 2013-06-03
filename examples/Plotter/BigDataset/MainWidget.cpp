/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "MainWidget.h"

#include "KDChartAbstractDiagram_p.h"
#include "KDChartChart.h"
#include "KDChartPlotter.h"

#include <QHBoxLayout>

#include <cmath>

MainWidget::MainWidget()
   : m_controlsContainer( new QWidget( this ) )
{
    // set up the chart

    QHBoxLayout* topLayout = new QHBoxLayout( this );
    m_controls.setupUi( m_controlsContainer );
    topLayout->addWidget( m_controlsContainer );

    KDChart::Chart* chart = new KDChart::Chart;
    topLayout->addWidget( chart );

    m_plotter = new KDChart::Plotter;
    m_plotter->setModel( &m_model );
    KDChart::AbstractDiagram::Private::get( m_plotter )->doDumpPaintTime = true;
    chart->coordinatePlane()->replaceDiagram( m_plotter );

    KDChart::CartesianCoordinatePlane* cPlane
        = qobject_cast< KDChart::CartesianCoordinatePlane* >( chart->coordinatePlane() );
    Q_ASSERT( cPlane );
    cPlane->setVerticalRange( QPair< qreal, qreal >( -2, 2 ) );

    KDChart::CartesianAxis* xAxis = new KDChart::CartesianAxis( m_plotter );
    xAxis->setPosition( KDChart::CartesianAxis::Bottom );
    xAxis->setTitleText("X");
    m_plotter->addAxis( xAxis );

    KDChart::CartesianAxis* yAxis = new KDChart::CartesianAxis( m_plotter );
    yAxis->setPosition( KDChart::CartesianAxis::Left );
    yAxis->setTitleText("Y");
    m_plotter->addAxis( yAxis );

    // wire up the UI

    // index of elements in vector must match corresponding Model::Function enum values
    m_functionSelector << m_controls.sineRadio << m_controls.triangleRadio << m_controls.squareRadio
                       << m_controls.noiseRadio << m_controls.oneDivSineRadio
                       << m_controls.sineOneDivRadio;
    foreach ( QRadioButton* r, m_functionSelector ) {
        connect( r, SIGNAL( toggled( bool ) ), SLOT( functionToggled( bool ) ) );
    }

    connect( m_controls.runButton, SIGNAL( toggled( bool ) ),
             &m_model, SLOT( setRunning( bool ) ) );

    // order matters again
    m_addPointsButtons << m_controls.add1kButton << m_controls.add10kButton << m_controls.add100kButton;
    foreach ( QPushButton* b, m_addPointsButtons ) {
        connect( b, SIGNAL( clicked( bool ) ), SLOT( addPointsButtonClicked() ) );
    }
}

// slot
void MainWidget::functionToggled( bool checked )
{
    if ( !checked ) {
        return;
    }
    int idx = m_functionSelector.indexOf( qobject_cast< QRadioButton* >( sender() ) );
    Q_ASSERT( idx >= 0 );
    m_model.setFunction( static_cast< Model::Function >( idx ) );
}

// slot
void MainWidget::addPointsButtonClicked()
{
    int idx = m_addPointsButtons.indexOf( qobject_cast< QPushButton* >( sender() ) );
    Q_ASSERT( idx >= 0 );
    m_model.appendPoints( pow( qreal( 10 ), qreal( idx + 3 ) ) );
}
