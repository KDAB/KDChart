#ifndef KDCHARTBARATTRIBUTES_H
#define KDCHARTBARATTRIBUTES_H

#include <QMetaType>
#include <QtXml/QDomDocumentFragment>
#include "KDChartGlobal.h"

namespace KDChart {

class KDCHART_EXPORT BarAttributes
{
public:
    BarAttributes();
    BarAttributes( const BarAttributes& );
    BarAttributes &operator= ( const BarAttributes& );

    ~BarAttributes();

    void setFixedDataValueGap( double gap );
    double fixedDataValueGap() const;

    void setUseFixedDataValueGap( bool gapIsFixed );
    bool useFixedDataValueGap() const;

    void setFixedValueBlockGap( double gap );
    double fixedValueBlockGap() const;

    void setUseFixedValueBlockGap( bool gapIsFixed );
    bool useFixedValueBlockGap() const;

    void setFixedBarWidth( double width );
    double fixedBarWidth() const;

    void setUseFixedBarWidth( bool useFixedBarWidth );
    bool useFixedBarWidth() const;

    void setGroupGapFactor ( double gapFactor );
    double groupGapFactor() const;

    void setBarGapFactor( double gapFactor );
    double barGapFactor() const;

    void setDrawSolidExcessArrows( bool solidArrows );
    bool drawSolidExcessArrows() const;

    bool operator==( const BarAttributes& ) const;
    inline bool operator!=( const BarAttributes& other ) const { return !operator==(other); }

    // XML serialization
    QDomDocumentFragment toXML() const;

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }
}; // End of class BarAttributes

}

Q_DECLARE_METATYPE( KDChart::BarAttributes )

#endif // KDCHARTBARATTRIBUTES_H
