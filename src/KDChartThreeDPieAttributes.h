#ifndef KDCHARTTHREEDPIEATTRIBUTES_H
#define KDCHARTTHREEDPIEATTRIBUTES_H

#include <QMetaType>
#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartGlobal.h"

namespace KDChart {

  /**
    * @brief A set of 3D pie attributes
    */
  class KDCHART_EXPORT ThreeDPieAttributes : public AbstractThreeDAttributes
  {
  public:
    ThreeDPieAttributes();
    ThreeDPieAttributes( const ThreeDPieAttributes& );
    ThreeDPieAttributes &operator= ( const ThreeDPieAttributes& );

    ~ThreeDPieAttributes();

    /* threeD Pies specific */
    void setUseShadowColors( bool useShadowColors );
    bool useShadowColors() const;

    bool operator==( const ThreeDPieAttributes& ) const;
    inline bool operator!=( const ThreeDPieAttributes& other ) const { return !operator==(other); }


    KDCHART_DECLARE_SWAP_DERIVED(ThreeDPieAttributes)

private:
    KDCHART_DECLARE_PRIVATE_DERIVED(ThreeDPieAttributes)

  }; // End of class ThreeDPieAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::ThreeDPieAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

Q_DECLARE_METATYPE( KDChart::ThreeDPieAttributes )
Q_DECLARE_TYPEINFO( KDChart::ThreeDPieAttributes, Q_MOVABLE_TYPE );
KDCHART_DECLARE_SWAP_SPECIALISATION_DERIVED( KDChart::ThreeDPieAttributes )

#endif // KDCHARTTHREEDPIEATTRIBUTES_H
