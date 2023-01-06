/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTLINEDIAGRAM_P_H
#define KDCHARTLINEDIAGRAM_P_H

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

#include "KDChartLineDiagram.h"

#include <QPainterPath>

#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartCartesianDiagramDataCompressor_p.h"
#include "KDChartThreeDLineAttributes.h"

#include <KDABLibFakes>

namespace KDChart {

class PaintContext;

/**
 * \internal
 */
class LineDiagram::Private : public AbstractCartesianDiagram::Private
{
    friend class LineDiagram;
    friend class LineDiagramType;

public:
    Private();
    Private(const Private &rhs);
    ~Private() override;

    LineDiagramType *implementor; // the current type
    LineDiagramType *normalDiagram;
    LineDiagramType *stackedDiagram;
    LineDiagramType *percentDiagram;
    bool centerDataPoints;
    bool reverseDatasetOrder;
    qreal tension = 0.0;
};

KDCHART_IMPL_DERIVED_DIAGRAM(LineDiagram, AbstractCartesianDiagram, CartesianCoordinatePlane)

class LineDiagram::LineDiagramType
{
public:
    explicit LineDiagramType(LineDiagram *d)
        : m_private(d->d_func())
    {
    }
    virtual ~LineDiagramType()
    {
    }
    virtual LineDiagram::LineType type() const = 0;
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const = 0;
    virtual void paint(PaintContext *ctx) = 0;
    LineDiagram *diagram() const;

protected:
    // make some elements of m_private available to derived classes:
    AttributesModel *attributesModel() const;
    QModelIndex attributesModelRootIndex() const;
    ReverseMapper &reverseMapper();
    CartesianDiagramDataCompressor &compressor() const;

    qreal interpolateMissingValue(const CartesianDiagramDataCompressor::CachePosition &pos) const;

    int datasetDimension() const;

    qreal valueForCell(int row, int column) const;

    LineDiagram::Private *m_private;
};
}

#endif /* KDCHARTLINEDIAGRAM_P_H */
