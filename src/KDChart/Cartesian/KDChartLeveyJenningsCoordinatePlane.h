/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef KDCHARTLEVEYJENNINGSCOORDINATEPLANE_H
#define KDCHARTLEVEYJENNINGSCOORDINATEPLANE_H

#include "KDChartCartesianCoordinatePlane.h"

#include "KDChartLeveyJenningsGridAttributes.h"

namespace KDChart {

class LeveyJenningsGrid;

/**
 * @brief Levey Jennings coordinate plane
 * This is actually nothing real more than a plain cartesian
 * coordinate plane. The difference is, that only Levey Jennings
 * Diagrams can be added to it.
 */
class KDCHART_EXPORT LeveyJenningsCoordinatePlane
    : public CartesianCoordinatePlane
{
    Q_OBJECT
    Q_DISABLE_COPY(LeveyJenningsCoordinatePlane)
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT(LeveyJenningsCoordinatePlane, Chart *)

    friend class LeveyJenningsGrid;

public:
    explicit LeveyJenningsCoordinatePlane(Chart *parent = nullptr);
    ~LeveyJenningsCoordinatePlane() override;

    void addDiagram(AbstractDiagram *diagram) override;

    LeveyJenningsGridAttributes gridAttributes() const;
    void setGridAttributes(const LeveyJenningsGridAttributes &attr);

protected:
    const QPointF translateBack(const QPointF &screenPoint) const;

private:
    LeveyJenningsGrid *grid() const;
};
}

#endif
