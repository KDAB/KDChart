#ifndef KDCHARTRULERATTRIBUTES_H
#define KDCHARTRULERATTRIBUTES_H

#include <QMetaType>
#include "KDChartGlobal.h"
#include "KDChartEnums.h"

class QPen;

namespace KDChart {

/**
  * @brief A set of attributes controlling the appearance of axis rulers
  */
class KDCHART_EXPORT RulerAttributes
{
public:
	RulerAttributes();
	RulerAttributes( const RulerAttributes& );
	RulerAttributes &operator= ( const RulerAttributes& );

    ~RulerAttributes();
    
    /**
      * Sets the pen used to draw the tick marks
      */
    void setTickMarkPen( const QPen& pen );
    QPen tickMarkPen() const;

    /**
      * Sets the pen used to draw major tick marks
      */
    void setMajorTickMarkPen( const QPen& pen );
    bool majorTickMarkPenIsSet() const;
    QPen majorTickMarkPen() const;

    /**
      * Sets the pen used to draw minor tick marks
      */
    void setMinorTickMarkPen( const QPen& pen );
    bool minorTickMarkPenIsSet() const;
    QPen minorTickMarkPen() const;

    /**
      * Sets the pen used to draw the tick mark at a specific value
      * 
      * Note: This will not paint a tick mark at the specified value
      * if it wasn't already drawn before. 
      */
    void setTickMarkPen( qreal value, const QPen& pen );
    QPen tickMarkPen( qreal value ) const;
    typedef QMap<qreal, QPen> TickMarkerPensMap;
    TickMarkerPensMap tickMarkPens() const;
    
    bool hasTickMarkPenAt( qreal value) const;

    /**
      * Color setter method provided for convenience
      */
    void setTickMarkColor( const QColor& color );
    QColor tickMarkColor() const;

    /**
      * Shows or hides minor tick marks
      */
    void setShowMinorTickMarks( bool show );
    bool showMinorTickMarks() const;

    /**
      * Shows or hides major tick marks
      */
    void setShowMajorTickMarks( bool show );
    bool showMajorTickMarks() const;
    
    /**
     * Set margin that should be used between the labals and the ticks. Per
     * default the value is -1 what means, that half of the label's font
     * height/width should be used as margin.
     */
    void setLabelMargin(int margin);
    int labelMargin() const;

    bool operator==( const RulerAttributes& ) const;
    inline bool operator!=( const RulerAttributes& other ) const { return !operator==(other); }

private:
    KDCHART_DECLARE_PRIVATE_BASE_VALUE( RulerAttributes )
}; // End of class RulerAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::RulerAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

KDCHART_DECLARE_SWAP_SPECIALISATION( KDChart::RulerAttributes )
Q_DECLARE_METATYPE( KDChart::RulerAttributes )
Q_DECLARE_TYPEINFO( KDChart::RulerAttributes, Q_MOVABLE_TYPE );


#endif // KDCHARTRULERATTRIBUTES_H
