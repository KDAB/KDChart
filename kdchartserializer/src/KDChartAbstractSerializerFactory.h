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

#ifndef __KDCHARTABSTRACTSERIALIZERFACTORY_H__
#define __KDCHARTABSTRACTSERIALIZERFACTORY_H__

/** \file KDChartAbstractSerializerFactory.h
  */

#include <KDChartGlobal>
#include "kdchartserializer_export.h"

#include <QObject>

namespace KDChart {

    class AbstractSerializer;

    class KDCHARTSERIALIZER_EXPORT AbstractSerializerFactory : public QObject
    {
        Q_DISABLE_COPY( AbstractSerializerFactory )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( AbstractSerializerFactory )

    public:
        AbstractSerializerFactory( QObject* parent = 0 );
        virtual ~AbstractSerializerFactory();

        virtual AbstractSerializer* instance( const QString& className ) const = 0;
        virtual QObject* createNewObject( const QString& className ) const = 0;
    };

} // end of namespace

#endif
