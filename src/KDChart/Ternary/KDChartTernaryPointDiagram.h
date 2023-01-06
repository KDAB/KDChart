/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTTERNARYPOINTDIAGRAM_H
#define KDCHARTTERNARYPOINTDIAGRAM_H

#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"

namespace KDChart {

/**
 * @brief A TernaryPointDiagram is a point diagram within a ternary coordinate plane
 */
class KDCHART_EXPORT TernaryPointDiagram : public AbstractTernaryDiagram
{
    Q_OBJECT
    Q_DISABLE_COPY(TernaryPointDiagram)
    KDCHART_DECLARE_DERIVED_DIAGRAM(TernaryPointDiagram, TernaryCoordinatePlane)

public:
    explicit TernaryPointDiagram(QWidget *parent = nullptr, TernaryCoordinatePlane *plane = nullptr);
    ~TernaryPointDiagram() override;

    void resize(const QSizeF &area) override;
    void paint(PaintContext *paintContext) override;

protected:
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
};
}

#endif
