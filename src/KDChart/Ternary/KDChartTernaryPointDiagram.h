/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#ifndef KDCHARTTERNARYPOINTDIAGRAM_H
#define KDCHARTTERNARYPOINTDIAGRAM_H

#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartAbstractTernaryDiagram.h"

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
