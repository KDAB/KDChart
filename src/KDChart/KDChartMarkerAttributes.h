/****************************************************************************
** Copyright (C) 2001-2016 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDCHARTMARKERATTRIBUTES_H
#define KDCHARTMARKERATTRIBUTES_H

#include <QMetaType>
#include "KDChartGlobal.h"

QT_BEGIN_NAMESPACE
class QColor;
class QSizeF;
class QPen;
class QPainterPath;
class QDebug;
template <typename T, typename K> class QMap;
QT_END_NAMESPACE

namespace KDChart {

    /**
      * @brief A set of attributes controlling the appearance of data set markers
      */
    class KDCHART_EXPORT MarkerAttributes
    {
    public:
        MarkerAttributes();
        MarkerAttributes( const MarkerAttributes& );
        MarkerAttributes &operator= ( const MarkerAttributes& );

        ~MarkerAttributes();

        enum MarkerStyle { MarkerCircle  = 0,
                           MarkerSquare  = 1,
                           MarkerDiamond = 2,
                           Marker1Pixel  = 3,
                           Marker4Pixels = 4,
                           MarkerRing    = 5,
                           MarkerCross   = 6,
                           MarkerFastCross = 7,
                           NoMarker = 8,
                           PainterPathMarker = 9,
                           StartCustomMarkers = 10 };

        void setVisible( bool visible );
        bool isVisible() const;

        typedef QMap<uint, uint> MarkerStylesMap;
        void setMarkerStylesMap( const MarkerStylesMap & map );
        MarkerStylesMap markerStylesMap() const;

        void setThreeD( bool value );
        bool threeD() const;

        /**
         * Set the marker-style to use. This could be either one of the
         * predefined \a MarkerStyle or a custom one that has a value
         * bigger or equal to StartCustomMarkers.
         *
         * Such a custom marker does then allow to fetch a custom pixmap
         * for each point (value pair) from the model using the
         * Qt::DecorationRole .
         */
        void setMarkerStyle( uint style );
        uint markerStyle() const;

        /**
         * Normally you need to specify a valid QSizeF here, but for Legends you can
         * use the invalid size QSizeF(), to enable automatic marker size calculation:
         *
         * For Markers shown in a Legend this means the marker size will be equal to
         * the font height used for the labels that are shown next to the markers.
         */
        void setMarkerSize( const QSizeF& size );
        QSizeF markerSize() const;

        void setMarkerColor( const QColor& color );
        QColor markerColor() const;

        void setCustomMarkerPath( const QPainterPath& path );
        QPainterPath customMarkerPath() const;

        void setPen( const QPen& pen );
        QPen pen() const;

        bool operator==( const MarkerAttributes& ) const;
        bool operator!=( const MarkerAttributes& ) const;

    private:
        KDCHART_DECLARE_PRIVATE_BASE_VALUE( MarkerAttributes )
    }; // End of class MarkerAttributes

    inline bool MarkerAttributes::operator!=( const MarkerAttributes & other ) const { return !operator==( other ); }
}

#ifndef QT_NO_DEBUG_STREAM
KDCHART_EXPORT QDebug operator<<( QDebug, const KDChart::MarkerAttributes & );
#endif

KDCHART_DECLARE_SWAP_SPECIALISATION( KDChart::MarkerAttributes )

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO( KDChart::MarkerAttributes, Q_MOVABLE_TYPE );
QT_END_NAMESPACE

Q_DECLARE_METATYPE( KDChart::MarkerAttributes )

#endif // KDCHARTMARKERATTRIBUTES_H
