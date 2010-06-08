#include "KDChartLeveyJenningsDiagram.h"
#include "KDChartDataValueAttributes.h"

#include "KDChartLeveyJenningsDiagram_p.h"

using namespace KDChart;

LeveyJenningsDiagram::Private::Private( const Private& rhs )
    : LineDiagram::Private( rhs ),
      lotChangedPosition( rhs.lotChangedPosition ),
      fluidicsPackChangedPosition( rhs.fluidicsPackChangedPosition ),
      sensorChangedPosition( rhs.sensorChangedPosition ),
      fluidicsPackChanges( rhs.fluidicsPackChanges ),
      sensorChanges( rhs.sensorChanges ),
      scanLinePen( rhs.scanLinePen ),
      icons( rhs.icons ),
      expectedMeanValue( rhs.expectedMeanValue ),
      expectedStandardDeviation( rhs.expectedStandardDeviation )
{
}

void LeveyJenningsDiagram::Private::setYAxisRange() const
{
    CartesianCoordinatePlane* const plane = static_cast< CartesianCoordinatePlane* >( diagram->coordinatePlane() );
    if( plane == 0 )
        return;

    plane->setVerticalRange( QPair< qreal, qreal >( expectedMeanValue - 4 * expectedStandardDeviation, 
                                                    expectedMeanValue + 4 * expectedStandardDeviation ) );
}
