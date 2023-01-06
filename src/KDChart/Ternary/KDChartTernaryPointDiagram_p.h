/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTTERNARYPOINTDIAGRAM_P_H
#define KDCHARTTERNARYPOINTDIAGRAM_P_H

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

#include <QtDebug>

#include "KDChartAbstractTernaryDiagram_p.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
class TernaryPointDiagram::Private : public AbstractTernaryDiagram::Private
{
    friend class TernaryPointDiagram;

public:
    Private();
    ~Private() override
    {
    }

    Private(const Private &rhs)
        : AbstractTernaryDiagram::Private(rhs)
    {
    }
};

KDCHART_IMPL_DERIVED_DIAGRAM(TernaryPointDiagram, AbstractTernaryDiagram, TernaryCoordinatePlane)
/*
inline TernaryPointDiagram::TernaryPointDiagram( Private * p, TernaryCoordinatePlane* plane )
  : AbstractTernaryDiagram( p, plane ) { init(); }
inline TernaryPointDiagram::Private * TernaryPointDiagram::d_func()
{ return static_cast<Private*>( AbstractTernaryDiagram::d_func() ); }
inline const TernaryPointDiagram::Private * TernaryPointDiagram::d_func() const
{ return static_cast<const Private*>( AbstractTernaryDiagram::d_func() ); }
*/
}

#endif /* KDCHARTTERNARYPOINTDIAGRAM_P_H */
