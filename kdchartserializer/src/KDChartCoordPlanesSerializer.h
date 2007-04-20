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

            virtual void savePlanes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const CoordinatePlaneList& planes,
                    const QString& title )const;

            /**
             * Parse the plane element, and return an AbstractCoordinatePlane* in \c planePtr
             * if the respective plane was found in the list of global elements.
             *
             * Make sure that you have called
             * \c KDChart::SerializeCollector::instance()->initializeParsedGlobalPointers()
             * before invoking this method, or it will stop parsing and return false.
             */
            virtual bool parsePlane(
                    const QDomNode& rootNode,
                    const QDomNode& pointerNode,
                    AbstractCoordinatePlane*& planePtr )const;
            virtual void savePlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCoordinatePlane* p )const;

            virtual bool parseAbstractPlane(
                    const QDomElement& container, AbstractCoordinatePlane& plane )const;
            virtual void saveAbstractPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCoordinatePlane& plane,
                    const QString& title )const;

            virtual bool parseCartPlane(
                    const QDomElement& container, CartesianCoordinatePlane& plane )const;
            virtual void saveCartPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const CartesianCoordinatePlane& plane,
                    const QString& title )const;

            virtual bool parsePolPlane(
                    const QDomElement& container, PolarCoordinatePlane& plane )const;
            virtual void savePolPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const PolarCoordinatePlane& plane,
                    const QString& title )const;

            /**
             * Returns the correct class name for a given class.
             *
             * \note Make sure to overwrite this, if you intend to use the
             * saveOtherPlane routine, or the classname will be set to
             * "UNKNOWN" for your own classes by default.
             *
             * When overwriting this method, you should first call the original method
             * and set your own name only when needed.
             */
            virtual const QString nameOfClass( const AbstractCoordinatePlane* p )const;

            virtual bool parseOtherPlane(
                    const QDomElement& container, AbstractCoordinatePlane& plane )const;
            /** By default this does nothing, it can be used by derived classes,
             * will be called whenever a coord. plane is found that is neither
             *  a CartesianCoordinatePlane nor a PolarCoordinatePlane.
             *
             * \note Make sure to implement nameOfClass if you want to use
             * saveOtherPlane, or the classname attribute will be stored as "UNKNOWN".
             */
            virtual void saveOtherPlane(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCoordinatePlane& plane )const;

            virtual bool parseAxesCalcMode(
                    const QDomElement& container,
                    AbstractCoordinatePlane::AxesCalcMode& mode )const;
            virtual void saveAxesCalcMode(
                    QDomDocument& doc,
                    QDomElement& e,
                    const CartesianCoordinatePlane::AxesCalcMode& mode,
                    const QString& title )const;

            const QString globalList()const;

        private:
            DiagramsSerializer* mDiagS;
    };

} // end of namespace

#endif
