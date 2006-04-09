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

#ifndef KDCHARTBACKGROUNDATTRIBUTES_H
#define KDCHARTBACKGROUNDATTRIBUTES_H

#include <QMetaType>
#include <QBrush>
#include "KDChartGlobal.h"

class QDomDocumentFragment;

namespace KDChart {

class KDCHART_EXPORT BackgroundAttributes
{
public:
    BackgroundAttributes();
    BackgroundAttributes( const BackgroundAttributes& );
    BackgroundAttributes &operator= ( const BackgroundAttributes& );

    ~BackgroundAttributes();

    enum BackgroundPixmapMode { BackgroundPixmapModeNone,
                                BackgroundPixmapModeCentered,
                                BackgroundPixmapModeScaled,
                                BackgroundPixmapModeStretched };

    void setVisible( bool visible );
    bool isVisible() const;

    void setBrush( const QBrush &brush );
    QBrush brush() const;

    void setPixmapMode( BackgroundPixmapMode mode );
    BackgroundPixmapMode pixmapMode() const;

    void setPixmap( const QPixmap &backPixmap );
    QPixmap pixmap() const;

    bool operator==( const BackgroundAttributes& ) const;
    inline bool operator!=( const BackgroundAttributes& other ) const { return !operator==(other); }

    // XML serialization
    QDomDocumentFragment toXML() const;

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }

}; // End of class BackgroundAttributes

}

Q_DECLARE_METATYPE( KDChart::BackgroundAttributes )

#endif // KDCHARTBACKGROUNDATTRIBUTES_H
