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
#ifndef __KDCHARTDIAGRAMSSERIALIZER_H__
#define __KDCHARTDIAGRAMSSERIALIZER_H__

/** \file KDChartDiagramsSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include "KDChartAttributesSerializer.h"

#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartBarDiagram>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>

namespace KDChart {

    class CoordPlanesSerializer;
    class AxesSerializer;
    class AttributesModelSerializer;

    class KDCHART_EXPORT DiagramsSerializer : public QObject
    {
        Q_OBJECT

        public:
            explicit DiagramsSerializer( CoordPlanesSerializer* coordS );
            virtual ~DiagramsSerializer();

            virtual bool parseDiagrams(
                    const QDomElement& e,
                    AbstractDiagramList& diags )const;

            virtual void saveDiagrams(
                    QDomDocument& doc,
                    QDomElement& e,
                    const ConstAbstractDiagramList& diags,
                    const QString& title )const;

            virtual void saveDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractDiagram* diagram )const;

            virtual void saveAbstractDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractDiagram& diagram,
                    const QString& title )const;

            virtual void saveCartCoordDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCartesianDiagram& diagram,
                    const QString& title )const;

            virtual void savePolCoordDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractPolarDiagram& diagram,
                    const QString& title )const;

            virtual void saveAbstractPieDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractPieDiagram& diagram,
                    const QString& title )const;

            virtual void saveLineDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const LineDiagram& diagram,
                    const QString& title )const;

            virtual void saveBarDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const BarDiagram& diagram,
                    const QString& title )const;

            virtual void savePieDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const PieDiagram& diagram,
                    const QString& title )const;

            virtual void savePolarDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const PolarDiagram& diagram,
                    const QString& title )const;

            virtual void saveRingDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const RingDiagram& diagram,
                    const QString& title )const;

            // By default this just saves the information stored by the
            // base class AbstractDiagram, it can be used by derived classes,
            // will be called whenever a diagram is found that is not one
            // of KD Chart's built-in diagram classes.
            // When using this option, just make sure to also call the
            // saveAbstractDiagram() method from within your own method,
            // so the base class's information will be stored too.
            virtual void saveOtherDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractDiagram& diagram )const;

        private:
            CoordPlanesSerializer* mCoordS;
            AxesSerializer* mAxesS;
            AttributesModelSerializer* mAttrModelS;
        mutable QString mGlobalList;
    };

} // end of namespace

#endif
