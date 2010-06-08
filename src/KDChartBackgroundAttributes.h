#ifndef KDCHARTBACKGROUNDATTRIBUTES_H
#define KDCHARTBACKGROUNDATTRIBUTES_H

#include <QDebug>
#include <QMetaType>
#include <QBrush>
#include "KDChartGlobal.h"

namespace KDChart {

/**
  * Set of attributes usable for background pixmaps
  */
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

    bool isEqualTo( const BackgroundAttributes& other, bool ignorePixmap=false ) const;

private:
    KDCHART_DECLARE_PRIVATE_BASE_VALUE( BackgroundAttributes )
}; // End of class BackgroundAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::BackgroundAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

Q_DECLARE_METATYPE( KDChart::BackgroundAttributes )
Q_DECLARE_TYPEINFO( KDChart::BackgroundAttributes, Q_MOVABLE_TYPE );
KDCHART_DECLARE_SWAP_SPECIALISATION( KDChart::BackgroundAttributes )

#endif // KDCHARTBACKGROUNDATTRIBUTES_H
