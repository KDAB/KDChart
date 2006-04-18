/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTTHREEDATTRIBUTES_H
#define KDCHARTTHREEDATTRIBUTES_H

#include <QMetaType>
#include <QtXml/QDomDocumentFragment>
#include "KDChartGlobal.h"

namespace KDChart {

class KDCHART_EXPORT ThreeDAttributes
{
public:
    ThreeDAttributes();
    ThreeDAttributes( const ThreeDAttributes& );
    ThreeDAttributes &operator= ( const ThreeDAttributes& );

    virtual ~ThreeDAttributes() = 0;

    void setEnabled( bool enabled );
    bool isEnabled() const;
    /*what do we use height attribute for?*/
    void setHeight( int pixels );
    int height() const;

    void setDepth( int depth );
    int depth() const;

    bool operator==( const ThreeDAttributes& ) const;
    inline bool operator!=( const ThreeDAttributes& other ) const { return !operator==(other); }

    // XML serialization
    QDomDocumentFragment toXML() const;

    KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC(ThreeDAttributes)
}; // End of class ThreeDAttributes

}

#endif // KDCHARTTHREEDATTRIBUTES_H
