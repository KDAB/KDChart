#ifndef KDCHARTTHREEDBARATTRIBUTES_H
#define KDCHARTTHREEDBARATTRIBUTES_H

#include <QMetaType>
#include <QtXml/QDomDocumentFragment>
#include "KDChartThreeDAttributes.h"
#include "KDChartGlobal.h"

namespace KDChart {

  class KDCHART_EXPORT ThreeDBarAttributes : public ThreeDAttributes
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

    // XML serialization
    QDomDocumentFragment toXML() const;

    KDCHART_DECLARE_PRIVATE_DERIVED(ThreeDBarAttributes)

  }; // End of class ThreeDBarAttributes

}

Q_DECLARE_METATYPE( KDChart::ThreeDBarAttributes )

#endif // KDCHARTTHREEDBARATTRIBUTES_H
