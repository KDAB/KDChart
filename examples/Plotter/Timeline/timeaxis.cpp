#include "timeaxis.h"

#include <QtCore/QDateTime>

TimeAxis::TimeAxis( KDChart::AbstractCartesianDiagram* parent )
    : KDChart::CartesianAxis( parent )
{
    // Intentionally left blank
}

const QString TimeAxis::customizedLabel( const QString& label ) const
{
    // Here we are free to format the value to whatever we want. As example we
    // could also return a scientific notation with something like the following
    // both lines;
    //const int precision = 2;
    //return QString::number(label.toDouble(), 'E', precision);

    // Format the time-value to a nice string representation.
    const QDateTime dateTime = QDateTime::fromTime_t( label.toDouble() * 3600.0 );
    return dateTime.date().toString();
}
