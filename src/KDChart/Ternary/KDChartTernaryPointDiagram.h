/****************************************************************************
** Copyright (C) 2001-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDCHARTTERNARYPOINTDIAGRAM_H
#define KDCHARTTERNARYPOINTDIAGRAM_H

#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"

namespace KDChart
{
/**
 * @brief A TernaryPointDiagram is a point diagram within a ternary coordinate
 * plane
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
    [[nodiscard]] const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
};

}

#endif
