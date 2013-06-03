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

#include <QApplication>
#include <KDChartChart>
#include <KDChartPlotter>
#include <KDChartLineDiagram>
#include <KDChartCartesianAxis>
#include <KDChartCartesianCoordinatePlane>
#include <KDChartLegend>
#include <QStandardItemModel>
#include <KDChartDataValueAttributes>
#include <KDChartTextAttributes>
#include <KDChartMarkerAttributes>

#include <cmath>

#define PI 3.141592653589793


// enable the next line to see points instead of lines:
// #define PLOTTED_POINTS


int main( int argc, char** argv )
{
    QApplication app( argc, argv );

#if defined PLOTTED_POINTS
    const int points = 60;
#else
    const int points = 1000;
#endif
    const qreal xMin = -2 * PI;
    const qreal xMax = 2 * PI;
    const qreal step = ( xMax - xMin ) / ( points - 1 );

    QStandardItemModel model( points, 6 );

    double x = xMin;
    for ( int n = 0; n < points; ++n, x += step) {
        QModelIndex index = model.index( n, 0 );
        model.setData( index, x );
        index = model.index( n, 1 );
        model.setData( index, sin( x ) * 100.0 );

        index = model.index( n, 2 );
        model.setData( index, x );
        index = model.index( n, 3 );
        model.setData( index, x );

        index = model.index( n, 4 );
        model.setData( index, x );
        index = model.index( n, 5 );
        model.setData( index, x * x * x );
    }

    model.setHeaderData( 0, Qt::Horizontal, QString::fromLatin1( "100 * sin(x)" ) );
    model.setHeaderData( 2, Qt::Horizontal, QString::fromLatin1( "x" ) );
    model.setHeaderData( 4, Qt::Horizontal, QString::fromLatin1( "x^3" ) );

    KDChart::Chart* chart = new KDChart::Chart();

    KDChart::AbstractCartesianDiagram* diagram = new KDChart::Plotter;
    diagram->setModel( &model );
    chart->coordinatePlane()->replaceDiagram( diagram );

#if defined PLOTTED_POINTS
    diagram->setPen( QPen(Qt::NoPen) );
    const int colCount = model.columnCount( diagram->rootIndex() );
    for ( int iColumn = 0; iColumn<colCount; ++iColumn ) {
        const QPen markerPen( diagram->brush( iColumn ).color() );
        KDChart::DataValueAttributes dva( diagram->dataValueAttributes( iColumn ) );
        KDChart::TextAttributes ta( dva.textAttributes() );
        KDChart::MarkerAttributes ma( dva.markerAttributes() );
        ma.setPen( markerPen );
        ma.setMarkerStyle( KDChart::MarkerAttributes::MarkerCircle );
        ma.setMarkerSize( QSize( 3,3 ) );

        dva.setVisible( true );
        ta.setVisible( false );
        ma.setVisible( true );
        dva.setTextAttributes(   ta );
        dva.setMarkerAttributes( ma );
        diagram->setDataValueAttributes( iColumn, dva );
    }
#endif

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
