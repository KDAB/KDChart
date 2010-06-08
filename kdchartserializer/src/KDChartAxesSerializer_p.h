#ifndef __KDCHARTAXESSERIALIZER_P_H__
#define __KDCHARTAXESSERIALIZER_P_H__

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <KDChartAxesSerializer.h>
#include <KDChartCartesianAxis>
//TODO once PolarAxis is implemented: #include <KDChartPolarAxis>

/**
 * \internal
 */
class KDChart::AxesSerializer::Private
{
    friend class ::KDChart::AxesSerializer;
    AxesSerializer * const q;
public:
    explicit Private( AxesSerializer * qq );
    ~Private(); // non-virtual, since nothing inherits this

    bool doParseCartesianAxis( const QDomElement& axisElement, CartesianAxis*& axisPtr ) const;
//    bool doParsePolarAxis( const QDomElement& axisElement, PolarAxis*& axisPtr ) const;

    void saveAbstractAxis( QDomDocument& doc, QDomElement& e, const AbstractAxis& axis, const QString& title ) const;
    void saveCartesianAxis( QDomDocument& doc, QDomElement& axisElement, const CartesianAxis& axis ) const;

};


#endif // KDChartAxesSerializer_p_H
