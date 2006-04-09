/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart licenses may use this file in
 ** accordance with the KDChart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTFRAMEATTRIBUTES_H
#define KDCHARTFRAMEATTRIBUTES_H

#include <QMetaType>
#include <QPen>
#include "KDChartGlobal.h"

class QDomDocumentFragment;

namespace KDChart {

class KDCHART_EXPORT FrameAttributes
{
public:
    FrameAttributes();
    FrameAttributes( const FrameAttributes& );
    FrameAttributes &operator= ( const FrameAttributes& );

    ~FrameAttributes();

    void setVisible( bool visible );
    bool isVisible() const;

    void setPen( const QPen & pen );
    QPen pen() const;

    void setPadding( int padding );
    int padding() const;

    bool operator==( const FrameAttributes& ) const;
    inline bool operator!=( const FrameAttributes& other ) const { return !operator==(other); }

    // XML serialization
    QDomDocumentFragment toXML() const;

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }

}; // End of class FrameAttributes

}

Q_DECLARE_METATYPE( KDChart::FrameAttributes )

#endif // KDCHARTFRAMEATTRIBUTES_H
