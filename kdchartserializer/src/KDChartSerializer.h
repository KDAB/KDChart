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

class QAbstractItemModel;

namespace KDChart {

    class CoordPlanesSerializer;

    class KDCHART_EXPORT Serializer : public QObject
    {
        Q_OBJECT

        public:
            explicit Serializer( Chart* chart = 0,
                                 QAbstractItemModel* model = 0 );
            /**
             * Free the allocated memory, ignoring the singletons
             * SerializeIdMapper and SerializeCollector. This enables you
             * to use their content after the Serializer is deleted.
             */
            virtual ~Serializer();

            /**
             * Make sure to call setChart() before calling write()
             * if you did not specify a chart with the constructor.
             */
            void setChart( Chart* chart ){ mChart = chart; }

            /**
             * If all of your chart's diagrams are using the same data model
             * you might want to call setModel() before calling read()
             * (or specify a model with the Serializer's constructor, resp.).
             *
             * If you do this then that model will be assigned to any diagrams
             * that will be created by the serializer.
             * Otherwise you need to call setModel() on each of the diagrams,
             * after read() has run.
             */
            void setModel(QAbstractItemModel * model);

            /**
             * After read() has run successfully the created chart can be
             * retrieved via chart()
             */
            Chart* chart() const { return mChart; }

            virtual bool read(QIODevice *device);
            virtual bool write(QIODevice *device) const;

            virtual bool parseRootElement(
                    const QDomElement& root );
            virtual bool saveRootElement(
                    QDomDocument& doc,
                    QDomElement& docRoot ) const;

            virtual bool parseChartElement(
                    const QDomNode& rootNode,
                    const QDomNode& pointerNode,
                    Chart*& chartPtr ) const;
            virtual bool saveChartElement(
                    QDomDocument& doc,
                    QDomElement& e ) const;

        private:
            QString mProgramName;
            Chart* mChart;
            CoordPlanesSerializer* mCoordS;
    };

} // end of namespace

#endif
