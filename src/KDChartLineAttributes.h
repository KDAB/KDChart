#ifndef KDCHARTLINEATTRIBUTES_H
#define KDCHARTLINEATTRIBUTES_H

#include <QMetaType>
#include <QtXml/QDomDocumentFragment>
#include "KDChartGlobal.h"

namespace KDChart {

class KDCHART_EXPORT LineAttributes
{
public:
    LineAttributes();
    LineAttributes( const LineAttributes& );
    LineAttributes &operator= ( const LineAttributes& );

    ~LineAttributes();


    /* area chart - all types */
    void setDisplayArea( bool display );
    bool displayArea() const;
    /*allows viewing the covered areas*/
    void setTransparency( uint alpha );
    uint transparency() const;

    bool operator==( const LineAttributes& ) const;
    inline bool operator!=( const LineAttributes& other ) const { return !operator==(other); }

    // XML serialization
    QDomDocumentFragment toXML() const;

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }
}; // End of class LineAttributes

}

Q_DECLARE_METATYPE( KDChart::LineAttributes )

#endif // KDCHARTLINEATTRIBUTES_H
