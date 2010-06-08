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

#ifndef __KDCHARTDIAGRAMSSERIALIZER_H__
#define __KDCHARTDIAGRAMSSERIALIZER_H__

/** \file KDChartDiagramsSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include "KDChartAbstractSerializer"
#include "KDChartLegendsSerializer"

#include "KDChartAttributesSerializer"

#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartBarDiagram>
#include <KDChartStockDiagram>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>
#include <KDChartPlotter>

namespace KDChart {

    class CoordPlanesSerializer;
    class AxesSerializer;
    class AttributesModelSerializer;

    class KDCHARTSERIALIZER_EXPORT DiagramsSerializer : public QObject, public AbstractSerializer
    {
        Q_OBJECT
        Q_DISABLE_COPY( DiagramsSerializer )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( DiagramsSerializer )

        friend class Private;
        friend class CoordPlanesSerializer;
        friend class LegendsSerializer;
        friend class LegendsSerializer::Private;

    public:
        explicit DiagramsSerializer( CoordPlanesSerializer* coordS = 0 );
        virtual ~DiagramsSerializer();

        /** reimp */
        void saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const;

        /**
         * reimp
         * 
         * Parse the diagram element, and store it in \c ptr.
         *
         * Make sure that you have called
         * \c KDChart::SerializeCollector::instance()->initializeParsedGlobalPointers()
         * before invoking this method, or it will stop parsing and return false.
         */
        bool parseElement( const QDomElement& container, QObject* ptr ) const;

       /**
         * Parse the diagram element, and return a AbstractDiagram* in \c diagramPtr
         * if the respective diagram was found in the list of global elements.
         *
         * This method is called transparently by the Serializer, so you should
         * not need to call it explicitly.
         *
         * In case still want to call it just make sure that you have called
         * \c KDChart::SerializeCollector::instance()->initializeParsedGlobalPointers()
         * \em before invoking this method, or it will stop parsing and return false.
        */
        bool parseDiagram( const QDomNode& rootNode, const QDomNode& pointerNode,
                           AbstractDiagram*& diagramPtr ) const;
        void saveDiagrams( QDomDocument& doc, QDomElement& e,
                           const ConstAbstractDiagramList& diags, const QString& title ) const;
    };

} // end of namespace

#endif
