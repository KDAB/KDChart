/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef __KDCHARTLEGENDSSERIALIZER_H__
#define __KDCHARTLEGENDSSERIALIZER_H__

/** \file KDChartLegendsSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include <KDChartAttributesSerializer>

#include <KDChartAbstractSerializer>

#include <KDChartLegend>
#include <KDChartChart>

namespace KDChart {

    class KDCHARTSERIALIZER_EXPORT LegendsSerializer : public QObject, public AbstractSerializer
    {
        Q_OBJECT
        Q_DISABLE_COPY( LegendsSerializer )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( LegendsSerializer )

        friend class Serializer;
        friend class DiagramsSerializer;

    public:
        LegendsSerializer( QObject* parent = 0 );
        virtual ~LegendsSerializer();

        /** reimp */
        void saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const;

        /**
         * reimp
         * 
         * Parse the legend element, and store it in \c ptr.
         *
         * Make sure that you have called
         * \c KDChart::SerializeCollector::instance()->initializeParsedGlobalPointers()
         * before invoking this method, or it will stop parsing and return false.
         */
        bool parseElement( const QDomElement& container, QObject* ptr ) const;

        static void saveLegends( QDomDocument& doc, QDomElement& e,
                                 const LegendList& planes, const QString& title );

        /**
         * Parse the legend element, and return a Legend* in \c legend
         * if the respective legend was found in the list of global elements.
         *
         * This method is called transparently by the Serializer, so you should
         * not need to call it explicitly.
         *
         * In case still want to call it just make sure that you have called
         * \c KDChart::SerializeCollector::instance()->initializeParsedGlobalPointers()
         * \em before invoking this method, or it will stop parsing and return false.
        */
        static bool parseLegend( const QDomNode& rootNode, const QDomNode& pointerNode,
                                 Legend*& legend );
    };

} // end of namespace

#endif
