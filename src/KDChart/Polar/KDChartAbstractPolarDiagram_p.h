/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTPOLARDIAGRAM_P_H
#define KDCHARTABSTRACTPOLARDIAGRAM_P_H

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

#include "KDChartAbstractDiagram_p.h"
#include <KDChartGridAttributes.h>

#include <KDABLibFakes>

namespace KDChart {

class PolarCoordinatePlane;

/**
 * \internal
 */
class AbstractPolarDiagram::Private : public AbstractDiagram::Private
{
    friend class AbstractPolarDiagram;

public:
    Private();
    ~Private() override;

    Private(const Private &rhs)
        : AbstractDiagram::Private(rhs)
        , granularity(0)
    {
        // just for consistency
    }

    /** \reimp */
    // FIXME: Optimize when needed
    qreal calcPercentValue(const QModelIndex &index) const override
    {
        Q_ASSERT(index.isValid());
        qreal sum = 0.0;
        for (int row = 0; row < attributesModel->rowCount(QModelIndex()); row++)
            sum += attributesModel->data(attributesModel->index(row, index.column(), QModelIndex())).toReal(); // checked
        if (sum == 0.0)
            return 0.0;
        return attributesModel->data(attributesModel->mapFromSource(index)).toReal() / sum * 100.0;
    }

private:
    qreal granularity;
};

KDCHART_IMPL_DERIVED_DIAGRAM(AbstractPolarDiagram, AbstractDiagram, PolarCoordinatePlane)
/*
inline AbstractPolarDiagram::AbstractPolarDiagram( Private * p )
    : AbstractDiagram( p ) { init(); }
inline AbstractPolarDiagram::AbstractPolarDiagram(
    Private *p, QWidget* parent, PolarCoordinatePlane* plane )
    : AbstractDiagram( p, parent, plane ) { init(); }
inline AbstractPolarDiagram::Private * AbstractPolarDiagram::d_func()
{ return static_cast<Private*>( AbstractDiagram::d_func() ); }
inline const AbstractPolarDiagram::Private * AbstractPolarDiagram::d_func() const
{ return static_cast<const Private*>( AbstractDiagram::d_func() ); }
*/
}
#endif /* KDCHARTABSTRACTCARTESIANDIAGRAM_P_H */
