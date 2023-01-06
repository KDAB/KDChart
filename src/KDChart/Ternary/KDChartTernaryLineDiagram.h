/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTTERNARYLINEDIAGRAM_H
#define KDCHARTTERNARYLINEDIAGRAM_H

#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"

namespace KDChart {

/**
 * @brief A TernaryLineDiagram is a line diagram with a ternary coordinate plane
 */
class KDCHART_EXPORT TernaryLineDiagram : public AbstractTernaryDiagram
{
    Q_OBJECT
    Q_DISABLE_COPY(TernaryLineDiagram)
    KDCHART_DECLARE_DERIVED_DIAGRAM(TernaryLineDiagram, TernaryCoordinatePlane)

public:
    explicit TernaryLineDiagram(QWidget *parent = nullptr, TernaryCoordinatePlane *plane = nullptr);
    ~TernaryLineDiagram() override;

    void resize(const QSizeF &area) override;
    void paint(PaintContext *paintContext) override;

protected:
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
};
}

#endif
