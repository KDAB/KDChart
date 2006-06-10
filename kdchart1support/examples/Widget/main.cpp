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

#include <KDChart1Widget.h>
#include <KDChart1Table.h>

#include <QDebug>
#include <QVector>

#include "mainwindow.h"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );

    MainWindow window;

    window.resize( 800, 600 );

    KDChart1TableData d( 3, 11 );
    // 1st series
    d.setCell( 0, 0,   -5      );
    d.setCell( 0, 1,   -4      );
    d.setCell( 0, 2,   -3      );
    d.setCell( 0, 3,   -2      );
    d.setCell( 0, 4,   -1      );
    d.setCell( 0, 5,    0      );
    d.setCell( 0, 6,    1      );
    d.setCell( 0, 7,    2      );
    d.setCell( 0, 8,    3      );
    d.setCell( 0, 9,    4      );
    d.setCell( 0,10,    5      );

    // 2nd series
    d.setCell( 1, 0,   25      );
    d.setCell( 1, 1,   16      );
    d.setCell( 1, 2,    9      );
    d.setCell( 1, 3,    4      );
    d.setCell( 1, 4,    1      );
    d.setCell( 1, 5,    0      );
    d.setCell( 1, 6,    1      );
    d.setCell( 1, 7,    4      );
    d.setCell( 1, 8,    9      );
    d.setCell( 1, 9,   16      );
    d.setCell( 1,10,   25      );

    // 3rd series
    d.setCell( 2, 0, -125      );
    d.setCell( 2, 1,  -64      );
    d.setCell( 2, 2,  -27      );
    d.setCell( 2, 3,   -8      );
    d.setCell( 2, 4,   -1      );
    d.setCell( 2, 5,    0      );
    d.setCell( 2, 6,    1      );
    d.setCell( 2, 7,    8      );
    d.setCell( 2, 8,   27      );
    d.setCell( 2, 9,   64      );
    d.setCell( 2,10,  125      );


    window.widget->setData( &d );
    /*
    window.widget->addHeaderFooter( "Header West",   HeaderFooter::Header, Position::West );
    window.widget->addHeaderFooter( "Header Center", HeaderFooter::Header, Position::Center );
    window.widget->addHeaderFooter( "Header East",   HeaderFooter::Header, Position::East );
    window.widget->addHeaderFooter( "Footer West",   HeaderFooter::Footer, Position::West );
    window.widget->addHeaderFooter( "Footer Center", HeaderFooter::Footer, Position::Center );
    window.widget->addHeaderFooter( "Footer East",   HeaderFooter::Footer, Position::East );
    */
    //window.widget->addLegend( Position::East );

    //window.widget->setType( Widget::Polar );

    window.show();

    return app.exec();
}
