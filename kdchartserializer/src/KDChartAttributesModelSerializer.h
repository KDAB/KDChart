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
#ifndef __KDCHARTATTRIBUTESMODELSERIALIZER_H__
#define __KDCHARTATTRIBUTESMODELSERIALIZER_H__

/** \file KDChartAttributesModelSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include "KDChartAttributesSerializer.h"

#include <KDChartAttributesModel>
#include <KDChartChart>

namespace KDChart {

    class KDCHART_EXPORT AttributesModelSerializer : public QObject
    {
        Q_OBJECT

        public:
            explicit AttributesModelSerializer();
            virtual ~AttributesModelSerializer();

            virtual bool parseAttributesModel(
                    const QDomElement& e,
                    AttributesModel& model )const;

            virtual void saveAttributesModel(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AttributesModel* model )const;
            virtual void createAttributesNode(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AttributesModel* model,
                    int role,
                    const QVariant& attributes )const;
    };

} // end of namespace

#endif