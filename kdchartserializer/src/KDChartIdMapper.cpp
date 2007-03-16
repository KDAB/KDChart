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

#include "KDChartIdMapper.h"

using namespace KDChart;

IdMapper::IdMapper()
    : mCounterTag( "::C_O_U_N_T_E_R" )
{
    // this space left empty intentionally
}

IdMapper::~IdMapper()
{
    clear();
}

void IdMapper::clear()
{
    // this space left empty intentionally
}

IdMapper* IdMapper::instance()
{
    static IdMapper instance;
    return &instance;
}

QString IdMapper::findOrMakeName(
        const void* id,
        const QString& baseName,
        bool& wasFound )
{
    wasFound = mMap.contains( id );
    if( wasFound )
        return mMap.value( id );

    // check if we have a counter stored already - if not we add one
    int counter = 1;
    QString counterName( baseName + mCounterTag );
    QMapIterator<const void*, QString> i( mMap );
    while( i.hasNext() ) {
        i.next();
        if( i.value() == counterName ){
            // we may cast away constness, since this is not
            // an external pointer but our own auxiliary counter entry
            int* storedCount = const_cast<int*>( static_cast<const int*>( i.key() ) );
            (*storedCount)++;
            counter = *storedCount;
        }
    }
    if( counter == 1 ){
        int* p = new int;
        *p = counter;
        mMap[ p ] = counterName;
    }

    // store a new name using the counter value, and return it
    QString name( baseName + ":" + QString::number( counter ) );
    mMap[ id ] = name;
    return name;
}


const void* IdMapper::findId( const QString& name )const
{
    QMapIterator<const void*, QString> i( mMap );
    while( i.hasNext() ) {
        i.next();
        if( i.value() == name )
            return i.key();
    }
    return 0;
}
