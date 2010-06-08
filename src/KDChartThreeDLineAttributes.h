#ifndef KDCHARTTHREEDLINEATTRIBUTES_H
#define KDCHARTTHREEDLINEATTRIBUTES_H

#include <QMetaType>
#include "KDChartAbstractThreeDAttributes.h"
#include "KDChartGlobal.h"

namespace KDChart {

  /**
    * @brief A set of 3D line attributes
    */
  class KDCHART_EXPORT ThreeDLineAttributes : public AbstractThreeDAttributes
  {
  public:
    ThreeDLineAttributes();
    ThreeDLineAttributes( const ThreeDLineAttributes& );
    ThreeDLineAttributes &operator= ( const ThreeDLineAttributes& );

    ~ThreeDLineAttributes();

    /* threeD lines specific */
    void setLineXRotation( const uint degrees );
    uint lineXRotation() const;
    void setLineYRotation( const uint degrees );
    uint lineYRotation() const;

    bool operator==( const ThreeDLineAttributes& ) const;
    inline bool operator!=( const ThreeDLineAttributes& other ) const { return !operator==(other); }

    KDCHART_DECLARE_SWAP_DERIVED(ThreeDLineAttributes)

    KDCHART_DECLARE_PRIVATE_DERIVED(ThreeDLineAttributes)

  }; // End of class ThreeDLineAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::ThreeDLineAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

Q_DECLARE_METATYPE( KDChart::ThreeDLineAttributes )
Q_DECLARE_TYPEINFO( KDChart::ThreeDLineAttributes, Q_MOVABLE_TYPE );
KDCHART_DECLARE_SWAP_SPECIALISATION_DERIVED( KDChart::ThreeDLineAttributes )


#endif // KDCHARTTHREEDLINEATTRIBUTES_H
