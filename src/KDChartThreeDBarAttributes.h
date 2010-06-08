#ifndef KDCHARTTHREEDBARATTRIBUTES_H
#define KDCHARTTHREEDBARATTRIBUTES_H

#include <QMetaType>
#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartGlobal.h"

namespace KDChart {

  /**
    * @brief A set of 3D bar attributes
    */
  class KDCHART_EXPORT ThreeDBarAttributes : public AbstractThreeDAttributes
  {
  public:
    ThreeDBarAttributes();
    ThreeDBarAttributes( const ThreeDBarAttributes& );
    ThreeDBarAttributes &operator= ( const ThreeDBarAttributes& );

    ~ThreeDBarAttributes();

    /* threeD Bars specific */
    void setUseShadowColors( bool useShadowColors );
    bool useShadowColors() const;

    //Pending Michel I am not sure this will be used
    void setAngle( uint threeDAngle );
    uint angle() const;

    bool operator==( const ThreeDBarAttributes& ) const;
    inline bool operator!=( const ThreeDBarAttributes& other ) const { return !operator==(other); }

    KDCHART_DECLARE_SWAP_DERIVED(ThreeDBarAttributes)

    KDCHART_DECLARE_PRIVATE_DERIVED(ThreeDBarAttributes)

  }; // End of class ThreeDBarAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::ThreeDBarAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

Q_DECLARE_METATYPE( KDChart::ThreeDBarAttributes )
Q_DECLARE_TYPEINFO( KDChart::ThreeDBarAttributes, Q_MOVABLE_TYPE );
KDCHART_DECLARE_SWAP_SPECIALISATION_DERIVED( KDChart::ThreeDBarAttributes )

#endif // KDCHARTTHREEDBARATTRIBUTES_H
