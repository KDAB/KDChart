/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartLineDiagram>

#include <QDebug>
#include <QPainter>

#define SET_RANGE       // set all diagrams to the same horizontal range

static const bool bUseAlignmentTrick = true;
static const int iNoDiagrams = 5;

using namespace KDChart;

MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );

    m_model.loadFromCSV( ":/data" );

    // Set up the diagrams

    for ( int i = 0; i < iNoDiagrams; ++i )
    {
        m_diagrams.push_back(new LineDiagram());
        m_diagrams.at(i)->setModel(&m_model);
        if ( i > 0 )
        {
            m_planes.push_back(new CartesianCoordinatePlane(m_chart));
        }
#ifdef SET_RANGE
        if ( i > 0 )
        {
            m_planes.at(i-1)->setHorizontalRange(QPair<qreal, qreal>(1,10));
        }
        else
        {
            CartesianCoordinatePlane* plane = static_cast<CartesianCoordinatePlane*>(m_chart->coordinatePlane());
            plane->setHorizontalRange(QPair<qreal, qreal>(1,10));
        }
#endif
        CartesianAxis* yAxis = new CartesianAxis ( m_diagrams.at(i) );
        yAxis->setPosition( KDChart::CartesianAxis::Left );
        m_diagrams.at(i)->addAxis(yAxis);
    }
    CartesianAxis* xAxis;
    if ( iNoDiagrams == 1 || !bUseAlignmentTrick )
    {
        xAxis = new CartesianAxis ( m_diagrams.at(iNoDiagrams-1) );
    }
    else
    {
        xAxis = new CartesianAxis ( m_diagrams.at(iNoDiagrams-2) );
    }
    xAxis->setPosition( KDChart::CartesianAxis::Bottom );
    // rotate the x-axis labels
    TextAttributes ta = xAxis->textAttributes();
    ta.setRotation(90);
    //ta.setAutoRotate(true);
    xAxis->setTextAttributes(ta);
    // set some long label texts
    QStringList sLabels;
    sLabels.push_back("00000000001");
    sLabels.push_back("00000000002");
    sLabels.push_back("00000000003");
    sLabels.push_back("00000000004");
    sLabels.push_back("00000000005");
    sLabels.push_back("00000000006");
    sLabels.push_back("00000000007");
    sLabels.push_back("00000000008");
    sLabels.push_back("00000000009");
    sLabels.push_back("00000000010");
    xAxis->setLabels(sLabels);
    m_diagrams.at(iNoDiagrams-1)->addAxis(xAxis);

    m_chart->coordinatePlane()->replaceDiagram( m_diagrams.at(0) );
    m_chart->setGlobalLeading(10.,10.,10.,100.);
    for ( int i = 1; i < iNoDiagrams; ++i )
    {
        m_planes.at(i-1)->replaceDiagram( m_diagrams.at(i) );
        m_chart->addCoordinatePlane( m_planes.at(i-1) );
    }
}
