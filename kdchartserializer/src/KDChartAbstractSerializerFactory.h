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
