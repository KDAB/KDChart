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

#ifndef __KDCHARTABSTRACTSERIALIZER_H__
#define __KDCHARTABSTRACTSERIALIZER_H__

/** \file KDChartAbstractSerializer.h
  */

#include <KDChartGlobal>

#include "kdchartserializer_export.h"

class QDomDocument;
class QDomElement;
class QDomNode;
class QObject;

namespace KDChart {

    class KDCHARTSERIALIZER_EXPORT AbstractSerializer
    {
    public:
        virtual ~AbstractSerializer() {}

        virtual void saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const = 0;
        virtual bool parseElement( const QDomElement& container, QObject* ptr ) const = 0;
    };

} // end of namespace

#endif
