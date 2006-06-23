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

#ifndef KDCHARTABSTRACTCOORDINATEPLANE_P_H
#define KDCHARTABSTRACTCOORDINATEPLANE_P_H

#include <KDChartGridAttributes.h>

namespace KDChart {

class AbstractCoordinatePlane::Private
{
    friend class AbstractCoordinatePlane;
protected:
    explicit Private()
        :referenceCoordinatePlane(0)
    {}
    virtual ~Private() {};

    AbstractDiagramList diagrams;
    GridAttributes gridAttributes;
    AbstractCoordinatePlane *referenceCoordinatePlane;
};

inline AbstractCoordinatePlane::AbstractCoordinatePlane( Private * p ) : _d( p ) {}
inline AbstractCoordinatePlane::AbstractCoordinatePlane( Private * p, QWidget* parent )
  : QWidget( parent ), _d( p ) {}

}
#endif /* KDCHARTABSTRACTCOORDINATEPLANE_P_H*/
