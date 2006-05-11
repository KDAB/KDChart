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

#ifndef KDCHARTBARDIAGRAM_P_H
#define KDCHARTBARDIAGRAM_P_H

#include "KDChartAttributesModel.h"
#include "KDChartAbstractCartesianDiagram_p.h"

namespace KDChart {

    class ModelDataCache;

    class Bar2Diagram::Private : public AbstractCartesianDiagram::Private
    {
        friend class Bar2Diagram;
    public:
        Private();
        ~Private();

        void calculateValueAndGapWidths( int rowCount, int colCount,
                                         double groupWidth,
                                         double& barWidth,
                                         double& spaceBetweenBars,
                                         double& spaceBetweenGroups );

        BarType barType;
        ModelDataCache* modelDataCache;
    private:
        double maxDepth;

    };

    inline Bar2Diagram::Bar2Diagram( Private * p, CartesianCoordinatePlane* plane )
        : AbstractCartesianDiagram( p, plane ) { init(); }
    inline Bar2Diagram::Private * Bar2Diagram::d_func()
    { return static_cast<Private*>( AbstractCartesianDiagram::d_func() ); }
    inline const Bar2Diagram::Private * Bar2Diagram::d_func() const
    { return static_cast<const Private*>( AbstractCartesianDiagram::d_func() ); }

}

#endif /* KDCHARTBARDIAGRAM_P_H */
