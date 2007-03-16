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

#include <KDChartAbstractCartesianDiagram>
#include <KDChartAbstractPolarDiagram>
#include <KDChartChart>

namespace KDChart {

    class AxesSerializer;
    class AttributesModelSerializer;

    class KDCHART_EXPORT DiagramsSerializer : public QObject
    {
        Q_OBJECT

        public:
            explicit DiagramsSerializer();
            virtual ~DiagramsSerializer();

            virtual bool parseDiagrams(
                    const QDomElement& e,
                    AbstractDiagramList& diags )const;

            virtual void saveDiagrams(
                    QDomDocument& doc,
                    QDomElement& e,
                    const ConstAbstractDiagramList& diags,
                    const QString& title )const;

            virtual void saveAbstractDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractDiagram& diagram,
                    const QString& title )const;

            virtual void saveCartDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractCartesianDiagram& diagram,
                    const QString& title )const;

            virtual void savePolDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractPolarDiagram& diagram,
                    const QString& title )const;

            // By default this does nothing, it can be used by derived classes,
            // will be called whenever a diagram is found that is not one
            // of KD Chart's built-in diagram classes.
            virtual void saveOtherDiagram(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractDiagram& diagram )const;

        private:
            AxesSerializer* mAxesS;
            AttributesModelSerializer* mAttrModelS;
    };

} // end of namespace

#endif
