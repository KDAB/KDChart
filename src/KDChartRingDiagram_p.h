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

#ifndef KDCHARTRINGDIAGRAM_P_H
#define KDCHARTRINGDIAGRAM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartAbstractPieDiagram_p.h"

//FIXME(khz): use an internal libfakes library instead of this internal header file
#include "kdchart_platformdef.h"


namespace KDChart {

/**
 * \internal
 */
class RingDiagram::Private : public AbstractPieDiagram::Private
{
    friend class RingDiagram;
public:
    Private();
    ~Private();

private:
    bool relativeThickness;
};

inline RingDiagram::RingDiagram( Private * p, PolarCoordinatePlane* plane )
    : AbstractPieDiagram( p, plane ) { init(); }
inline RingDiagram::Private * RingDiagram::d_func()
{ return static_cast<Private*>( AbstractPieDiagram::d_func() ); }
inline const RingDiagram::Private * RingDiagram::d_func() const
{ return static_cast<const Private*>( AbstractPieDiagram::d_func() ); }

}

#endif /* KDCHARTRINGDIAGRAM_P_H */

