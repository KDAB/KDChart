/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTCARTESIANDIAGRAM_P_H
#define KDCHARTABSTRACTCARTESIANDIAGRAM_P_H

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

#include "KDChartAbstractCartesianDiagram.h"

#include <KDChartAbstractDiagram_p.h>
#include <KDChartAbstractThreeDAttributes.h>
#include <KDChartGridAttributes.h>

#include <KDABLibFakes>

namespace KDChart {

class CartesianCoordinatePlane;
class AbstractCartesianDiagram;

/**
 * \internal
 */
class AbstractCartesianDiagram::Private : public AbstractDiagram::Private
{
    friend class AbstractCartesianDiagram;

public:
    Private();
    ~Private() override;

    Private(const Private &rhs)
        : AbstractDiagram::Private(rhs)
        , axesList() // Do not copy axes and reference diagrams.
        , referenceDiagramOffset()
    {
    }

    /** \reimp */
    CartesianDiagramDataCompressor::AggregatedDataValueAttributes aggregatedAttrs(
        const QModelIndex &index,
        const CartesianDiagramDataCompressor::CachePosition *position) const override
    {
        if (position)
            return compressor.aggregatedAttrs(diagram, index, *position);
        CartesianDiagramDataCompressor::AggregatedDataValueAttributes allAttrs;
        allAttrs[index] = diagram->dataValueAttributes(index);
        return allAttrs;
    }

    CartesianAxisList axesList;

    AbstractCartesianDiagram *referenceDiagram = nullptr;
    QPointF referenceDiagramOffset;

    mutable CartesianDiagramDataCompressor compressor;
};

KDCHART_IMPL_DERIVED_DIAGRAM(AbstractCartesianDiagram, AbstractDiagram, CartesianCoordinatePlane)
}
#endif /* KDCHARTABSTRACTCARTESIANDIAGRAM_P_H */
