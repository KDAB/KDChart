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
#ifndef __KDCHARTSERIALIZER_H__
#define __KDCHARTSERIALIZER_H__

/** \file KDChartSerializer.h
    \brief Library for reading/saving KD Chart data and configuration in streams.
  */

#include <KDChartChart>
#include <KDChartFrameAttributes>
#include <KDChartBackgroundAttributes>

#include "kdchartserializer_export.h"

#include <QDomDocument>

class QAbstractItemModel;

namespace KDChart {

    class CoordPlanesSerializer;
    class AbstractSerializerFactory;

    class KDCHARTSERIALIZER_EXPORT Serializer : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY( Serializer )

        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( Serializer );

    public:
        explicit Serializer( Chart* chart = 0,
                             QAbstractItemModel* model = 0 );
        /**
         * Free the allocated memory, ignoring the singletons
         * SerializeIdMapper and SerializeCollector. This enables you
         * to use their content after the Serializer is deleted.
         */
        virtual ~Serializer();

        /**
         * Make sure to call setChart() before calling write()
         * if you did not specify a chart with the constructor.
         */
        void setChart( Chart* chart );

        /**
         * \brief Set the data model to be assigned to diagrams created by read().
         *
         * This will affect new diagrams instantiated after setModel was
         * called, but it will not change any data model assignments that
         * were made before setModel was called.
         *
         * \note If you do not call setModel nor specify a model with the
         * Serializer's constructor, then you need to manually call the
         * setModel methods of all of the diagrams after read() has
         * finished. File \c examples/complexLayout/mainwindow.cpp
         * illustrates a way how to do that.
         */
        void setModel(QAbstractItemModel * model);

        /**
         * After read() has run successfully the created chart can be
         * retrieved via chart().
         */
        Chart* chart() const;

        virtual bool read(QIODevice *device);
        virtual bool write(QIODevice *device) const;

        virtual bool parseRootElement(
                const QDomElement& root );
        virtual bool saveRootElement(
                QDomDocument& doc,
                QDomElement& docRoot ) const;

        virtual bool parseChartElement(
                const QDomNode& rootNode,
                const QDomNode& pointerNode,
                Chart*& chartPtr ) const;
        virtual bool saveChartElement(
                QDomDocument& doc,
                QDomElement& e ) const;

        static void registerBuiltInSerializerFactories( QObject* parent = 0 );

        template< class T >
        static void registerElementSerializerFactory( AbstractSerializerFactory* factory )
        {
            registerElementSerializerFactory( T::staticMetaObject.className(), factory );
        }
        template< class T >
        static void unregisterElementSerializerFactory()
        {
            unregisterElementSerializerFactory( T::staticMetaObject.className() );
        }
        template< class T >
        static AbstractSerializerFactory* elementSerializerFactory()
        {
            return elementSerializerFactory( T::staticMetaObject.className() );
        }

        static AbstractSerializerFactory* elementSerializerFactory( const QObject* element );
        static AbstractSerializerFactory* elementSerializerFactory( const QString& className );

    protected:
        static void registerElementSerializerFactory( const char* name, AbstractSerializerFactory* factory );
        static void unregisterElementSerializerFactory( const char* name );
        static AbstractSerializerFactory* elementSerializerFactory( const char* className );
    };

} // end of namespace

#include <KDChartSerializerRegistrator>

#endif
