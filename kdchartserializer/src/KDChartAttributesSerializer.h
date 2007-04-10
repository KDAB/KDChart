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
#ifndef __KDCHARTATTRSSERIALIZER_H__
#define __KDCHARTATTRSSERIALIZER_H__

/** \file KDChartAttributesSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include <KDChartTextAttributes>
#include <KDChartFrameAttributes>
#include <KDChartBackgroundAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartBarAttributes>
#include <KDChartLineAttributes>
#include <KDChartPieAttributes>
#include <KDChartAbstractThreeDAttributes>
#include <KDChartThreeDBarAttributes>
#include <KDChartThreeDLineAttributes>
#include <KDChartThreeDPieAttributes>
#include <KDChartGridAttributes>

#include <QDomDocument>

namespace KDChart {

    class KDCHART_EXPORT AttributesSerializer
    {
        public:
            static bool parseLeading(
                    const QDomElement& container, int& left, int& top, int& right, int& bottom );
            static void saveLeading(
                    QDomDocument& doc,
                    QDomElement& e,
                    int left, int top, int right, int bottom,
                    const QString& title );

            static bool parseTextAttributes(
                    const QDomElement& container, TextAttributes& a );
            static void saveTextAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const TextAttributes& a,
                    const QString& title );

            static bool parseMeasure(
                    const QDomElement& container, Measure& a );
            static void saveMeasure(
                    QDomDocument& doc,
                    QDomElement& e,
                    const Measure& m,
                    const QString& title );

            static bool parseFrameAttributes(
                    const QDomElement& container, FrameAttributes& a );
            static void saveFrameAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const FrameAttributes& a,
                    const QString& title );

            static bool parseBackgroundAttributes(
                    const QDomElement& container, BackgroundAttributes& a );
            static void saveBackgroundAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const BackgroundAttributes& a,
                    const QString& title );

            static bool parseDataValueAttributes(
                    const QDomElement& container, DataValueAttributes& a );
            static void saveDataValueAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const DataValueAttributes& a,
                    const QString& title );

            static bool parseLineAttributes(
                    const QDomElement& container, LineAttributes& a );
            static void saveLineAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const LineAttributes& a,
                    const QString& title );

            static bool parseBarAttributes(
                    const QDomElement& container, BarAttributes& a );
            static void saveBarAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const BarAttributes& a,
                    const QString& title );

            static bool parsePieAttributes(
                    const QDomElement& container, PieAttributes& a );
            static void savePieAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const PieAttributes& a,
                    const QString& title );

            static bool parseAbstractThreeDAttributes(
                    const QDomElement& container, AbstractThreeDAttributes& a );
            static void saveAbstractThreeDAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const AbstractThreeDAttributes* a,
                    const QString& title );

            static bool parseThreeDBarAttributes(
                    const QDomElement& container, ThreeDBarAttributes& a );
            static void saveThreeDBarAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const ThreeDBarAttributes& a,
                    const QString& title );

            static bool parseThreeDLineAttributes(
                    const QDomElement& container, ThreeDLineAttributes& a );
            static void saveThreeDLineAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const ThreeDLineAttributes& a,
                    const QString& title );

            static bool parseThreeDPieAttributes(
                    const QDomElement& container, ThreeDPieAttributes& a );
            static void saveThreeDPieAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const ThreeDPieAttributes& a,
                    const QString& title );

            static bool parseMarkerAttributes(
                    const QDomElement& container, MarkerAttributes& a );
            static void saveMarkerAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const MarkerAttributes& a,
                    const QString& title );
            static MarkerAttributes::MarkerStyle markerStyleFromName(
                    QString name );
            static QString markerStyleToName(
                    MarkerAttributes::MarkerStyle style );

            static bool parseRelativePosition(
                    const QDomElement& container, RelativePosition& a );
            static void saveRelativePosition(
                    QDomDocument& doc,
                    QDomElement& e,
                    const RelativePosition& a,
                    const QString& title );

            static bool parseGridAttributes(
                    const QDomElement& container, GridAttributes& a );
            static void saveGridAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const GridAttributes& a,
                    const QString& title );

            /**
             * This method assumes you have called
             * SerializeCollector::initializeParsedGlobalPointers()
             * before.
             * Return value is TRUE if the parsed pointer
             * was found in the internal list of known objects.
             * Otherwise it will return FALSE.
             */
            static bool parseQObjectPointerNode(
                    const QDomNode& node,
                    QObject*& p );

            static void saveQObjectPointer(
                    QDomDocument& doc,
                    QDomElement& e,
                    const QObject* p,
                    const QString& title );

            static const QString showDomPath( const QDomElement& container );
    };

} // end of namespace

#endif
