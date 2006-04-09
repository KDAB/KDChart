#ifndef KDCHARTTHREEDLINEATTRIBUTES_H
#define KDCHARTTHREEDLINEATTRIBUTES_H

#include <QMetaType>
#include <QtXml/QDomDocumentFragment>
#include "KDChartThreeDAttributes.h"
#include "KDChartGlobal.h"

namespace KDChart {

  class KDCHART_EXPORT ThreeDLineAttributes : public ThreeDAttributes
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

    // XML serialization
    QDomDocumentFragment toXML() const;

    KDCHART_DECLARE_PRIVATE_DERIVED(ThreeDLineAttributes)

  }; // End of class ThreeDLineAttributes

}

Q_DECLARE_METATYPE( KDChart::ThreeDLineAttributes )

#endif // KDCHARTTHREEDLINEATTRIBUTES_H
