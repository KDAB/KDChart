#ifndef KDCHARTMARKERATTRIBUTES_H
#define KDCHARTMARKERATTRIBUTES_H

#include <QMetaType>
#include "KDChartGlobal.h"

class QColor;
class QSizeF;
class QPen;
class QDebug;
template <typename T, typename K> class QMap;

namespace KDChart {

    /**
      * @brief A set of ottributes controlling the appearance of data set markers
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
                           NoMarker = 8 };

        void setVisible( bool visible );
        bool isVisible() const;

        typedef QMap<uint, MarkerStyle> MarkerStylesMap;
        void setMarkerStylesMap( const MarkerStylesMap & map );
        MarkerStylesMap markerStylesMap() const;

        void setMarkerStyle( MarkerStyle style );
        MarkerStyle markerStyle() const;

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

        void setPen( const QPen& pen );
        QPen pen() const;

        bool operator==( const MarkerAttributes& ) const;
        bool operator!=( const MarkerAttributes& ) const;

    private:
        KDCHART_DECLARE_PRIVATE_BASE_VALUE( MarkerAttributes )
    }; // End of class MarkerAttributes

    inline bool MarkerAttributes::operator!=( const MarkerAttributes & other ) const { return !operator==( other ); }
}

KDCHART_DECLARE_SWAP_SPECIALISATION( KDChart::MarkerAttributes )
Q_DECLARE_TYPEINFO( KDChart::MarkerAttributes, Q_MOVABLE_TYPE );
Q_DECLARE_METATYPE( KDChart::MarkerAttributes )

#ifndef QT_NO_DEBUG_STREAM
KDCHART_EXPORT QDebug operator<<( QDebug, const KDChart::MarkerAttributes & );
#endif

#endif // KDCHARTMARKERATTRIBUTES_H
