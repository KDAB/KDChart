/* -*- Mode: C++ -*-
   KDChart1 - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart1 library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#include "KDChart1_Presentation.h"
#include <qapplication.h>
#ifdef QWS
#include <qpeapplication.h>
#endif

int main( int argc, char **argv )
{
#ifdef QWS
    QPEApplication a( argc, argv );
#else
    QApplication a( argc, argv );
#endif
    KDChart1PresentationWidget t( argc, argv );

    t.setWindowTitle( "KDChart1 Real Time Configuration Examples" );
#ifdef QWS
    t.showMaximized();
#else
    t.resize( 1024, 700 );
#endif
    t.show();

    return a.exec();
}
