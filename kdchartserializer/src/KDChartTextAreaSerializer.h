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

#ifndef __KDCHARTTEXTAREASERIALIZER_H__
#define __KDCHARTTEXTAREASERIALIZER_H__

/** \file KDChartTextAreaSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include <KDChartHeaderFooter>
#include <KDChartChart>

#include <KDChartAbstractSerializer>

#include <QDomDocument>

namespace KDChart {

    class KDCHARTSERIALIZER_EXPORT TextAreaSerializer : public QObject, public AbstractSerializer
    {
        Q_OBJECT
        Q_DISABLE_COPY( TextAreaSerializer )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( TextAreaSerializer )

    public:
        TextAreaSerializer();
        virtual ~TextAreaSerializer();

        static void saveHeadersFooters(
                QDomDocument& doc,
                QDomElement& e,
                const KDChart::HeaderFooterList& areas,
                const QString& title );

        /** reimp */
        void saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const;
        /** reimp */
        bool parseElement( const QDomElement& container, QObject* ptr ) const;

       /**
         * Parse the header/footer element, and return a HeaderFooter* in \c hdFt
         * if the respective header/footer was found in the list of global elements.
         *
         * This method is called transparently by the Serializer, so you should
         * not need to call it explicitly.
         *
         * In case still want to call it just make sure that you have called
         * \c KDChart::SerializeCollector::instance()->initializeParsedGlobalPointers()
         * \em before invoking this method, or it will stop parsing and return false.
        */
        static bool parseHeaderFooter(
                const QDomNode& rootNode,
                const QDomNode& pointerNode,
                HeaderFooter*& hdFt );

protected:
    };

} // end of namespace

#endif
