/****************************************************************************
 ** Copyright (C) 2007 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTLEVEYJENNINGLAXIS_P_H
#define KDCHARTLEVEYJENNINGSAXIS_P_H

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

#include "KDChartLeveyJenningsAxis.h"
#include "KDChartLeveyJenningsDiagram.h"
#include "KDChartCartesianAxis_p.h"

#include <KDABLibFakes>


namespace KDChart {

/**
  * \internal
  */
class LeveyJenningsAxis::Private : public CartesianAxis::Private
{
    friend class LeveyJenningsAxis;

public:
    Private( LeveyJenningsDiagram* diagram, LeveyJenningsAxis* axis )
        : CartesianAxis::Private( diagram, axis )
    {}
    ~Private() {}

private:
    LeveyJenningsGridAttributes::GridType type;
};

inline LeveyJenningsAxis::LeveyJenningsAxis( Private * p, AbstractDiagram* diagram )
    : CartesianAxis( p, diagram )
{
    init();
}
inline LeveyJenningsAxis::Private * LeveyJenningsAxis::d_func()
{ return static_cast<Private*>( CartesianAxis::d_func() ); }
inline const LeveyJenningsAxis::Private * LeveyJenningsAxis::d_func() const
{ return static_cast<const Private*>( CartesianAxis::d_func() ); }

}

#endif
