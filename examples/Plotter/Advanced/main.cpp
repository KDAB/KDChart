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

#include <QtGui>

#include <KDChartChart>
#include <KDChartGridAttributes>
#include <KDChartFrameAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartPlotter>
#include <KDChartLegend>
#include <KDChartBackgroundAttributes>

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    KDChart::Chart chart;

    QStandardItemModel model;

    model.setRowCount(100);
    model.setColumnCount(6); // 3 data sets
    for (int i=0; i<100; ++i) {
        // dateset 1
        qreal t = i;
        qreal v = qreal(i)*i;
        QModelIndex index = model.index( i, 0 );
        model.setData( index, QVariant( t ) );
        index = model.index( i, 1 );
        model.setData( index, QVariant( v ) );

        // dateset 2
        t = i*2;
        v = qreal(i)*i-i;
        index = model.index( i, 2 );
        model.setData( index, QVariant( t ) );
        index = model.index( i, 3 );
        model.setData( index, QVariant( v ) );

        // dateset 3
        t = qreal(i)/2;
        v = qreal(i)*i+i;
        index = model.index( i, 4 );
        model.setData( index, QVariant( t ) );
        index = model.index( i, 5 );
        model.setData( index, QVariant( v ) );
    }

    model.setHeaderData( 0, Qt::Horizontal, "Dataset 1" );
    model.setHeaderData( 2, Qt::Horizontal, "Dataset 2" );
    model.setHeaderData( 4, Qt::Horizontal, "Dataset 3" );

    // general chart layout
    KDChart::FrameAttributes fm = chart.frameAttributes();
    fm.setVisible(true);
    fm.setPen( QPen(Qt::black) );
    chart.setFrameAttributes(fm);
    chart.setGlobalLeading( 10, 0, 10, 10 );

    KDChart::BackgroundAttributes chart_bg;
    chart_bg.setBrush(Qt::white);
    chart_bg.setVisible(true);
    chart.setBackgroundAttributes(chart_bg);

    // coordinate plane setup
    KDChart::AbstractCoordinatePlane * plane1 = chart.coordinatePlane();
    plane1->setRubberBandZoomingEnabled(true);

    // create cartesian diagrams
    KDChart::Plotter * plotter = new KDChart::Plotter;
    plotter->setHidden( 0, true );
    plotter->setAntiAliasing(false);
    plotter->setModel( &model );
    plane1->replaceDiagram( plotter );

    // customize grids
    KDChart::CartesianCoordinatePlane * cp1 = static_cast<KDChart::CartesianCoordinatePlane *>(plane1);
    KDChart::GridAttributes gv = cp1->gridAttributes( Qt::Vertical );
    QPen gridPen(QColor(200,100,100));
    gridPen.setStyle(Qt::DashLine);
    gv.setGridPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    gridPen.setColor( QColor(255,155,155) );
    gv.setSubGridPen(gridPen);
    cp1->setGridAttributes( Qt::Vertical,  gv );

    // axis
    KDChart::CartesianAxis * xAxis = new KDChart::CartesianAxis( plotter );
    xAxis->setPosition( KDChart::CartesianAxis::Bottom );
    xAxis->setTitleText("X-Title");

    KDChart::TextAttributes att = xAxis->titleTextAttributes();
    QFont f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize( KDChart::Measure(16) );
    xAxis->setTitleTextAttributes(att);

    KDChart::CartesianAxis * y1Axis = new KDChart::CartesianAxis( plotter );
    y1Axis->setPosition( KDChart::CartesianAxis::Left );
    y1Axis->setTitleText("Y-Title");

    att = y1Axis->titleTextAttributes();
    f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize( KDChart::Measure(16) );
    y1Axis->setTitleTextAttributes(att);

    // add the axis to the plotter
    plotter->addAxis( xAxis );
    plotter->addAxis( y1Axis );

    // create legend
    KDChart::Legend * legend = new KDChart::Legend( plotter, &chart );
    chart.addLegend( legend );
    att = legend->textAttributes();
    f = att.font();
    f.setBold(false);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize( KDChart::Measure(16) );
    legend->setTextAttributes(att);

    legend->setPosition( KDChart::Position::East );
    legend->setAlignment( Qt::AlignCenter );
    legend->setTitleText( "Curves" );
    att = legend->titleTextAttributes();
    f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize( KDChart::Measure(16) );
    legend->setTitleTextAttributes(att);

    KDChart::BackgroundAttributes legend_bg;
    legend_bg.setBrush(Qt::white);
    legend_bg.setVisible(true);
    legend->setBackgroundAttributes(legend_bg);

    KDChart::DataValueAttributes attr = plotter->dataValueAttributes();
    KDChart::TextAttributes tattr = attr.textAttributes();
    tattr.setRotation( 0 );
    attr.setTextAttributes( tattr );
    plotter->setDataValueAttributes( attr );

    // customize marker properties

    // Dataset 1 : green, MarkerRing, no line
    QColor SERIES_1_OUTLINE = QColor(0,128,0);
    attr = plotter->dataValueAttributes(0);
    KDChart::MarkerAttributes mattr = attr.markerAttributes();
    mattr.setMarkerColor(SERIES_1_OUTLINE);
    mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerRing);
    mattr.setMarkerSize(QSizeF(6.0, 6.0));
    mattr.setVisible(true);
    attr.setMarkerAttributes(mattr);
    attr.setVisible(true);
    plotter->setDataValueAttributes(0, attr);
    plotter->setPen(0, Qt::NoPen);

    // Dataset 2 : MarkerDiamond, (black outline, red inside), no line
    QColor SERIES_2_INSIDE = QColor(255,100,100);
    attr = plotter->dataValueAttributes(1);
    mattr = attr.markerAttributes();
    mattr.setMarkerColor(SERIES_2_INSIDE);
    mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerDiamond);
    mattr.setMarkerSize(QSizeF(8.0, 8.0));
    mattr.setVisible(true);
    attr.setMarkerAttributes(mattr);
    attr.setVisible(true);
    plotter->setDataValueAttributes(1, attr);
    plotter->setPen(1, Qt::NoPen);

    // Dataset 3 : MarkerCircle, (green outline, yellow inside), blue line
    QColor SERIES_3_INSIDE = QColor("yellow");
    QColor SERIES_3_LINE = QColor("navy");
    attr = plotter->dataValueAttributes(2);
    mattr = attr.markerAttributes();
    mattr.setMarkerColor(SERIES_3_INSIDE);
    mattr.setMarkerStyle(KDChart::MarkerAttributes::MarkerCircle);
    mattr.setMarkerSize(QSizeF(8.0, 8.0));
    mattr.setVisible(true);
    attr.setMarkerAttributes(mattr);
    attr.setVisible(true);
    plotter->setDataValueAttributes(2, attr);
    plotter->setPen(2, QPen(SERIES_3_LINE));

    chart.show();
    return a.exec();
}
