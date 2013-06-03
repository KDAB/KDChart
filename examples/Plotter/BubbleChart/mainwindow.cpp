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

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartPlotter>
#include <KDChartDataValueAttributes>
#include <KDChartTextAttributes>
#include <KDChartMarkerAttributes>

#include <QStandardItemModel>
#include <QTimer>


using namespace KDChart;



static const int nBubbles = 7;
// we display seven bubbles using the following data structure:
// 0: Y
// 1: X
// 2: size
struct DataType{
    DataType( qreal x_,
              qreal y_,
              qreal size_ )
    : x(x_)
    , y(y_)
    , size(size_)
    {}
    qreal x;
    qreal y;
    qreal size;
};
static const DataType bubblesData[nBubbles] = {
    DataType(0.5, 1.0, 100),
    DataType(1.0, 0.5, 60 ),
    DataType(1.6, 2.0, 28 ),
    DataType(0.7, 0.3, 55 ),
    DataType(1.3, 2.0, 95 ),
    DataType(2.0, 1.0, 75 ),
    DataType(1.4, 1.1, 85 )
};

#define ROLE_SIZE Qt::UserRole + 1



MainWindow::MainWindow( QWidget* parent ) :
    QWidget( parent ),
    m_model(0)
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );
    // Set up the data
    initializeDataModel();
    // Set up the diagram
    m_plotter = new Plotter();
    // Register the data model at the diagram
    m_plotter->setModel( m_model );
    // Add axes to the diagram
    CartesianAxis *xAxis  = new CartesianAxis( m_plotter );
    CartesianAxis *xAxis2 = new CartesianAxis( m_plotter );
    CartesianAxis *yAxis  = new CartesianAxis ( m_plotter );
    CartesianAxis *yAxis2 = new CartesianAxis ( m_plotter );
    xAxis->setPosition ( KDChart::CartesianAxis::Bottom );
    xAxis2->setPosition( KDChart::CartesianAxis::Top );
    yAxis->setPosition ( KDChart::CartesianAxis::Left );
    yAxis2->setPosition( KDChart::CartesianAxis::Right );
    m_plotter->addAxis( xAxis );
    m_plotter->addAxis( xAxis2 );
    m_plotter->addAxis( yAxis );
    m_plotter->addAxis( yAxis2 );
    connect( threeDEnabled, SIGNAL( toggled(bool) ), this, SLOT( setMarkerAttributes() ) );

    m_chart->coordinatePlane()->replaceDiagram( m_plotter );
    m_chart->setGlobalLeading( 20, 20, 20, 20 );

    setMarkerAttributes();
}


void MainWindow::initializeDataModel()
{
    m_model = new QStandardItemModel( nBubbles, 2 );
    m_model->setHeaderData(0, Qt::Horizontal, tr("Some Bubbles"));
    for ( int i=0;  i < nBubbles;  ++i ) {
        const QModelIndex indexX = m_model->index(i, 0);
        const QModelIndex indexY = m_model->index(i, 1);

        m_model->setData(indexX, QVariant( bubblesData[i].x ), Qt::DisplayRole);

        m_model->setData(indexY, QVariant( bubblesData[i].y ), Qt::DisplayRole);

        m_model->setData(indexY, bubblesData[i].size, ROLE_SIZE );
    }
}


void MainWindow::setMarkerAttributes()
{
    // disable the connecting line
    m_plotter->setPen( Qt::NoPen );

    for ( int iRow = 0; iRow<nBubbles; ++iRow ) {
        const QModelIndex indexX = m_plotter->model()->index(iRow, 0);
        const QModelIndex indexY = m_plotter->model()->index(iRow, 1);
        DataValueAttributes dva( m_plotter->dataValueAttributes( indexX ) );
        dva.setVisible( true );

        TextAttributes ta( dva.textAttributes() );
        ta.setVisible( false );

        MarkerAttributes ma( dva.markerAttributes() );
        ma.setVisible( true );
        ma.setMarkerStyle( MarkerAttributes::MarkerCircle );
        ma.setThreeD( threeDEnabled->isChecked() );

        // set the size
        const qreal d = m_model->data( indexY, ROLE_SIZE ).toReal();
        ma.setMarkerSize( QSizeF(d,d) );

        RelativePosition pos( dva.positivePosition() );
        pos.setAlignment( Qt::AlignCenter );
        pos.setHorizontalPadding(0);

        dva.setPositivePosition( pos );
        dva.setMarkerAttributes( ma );
        dva.setTextAttributes( ta );

        // note: The KDChart::Plotter looks at the X cell
        //       for data value attributes,
        //       any attrs set at the Y cell are ignored.
        m_plotter->setDataValueAttributes( indexX, dva );
    }
    m_chart->update();
}
