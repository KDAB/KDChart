#include "AdjustedCartesianAxis.h"


AdjustedCartesianAxis::AdjustedCartesianAxis ( KDChart::AbstractCartesianDiagram* diagram )
    : CartesianAxis ( diagram )
    , m_lowerBound(0.0)
    , m_upperBound(0.0)
{
    // this bloc left empty intentionally
}

const QString AdjustedCartesianAxis::customizedLabel( const QString& label )const
{
    bool bOK;
    double val = label.toDouble( & bOK );
    if( bOK ){
        if( val < m_lowerBound )
            return tr("LOW");
        else if( val > m_upperBound )
            return tr("HIGH");
    }
    return label;
}
