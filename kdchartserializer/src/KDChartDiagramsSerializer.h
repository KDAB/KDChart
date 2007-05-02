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
#ifndef __KDCHARTDIAGRAMSSERIALIZER_H__
#define __KDCHARTDIAGRAMSSERIALIZER_H__

/** \file KDChartDiagramsSerializer.h
    \brief Auxiliary methods for reading/saving KD Chart data and configuration in streams.
  */

#include "KDChartAbstractSerializer.h"

#include "KDChartAttributesSerializer.h"

#include <KDChartChart>
#include <KDChartLineDiagram>
#include <KDChartBarDiagram>
#include <KDChartPieDiagram>
#include <KDChartPolarDiagram>
#include <KDChartRingDiagram>

namespace KDChart {

    class CoordPlanesSerializer;
    class AxesSerializer;
    class AttributesModelSerializer;

    class KDCHART_EXPORT DiagramsSerializer : public QObject, public AbstractSerializer
    {
        Q_OBJECT
        Q_DISABLE_COPY( DiagramsSerializer )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( DiagramsSerializer )

    public:
        explicit DiagramsSerializer( CoordPlanesSerializer* coordS = 0 );
        virtual ~DiagramsSerializer();

        virtual void saveDiagrams(
                QDomDocument& doc,
                QDomElement& e,
                const ConstAbstractDiagramList& diags,
                const QString& title )const;
        
        virtual void saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const;

        /**
         * Parse the diagram element, and return an AbstractDiagram* in \c diagramPtr
         * if the respective diagram was found in the list of global elements.
         *
         * Make sure that you have called
         * \c KDChart::SerializeCollector::instance()->initializeParsedGlobalPointers()
         * before invoking this method, or it will stop parsing and return false.
         */
        virtual bool parseElement( const QDomElement& container, QObject*& ptr ) const;

        virtual bool parseDiagram(
                const QDomNode& rootNode,
                const QDomNode& pointerNode,
                AbstractDiagram*& diagramPtr )const;
        virtual bool doParseDiagram(
                const QDomElement& container,
                AbstractDiagram*& diagramPtr )const;
        virtual void saveDiagram(
                QDomDocument& doc,
                QDomElement& e,
                const AbstractDiagram* diagram )const;

        virtual bool parseAbstractDiagram(
                const QDomElement& container, AbstractDiagram& diagram )const;
        virtual void saveAbstractDiagram(
                QDomDocument& doc,
                QDomElement& e,
                const AbstractDiagram& diagram,
                const QString& title )const;

        virtual bool parseCartCoordDiagram(
                const QDomElement& container, AbstractCartesianDiagram& diagram )const;
        virtual void saveCartCoordDiagram(
                QDomDocument& doc,
                QDomElement& e,
                const AbstractCartesianDiagram& diagram,
                const QString& title )const;

        virtual bool parsePolCoordDiagram(
                const QDomElement& container, AbstractPolarDiagram& diagram )const;
        virtual void savePolCoordDiagram(
                QDomDocument& doc,
                QDomElement& e,
                const AbstractPolarDiagram& diagram,
                const QString& title )const;

        virtual bool parseAbstractPieDiagram(
                const QDomElement& container, AbstractPieDiagram& diagram )const;
        virtual void saveAbstractPieDiagram(
                QDomDocument& doc,
                QDomElement& e,
                const AbstractPieDiagram& diagram,
                const QString& title )const;

        virtual bool parseLineDiagram(
                const QDomElement& container, LineDiagram& diagram )const;
        virtual void saveLineDiagram(
                QDomDocument& doc,
                QDomElement& diagElement,
                const LineDiagram& diagram )const;

        virtual bool parseBarDiagram(
                const QDomElement& container, BarDiagram& diagram )const;
        virtual void saveBarDiagram(
                QDomDocument& doc,
                QDomElement& diagElement,
                const BarDiagram& diagram )const;

        virtual bool parsePieDiagram(
                const QDomElement& container, PieDiagram& diagram )const;
        virtual void savePieDiagram(
                QDomDocument& doc,
                QDomElement& diagElement,
                const PieDiagram& diagram )const;

        virtual bool parsePolarDiagram(
                const QDomElement& container, PolarDiagram& diagram )const;
        virtual void savePolarDiagram(
                QDomDocument& doc,
                QDomElement& diagElement,
                const PolarDiagram& diagram )const;

        virtual bool parseRingDiagram(
                const QDomElement& container, RingDiagram& diagram )const;
        virtual void saveRingDiagram(
                QDomDocument& doc,
                QDomElement& diagElement,
                const RingDiagram& diagram )const;

        /**
         * Returns the correct class name for a given class.
         *
         * \note Make sure to overwrite this, if you intend to use the
         * saveOtherDiagram routine, or the classname will be set to
         * "UNKNOWN" for your own classes by default.
         *
         * When overwriting this method, you should first call the original method
         * and set your own name only when needed.
         */
        virtual const QString nameOfClass( const AbstractDiagram* p )const;

        virtual bool parseOtherDiagram(
                const QDomElement& container, AbstractDiagram& diagram )const;
        // By default this just saves the information stored by the
        // base class AbstractDiagram, it can be used by derived classes,
        // will be called whenever a diagram is found that is not one
        // of KD Chart's built-in diagram classes.
        // When using this option, just make sure to also call the
        // saveAbstractDiagram() method from within your own method,
        // so the base class's information will be stored too.
        virtual void saveOtherDiagram(
                QDomDocument& doc,
                QDomElement& diagElement,
                const AbstractDiagram& diagram )const;
    };

} // end of namespace

#endif
