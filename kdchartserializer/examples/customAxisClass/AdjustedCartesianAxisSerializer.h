#ifndef __ADJUSTEDCARTESIANAXISSERIALIZER_H__
#define __ADJUSTEDCARTESIANAXISSERIALIZER_H__

#include <KDChartAxesSerializer>

class AdjustedCartesianAxisSerializer : public KDChart::AxesSerializer
{
    Q_OBJECT

    Q_DISABLE_COPY( AdjustedCartesianAxisSerializer )

public:
    explicit AdjustedCartesianAxisSerializer();
    virtual ~AdjustedCartesianAxisSerializer();

    /** reimp */
    virtual void saveElement( QDomDocument& doc, QDomElement& e, const QObject* obj ) const;
    /** reimp */
    virtual bool parseElement( const QDomElement& container, QObject* ptr ) const;
};

#endif
