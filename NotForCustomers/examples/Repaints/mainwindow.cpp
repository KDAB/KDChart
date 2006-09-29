/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#include "mainwindow.h"

#include <KDChartChart>
#include <KDChartAbstractCoordinatePlane>
#include <KDChartCartesianAxis>
#include <KDChartLineDiagram>
#include <KDChartLegend>
#include <KDChartPosition>

#include <QDebug>
#include <QPainter>
#include <QString>
#include <KDChartDatasetProxyModel>



using namespace KDChart;

MainWindow::MainWindow( QAbstractItemModel* model, QWidget* parent ) :
    QWidget( parent ), m_pModel( model )
{
    setupUi( this );

    QHBoxLayout* chartLayout = new QHBoxLayout( chartFrame );
    m_chart = new Chart();
    chartLayout->addWidget( m_chart );

    // Initalize some default colors
    QList< QColor > colors;
    colors << Qt::red << Qt::yellow << Qt::green << Qt::cyan << Qt::blue << Qt::magenta << Qt::black << Qt::white ;

    // Setup a set of  diagrams;
    QList< LineDiagram* > listDiagrams;
    const int numdiagrams = 5;

    for ( int i=1; i<numdiagrams+1; i++ )
    {
        LineDiagram* diagram = new LineDiagram();
        //diagram->setAntiAliasing( false );

        // Each diagram displays one curve: column 0 vs column i
        DatasetProxyModel* proxy = new DatasetProxyModel();
        DatasetDescriptionVector columns;
        columns << 0 << i ;
        proxy->setSourceModel( m_pModel );
        proxy->setDatasetColumnDescriptionVector( columns );
       
        // Each diagram gets its own color (pen and brush)
        diagram->setPen( QPen( colors[i-1], 2 ) );
        diagram->setBrush( 1, colors[i-1] );
        diagram->setModel( proxy );
        diagram->setDatasetDimension( 2 );
        listDiagrams << diagram ;
    }

    // Specify all the diagrams to the coordinateplane
    m_chart->coordinatePlane()->replaceDiagram( listDiagrams[0] );
    for ( int i=1; i<numdiagrams; i++ )
    {
        m_chart->coordinatePlane()->addDiagram( listDiagrams[i] );
    }

    // Construct a legend
    Legend* legend = new Legend( (AbstractDiagram*)0, m_chart );
    legend->setTitleText( "Legend" );
    legend->setOrientation( Qt::Vertical );
    legend->setPosition( Position::West );

    // Specify all the diagrams to the legend, while printing the titles of
    // the columns the diagrams paint. 
    for ( int i=0; i<numdiagrams; i++ )
    {
        QString title = listDiagrams[i]->model()->headerData( 1, Qt::Horizontal ).toString();
        qDebug() << "Added:" << title ;
        legend->addDiagram( listDiagrams[i] );
    }

    // Construct a vertical axis
    CartesianAxis* axis = new CartesianAxis;
    axis->setPosition( CartesianAxis::Left );
    
    CartesianAxis* axis2 = new CartesianAxis;
    axis2->setPosition( CartesianAxis::Bottom );

    for ( int i=0; i<numdiagrams; i++ )
    {
        listDiagrams[i]->addAxis( axis );
        listDiagrams[i]->addAxis( axis2 );
    }


    m_chart->addLegend( legend );
}
