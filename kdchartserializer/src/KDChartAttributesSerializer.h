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

#include <QDomDocument>

namespace KDChart {

    class KDCHART_EXPORT AttributesSerializer
    {
        public:
            static bool parseLeading(
                    const QDomElement& e, int& left, int& top, int& right, int& bottom );
            static void saveLeading(
                    QDomDocument& doc,
                    QDomElement& e,
                    int left, int top, int right, int bottom,
                    const QString& title );

            static bool parseTextAttributes(
                    const QDomElement& e, TextAttributes& a );
            static void saveTextAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const TextAttributes& a,
                    const QString& title );

            static bool parseMeasure(
                    const QDomElement& e, Measure& a );
            static void saveMeasure(
                    QDomDocument& doc,
                    QDomElement& e,
                    const Measure& m,
                    const QString& title );
            static bool parseFrameAttributes(
                    const QDomElement& e, FrameAttributes& a );
            static void saveFrameAttributes(
                    QDomDocument& doc,
            QDomElement& e,
            const FrameAttributes& a,
            const QString& title );

            static bool parseBackgroundAttributes(
                    const QDomElement& e, BackgroundAttributes& a );
            static void saveBackgroundAttributes(
                    QDomDocument& doc,
                    QDomElement& e,
                    const BackgroundAttributes& a,
                    const QString& title );
    };

} // end of namespace

#endif
