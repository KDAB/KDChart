#ifndef TIMEAXIS_H
#define TIMEAXIS_H

#include <KDChartCartesianAxis>

class TimeAxis : public KDChart::CartesianAxis
{
    Q_OBJECT
public:
    explicit TimeAxis( KDChart::AbstractCartesianDiagram* parent );
    const QString customizedLabel( const QString& label ) const;
};


#endif
