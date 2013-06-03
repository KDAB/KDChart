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
#include <KDChartWidget>
#include <KDChartBarDiagram>
#include <QPen>
#include <KDChartThreeDBarAttributes>

using namespace KDChart;

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    Widget widget;
    widget.resize( 600, 600 );

    QVector< qreal > vec0,  vec1;

    vec0 << 5 << 4 << 3 << 2 << 1 << 0
         << 1 << 2 << 3 << 4 << 5;
    vec1 << 25 << 16 << 9 << 4 << 1 << 0
         << 1 << 4 << 9 << 16 << 25;

    widget.setDataset( 0, vec0, "vec0" );
    widget.setDataset( 1, vec1, "vec1" );
    widget.setType( Widget::Bar ,  Widget::Stacked );

    //Configure a pen and draw a line
    //surrounding the bars
    QPen pen;
    pen.setWidth( 2 );
    pen.setColor(  Qt::darkGray );
    // call your diagram and set the new pen
    widget.barDiagram()->setPen(  pen );

    //Set up your ThreeDAttributes
    //display in ThreeD mode
    ThreeDBarAttributes td( widget.barDiagram()->threeDBarAttributes() );
    td.setDepth(  15 );
    td.setEnabled(  true );
    widget.barDiagram()->setThreeDBarAttributes( td );
    widget.setGlobalLeadingTop( 20 );
    widget.show();

    return app.exec();
}
