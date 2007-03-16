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
#ifndef __KDCHARTSERIALIZER_H__
#define __KDCHARTSERIALIZER_H__

/** \file KDChartSerializer.h
    \brief Library for reading/saving KD Chart data and configuration in streams.
  */

#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartBackgroundAttributes>

#include <QDomDocument>

namespace KDChart {

    class AttributesSerializer;
    class CoordPlanesSerializer;

    class KDCHART_EXPORT Serializer : public QObject
    {
        Q_OBJECT

        public:
            explicit Serializer( Chart* chart = 0 );
            /**
             * Free the allocated memory, ignoring the singletons
             * SerializeIdMapper and SerializeCollector. This enables you
             * to use their content after the Serializer is deleted.
             */
            virtual ~Serializer();

            void setChart( Chart* chart ){ mChart = chart; }
            Chart* chart() const { return mChart; }

            virtual bool read(QIODevice *device);
            virtual bool write(QIODevice *device) const;

            virtual bool parseChartElement(
                    const QDomElement& root ) const;
            virtual bool saveRootElement(
                    QDomDocument& doc,
                    QDomElement& docRoot ) const;

            virtual bool saveChartElement(
                    QDomDocument& doc,
                    QDomElement& e ) const;

        private:
            Chart* mChart;
            AttributesSerializer* mAttrS;
            CoordPlanesSerializer* mCoordS;
    };

} // end of namespace

#endif
