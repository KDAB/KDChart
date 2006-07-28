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
#include <KDChartWidget>
#include <KDChartLineDiagram>
//#include <KDChartThreeDLineAttributes>


using namespace KDChart;

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    Widget widget;
    widget.resize( 600, 600 );

    QVector< double > vec0,  vec1,  vec2;

    vec0 << 5 << 1 << 3 << 4 << 1;
    vec1 << 3 << 6 << 2 << 4 << 8;
    vec2 << 0 << 7 << 1 << 2 << 1;

    widget.setDataset( 0, vec0, "vec0" );
    widget.setDataset( 1, vec1, "vec1" );
    widget.setDataset( 2, vec2, "vec2" );
    widget.setSubType(  Widget::Percent );

    //Configure a pen and draw a line
    //a dashed line for column 1
    QPen pen;
    pen.setWidth( 3 );
    pen.setStyle( Qt::DashDotLine );
    pen.setColor(  Qt::green );
    // call your diagram and set the new pen
    widget.lineDiagram()->setPen(  1 , pen );

    //Set up your ThreeDAttributes
    //display in ThreeD mode
    LineAttributes la;
    la.setDisplayArea(  true );
    la.setTransparency( 25 );
    widget.lineDiagram()->setLineAttributes( la );

    widget.show();

    return app.exec();
}
