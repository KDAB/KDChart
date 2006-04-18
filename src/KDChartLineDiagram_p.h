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

#ifndef KDCHARTLINEDIAGRAM_P_H
#define KDCHARTLINEDIAGRAM_P_H


#include "KDChartAbstractCartesianDiagram_p.h"

namespace KDChart {

  //class CartesianCoordinatePlane;

class LineDiagram::Private : public AbstractCartesianDiagram::Private
{
    friend class LineDiagram;
public:
    Private();
    ~Private();

    LineType lineType;
};


inline LineDiagram::LineDiagram( Private * p, CartesianCoordinatePlane* plane )
  : AbstractCartesianDiagram( p, plane ) { init(); }
inline LineDiagram::Private * LineDiagram::d_func()
{ return static_cast<Private*>( AbstractCartesianDiagram::d_func() ); }
inline const LineDiagram::Private * LineDiagram::d_func() const
{ return static_cast<const Private*>( AbstractCartesianDiagram::d_func() ); }

}

#endif /* KDCHARTLINEDIAGRAM_P_H */

