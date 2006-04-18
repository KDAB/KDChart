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

#ifndef KDCHARTAXIS_P_H
#define KDCHARTAXIS_P_H

#include "KDChartAbstractArea_p.h"
#include <KDChartTextAttributes.h>
#include <KDChartAbstractDiagram.h>


namespace KDChart {

class AbstractAxis::Private : public AbstractArea::Private
{
    friend class AbstractAxis;

public:
    Private();
    ~Private();

    AbstractDiagram* diagram() const;

private:
    AbstractDiagram* parent;
};


inline AbstractAxis::AbstractAxis( Private * p, AbstractDiagram* parent_ )
    :  AbstractArea( p, parent_ )
{
    p->parent = parent_;
    init();
}

inline AbstractDiagram* AbstractAxis::Private::diagram() const
{
    return parent;
}

inline AbstractAxis::Private * AbstractAxis::d_func()
{ return static_cast<Private*>( AbstractArea::d_func() ); }
inline const AbstractAxis::Private * AbstractAxis::d_func() const
{ return static_cast<const Private*>( AbstractArea::d_func() ); }

}
#endif /* KDCHARTAREA_P_H */

