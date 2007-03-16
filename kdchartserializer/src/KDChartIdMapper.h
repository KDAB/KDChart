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
#ifndef __KDCHART_ID_MAPPER_H__
#define __KDCHART_ID_MAPPER_H__

#include <QString>
#include <QMap>

namespace KDChart {

    class IdMapper
    {
        //friend class Serializer; // is allowed to delete the mapper instance

    public:
        static IdMapper* instance();

    protected:
        IdMapper();
        virtual ~IdMapper();

    public:
        /** Returns the name that was stored for this pointer.
         *
         * If none was stored before, a new name is created
         * ( basename + "_" + a current number ),
         * the entry is stored and the name is returned.
         */
        QString findOrMakeName( void* id, const QString& baseName );
        /** Returns the pointer that was stored for the given name.
         *
         * If none was stored, it returns zero.
         */
        void* findId( const QString& name )const;

    private:
        QMap<void*, QString> mMap;
        QString mCounterTag;
    };

} // end of namespace

#endif
