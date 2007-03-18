/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#ifndef __KDCHARTCOORPLANESSERIALIZER_H__
#define __KDCHARTCOORPLANESSERIALIZER_H__

/** \file KDChartCoordPlanesSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include "KDChartAttributesSerializer.h"

#include <KDChartCartesianCoordinatePlane>
#include <KDChartPolarCoordinatePlane>
#include <KDChartChart>

namespace KDChart {

    class DiagramsSerializer;

    class KDCHART_EXPORT CoordPlanesSerializer : public QObject
    {
        Q_OBJECT

        public:
            explicit CoordPlanesSerializer();
            virtual ~CoordPlanesSerializer();

            virtual bool parsePlanes(
                    const QDomElement& e,
                    CoordinatePlaneList& planes )const;

            virtual void savePlanes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const CoordinatePlaneList& planes,
                    const QString& title )const;

            virtual void savePlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCoordinatePlane* p )const;

            virtual void saveAbstractPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCoordinatePlane& plane,
                    const QString& title )const;

            virtual void saveCartPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const CartesianCoordinatePlane& plane,
                    const QString& title )const;

            virtual void savePolPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const PolarCoordinatePlane& plane,
                    const QString& title )const;

            // By default this does nothing, it can be used by derived classes,
            // will be called whenever a coord. plane is found that is neither
            // a CartesianCoordinatePlane nor a PolarCoordinatePlane.
            virtual void saveOtherPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCoordinatePlane& plane )const;

            const QString globalList()const;

        private:
            DiagramsSerializer* mDiagS;
    };

} // end of namespace

#endif
