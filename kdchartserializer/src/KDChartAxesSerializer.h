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
#ifndef __KDCHARTAXESSERIALIZER_H__
#define __KDCHARTAXESSERIALIZER_H__

/** \file KDChartAxesSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include "KDChartAttributesSerializer.h"

#include <KDChartCartesianAxis>
//TODO once PolarAxis is implemented: #include <KDChartPolarAxis>
#include <KDChartChart>

namespace KDChart {

    //class DiagramsSerializer;

    class KDCHART_EXPORT AxesSerializer : public QObject
    {
        Q_OBJECT

        public:
            explicit AxesSerializer();
            virtual ~AxesSerializer();

            virtual bool parseCartesianAxes(
                    const QDomElement& e,
                    CartesianAxisList& axes,
                    const QDomElement* styleList=0 )const;

            //TODO once PolarAxis is implemented:
            /*
            virtual bool parsePolarAxes(
                    const QDomElement& e,
                    PolarAxisList& axes,
                    const QDomElement* styleList=0 )const;
            */
            virtual void saveCartesianAxes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const CartesianAxisList& axes,
                    const QString& title,
                    const QDomElement* styleList=0 )const;

            //TODO once PolarAxis is implemented:
            /*
            virtual void savePolarAxes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const PolarAxisList& planes,
                    const QString& title,
                    const QDomElement* styleList=0 )const;
            */
            virtual void saveAbstractAxis(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractAxis& axis,
                    const QString& title,
                    const QDomElement* styleList=0 )const;

            virtual void saveCartAxis(
                    QDomDocument& doc,
                    QDomElement& e,
                    const CartesianAxis& axis,
                    const QString& title,
                    const QDomElement* styleList=0 )const;

            //TODO once PolarAxis is implemented:
            /*
            virtual void savePolAxis(
                    QDomDocument& doc,
                    QDomElement& e,
                    const PolarAxis& axis,
                    const QString& title,
                    const QDomElement* styleList=0 )const;
            */
    };

} // end of namespace

#endif
