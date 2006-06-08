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
#ifndef __KDCHART1_H__
#define __KDCHART1_H__

#ifdef QSA
#include <qsproject.h>
#endif // QSA

#include "KDChart1Global.h"
#include "KDChart1Table.h"
#include "KDChart1UnknownTypeException.h"
#include "KDChart1Params.h"
#include "KDChart1DataRegion.h"

/** \file KDChart1.h
  \brief Definition of a single entry-point to the charting engine for
  applications that wish to provide their own QPainter.

  It is not useful to instantiate the KDChart1 class as it only contains
  static methods.
  */

class KDChart1Painter;
class QPainter;
class QSInterpreter;


class KDCHART1_EXPORT KDChart1
{
    friend void cleanupPainter();

    public:
    static bool setupGeometry( QPainter*             painter,
                               KDChart1Params*        params,
                               KDChart1TableDataBase* data,
                               const QRect&          drawRect );

    static bool painterToDrawRect( QPainter* painter, QRect& drawRect );

    static void paint( QPainter*, KDChart1Params*, KDChart1TableDataBase*,
            KDChart1DataRegionList* regions = 0,
            const QRect* rect = 0,
            bool mustCalculateGeometry = true );

    static void print( QPainter*, KDChart1Params*, KDChart1TableDataBase*,
            KDChart1DataRegionList* regions = 0,
            const QRect* rect = 0,
            bool mustCalculateGeometry = true );

#ifdef QSA
    static void initInterpreter( QSInterpreter* );
    static void initProject( QSProject* );
#endif

    private:
    KDChart1(); // prevent instantiations

    static KDChart1Params* oldParams;
    static KDChart1Painter* cpainter;
    static KDChart1Painter* cpainter2;
    static KDChart1Params::ChartType cpainterType;
    static KDChart1Params::ChartType cpainterType2;

#ifdef QSA
    static QString globals();
    static void privateInitInterpreter( QSInterpreter* interpreter );
#endif
};

#endif
