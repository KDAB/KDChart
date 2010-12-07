#ifndef KDCHARTVALUETRACKERATTRIBUTES_H
#define KDCHARTVALUETRACKERATTRIBUTES_H

#include <QDebug>
#include <QMetaType>
#include "KDChartGlobal.h"

namespace KDChart {

    /**
     * \class ValueTrackerAttributes KDChartValueTrackerAttributes.h KDChartValueTrackerAttributes
     * \brief Cell-specific attributes regarding value tracking
     *
     * ValueTrackerAttributes groups the properties regarding
     * value tracking, and how it is displayed.
     * Value tracking can be used to emphasize on one or several
     * specific points in a line diagram.
   */

    class KDCHART_EXPORT ValueTrackerAttributes
    {
        public:
            ValueTrackerAttributes();
            ValueTrackerAttributes( const ValueTrackerAttributes& );
            ValueTrackerAttributes &operator= ( const ValueTrackerAttributes& );

            ~ValueTrackerAttributes();

            /** Set the pen the value tracking lines and markers will be drawn with
             * \param pen The pen the lines and markers will be drawn with
             */
            void setPen( const QPen& pen );

            /**
             * @return The pen the lines and markers are drawn with
             */
            QPen pen() const;

            /** Set the brush the area below the value tracking
             * lines should be filled with. Default is a black brush
             * with the style Qt::NoBrush.
             * \param brush The brush the area should be filled with
             */
            void setAreaBrush( const QBrush& brush );

            /**
             * @return The brush the area below the value tracking lines is filled with
             */
            QBrush areaBrush() const;

            /** Set the size of the markers. This includes both the arrows at
             * the axises and the circle at the data point.
             * \param size The size of the markers
             */
            void setMarkerSize( const QSizeF& size );

            /**
             * @return The size of the markers
             */
            QSizeF markerSize() const;

            /**
             * @return The orientations used to show the value tracking. Using only a
             * vertical line, horizontal line or both (the default).
             */
            Qt::Orientations orientations() const;

            /** Set the orientations used to show the value tracking.
             * \param orientations The orientations of the value tracking lines.
             */
            void setOrientations( Qt::Orientations orientations );

            /** Set whether value tracking should be enabled for a specific
             * index or not
             * \param enabled Whether value tracking should be enabled or not
             */
            void setEnabled( bool enabled );

            /**
             * @return Whether value tracking is enabled or not
             */
            bool isEnabled() const;

            bool operator==( const ValueTrackerAttributes& ) const;
            inline bool operator!=( const ValueTrackerAttributes& other ) const { return !operator==(other); }

        private:
            KDCHART_DECLARE_PRIVATE_BASE_VALUE( ValueTrackerAttributes )
    }; // End of class ValueTrackerAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::ValueTrackerAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

Q_DECLARE_METATYPE( KDChart::ValueTrackerAttributes )
KDCHART_DECLARE_SWAP_SPECIALISATION( KDChart::ValueTrackerAttributes )
Q_DECLARE_TYPEINFO( KDChart::ValueTrackerAttributes, Q_MOVABLE_TYPE );

#endif // KDCHARTVALUETRACKERATTRIBUTES_H
