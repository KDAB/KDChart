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

#ifndef __KDCHARTATTRIBUTESMODELSERIALIZER_H__
#define __KDCHARTATTRIBUTESMODELSERIALIZER_H__

/** \file KDChartAttributesModelSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include "KDChartAttributesSerializer.h"

#include <KDChartAttributesModel>
#include <KDChartChart>

namespace KDChart {

    class KDCHARTSERIALIZER_EXPORT AttributesModelSerializer : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY( AttributesModelSerializer )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( AttributesModelSerializer )

    public:
        AttributesModelSerializer();
        virtual ~AttributesModelSerializer();

        virtual bool parseAttributesModel(
                const QDomNode& rootNode,
                const QString& globalName,
                AttributesModel& model )const;

        virtual bool parseAttributesNode(
                const QDomElement& e,
                QVariant& attributes,
                int& role )const;

        virtual void saveAttributesModel(
                QDomDocument& doc,
                QDomElement& e,
                const AttributesModel* model,
                bool isExternalModel )const;
        virtual void createAttributesNode(
                QDomDocument& doc,
                QDomElement& e,
                const AttributesModel* model,
                int role,
                const QVariant& attributes )const;
    };

} // end of namespace

#endif
