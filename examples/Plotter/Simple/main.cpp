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

#include <QApplication>
#include <KDChartChart>
#include <KDChartPlotter>
#include <KDChartLineDiagram>
#include <KDChartCartesianAxis>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLegend>
#include <QStandardItemModel>

#include <cmath>

#define PI 3.141592653589793

int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    const int points = 300;
    const double xMin = -2 * PI;
    const double xMax = 2 * PI;
    const double step = ( xMax - xMin ) / ( points - 1 );
    
    QStandardItemModel model( points, 4 );
   
    double x = xMin;
    for( int n = 0; n < points; ++n, x += step) {
        QModelIndex index = model.index( n, 0 );
        model.setData( index, QVariant( x ) );
        index = model.index( n, 1 );
        model.setData( index, QVariant( sin( x ) * 100 ) );

        index = model.index( n, 2 );
        model.setData( index, QVariant( x ) );
        index = model.index( n, 3 );
        model.setData( index, QVariant( x*x*x ) );
    }

    model.setHeaderData( 0, Qt::Horizontal, QString::fromLatin1( "100 * sin(x)" ) );
    model.setHeaderData( 1, Qt::Horizontal, QString::fromLatin1( "x^3" ) );

    KDChart::Chart* chart = new KDChart::Chart();

    KDChart::AbstractCartesianDiagram* diagram = new KDChart::Plotter;
    diagram->setModel( &model );
    chart->coordinatePlane()->replaceDiagram( diagram );
   
    KDChart::CartesianAxis* xAxis = new KDChart::CartesianAxis( diagram );
    KDChart::CartesianAxis* yAxis = new KDChart::CartesianAxis( diagram );
    xAxis->setPosition( KDChart::CartesianAxis::Bottom );
    yAxis->setPosition( KDChart::CartesianAxis::Left );
    diagram->addAxis( xAxis );
    diagram->addAxis( yAxis );

    KDChart::Legend* legend = new KDChart::Legend( diagram, chart );
    legend->setPosition( KDChart::Position::East );
    legend->setAlignment( Qt::AlignCenter );
    legend->setTitleText( "Legend" );
    chart->addLegend( legend );

    chart->show();

    return app.exec();
}
