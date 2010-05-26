#include "timeaxis.h"

#include <QtCore/QDateTime>

TimeAxis::TimeAxis( KDChart::AbstractCartesianDiagram* parent )
    : KDChart::CartesianAxis( parent )
{
    // Intentionally left blank
}

const QString TimeAxis::customizedLabel( const QString& label ) const
{
    const QDateTime dateTime = QDateTime::fromTime_t( label.toDouble() * 3600.0 );
    return dateTime.date().toString();
}
