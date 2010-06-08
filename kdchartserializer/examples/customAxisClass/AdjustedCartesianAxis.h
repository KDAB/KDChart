#ifndef ADJUSTED_CARTESIAN_AXIS_H
#define ADJUSTED_CARTESIAN_AXIS_H

#include <KDChartCartesianAxis>
#include <KDChartGlobal>


class AdjustedCartesianAxis: public KDChart::CartesianAxis
{
    Q_OBJECT

    Q_DISABLE_COPY( AdjustedCartesianAxis )

public:
    explicit AdjustedCartesianAxis( KDChart::AbstractCartesianDiagram* diagram = 0 );

    virtual const QString customizedLabel( const QString& label )const;

    void setBounds( qreal lower, qreal upper ){
        m_lowerBound = lower;
        m_upperBound = upper;
    }
    qreal lowerBound()const { return m_lowerBound; }
    qreal upperBound()const { return m_upperBound; }

private:
    qreal m_lowerBound;
    qreal m_upperBound;
};

#endif // ADJUSTED_CARTESIAN_AXIS_H
