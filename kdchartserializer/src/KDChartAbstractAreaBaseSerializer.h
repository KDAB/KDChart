/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef __KDCHARTABSTRACTAREABASESERIALIZER_H__
#define __KDCHARTABSTRACTAREABASESERIALIZER_H__

/** \file KDChartAbstractAreaBaseSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include <KDChartAbstractAreaBase.h>
#include <QDomDocument>

#include "kdchartserializer_export.h"

namespace KDChart {

    class KDCHARTSERIALIZER_EXPORT AbstractAreaBaseSerializer
    {
        Q_DISABLE_COPY( AbstractAreaBaseSerializer )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( AbstractAreaBaseSerializer )
    public:
        AbstractAreaBaseSerializer();
        virtual ~AbstractAreaBaseSerializer();

        static bool parseAbstractAreaBase(
                const QDomElement& e, AbstractAreaBase& area );
        static void saveAbstractAreaBase(
                QDomDocument& doc,
                QDomElement& e,
                const AbstractAreaBase& area,
                const QString& title );
    };

} // end of namespace

#endif
