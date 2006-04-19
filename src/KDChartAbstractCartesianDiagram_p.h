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

#ifndef KDCHARTABSTRACTCARTESIANDIAGRAM_P_H
#define KDCHARTABSTRACTCARTESIANDIAGRAM_P_H

#include <KDChartAbstractDiagram_p.h>
#include <KDChartAbstractThreeDAttributes.h>
#include <KDChartGridAttributes.h>

namespace KDChart {

  class CartesianCoordinatePlane;

class AbstractCartesianDiagram::Private : public AbstractDiagram::Private
{
    friend class AbstractCartesianDiagram;
public:
    Private();
   ~Private();

   CartesianAxisList axesList;

   AbstractCartesianDiagram* referenceDiagram;
   QPointF referenceDiagramOffset;

   GridAttributes gridAttributes;
};

inline AbstractCartesianDiagram::AbstractCartesianDiagram( Private * p )
    : AbstractDiagram( p ) { init(); }
inline AbstractCartesianDiagram::AbstractCartesianDiagram( Private * p, CartesianCoordinatePlane* parent )
    : AbstractDiagram( p, parent ) { init(); }
inline AbstractCartesianDiagram::Private * AbstractCartesianDiagram::d_func()
{ return static_cast<Private*>( AbstractDiagram::d_func() ); }
inline const AbstractCartesianDiagram::Private * AbstractCartesianDiagram::d_func() const
{ return static_cast<const Private*>( AbstractDiagram::d_func() ); }

}
#endif /* KDCHARTABSTRACTCARTESIANDIAGRAM_P_H */

