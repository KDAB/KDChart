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
