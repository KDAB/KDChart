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
#ifndef __KDCHARTTEXTAREASERIALIZER_H__
#define __KDCHARTTEXTAREASERIALIZER_H__

/** \file KDChartTextAreaSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include <KDChartHeaderFooter>
#include <KDChartChart>

#include <QDomDocument>

namespace KDChart {

    class KDCHART_EXPORT TextAreaSerializer : public QObject
    {
        public:
            static bool parseHeadersFooters(
                    const QDomElement& container, KDChart::HeaderFooterList& areas );
            static void saveHeadersFooters(
                    QDomDocument& doc,
                    QDomElement& e,
                    const KDChart::HeaderFooterList& areas,
                    const QString& title );

            static bool parseHeaderFooter(
                    const QDomElement& container, HeaderFooter& hdFt );
            static void TextAreaSerializer::saveHeaderFooter(
                    QDomDocument& doc,
                    QDomElement& hdFtElement,
                    const KDChart::HeaderFooter& hdFt );

            static bool parseTextArea(
                    const QDomElement& container, TextArea& area );
            static void saveTextArea(
                    QDomDocument& doc,
                    QDomElement& e,
                    const TextArea& area,
                    const QString& title );

            static bool parseTextLayoutItem(
                    const QDomElement& container, TextLayoutItem& item );
            static void saveTextLayoutItem(
                    QDomDocument& doc,
                    QDomElement& e,
                    const TextLayoutItem& item,
                    const QString& title );
    };

} // end of namespace

#endif
