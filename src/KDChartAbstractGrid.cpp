#include "KDChartAbstractGrid.h"
#include "KDChartPaintContext.h"

#include <qglobal.h>

#include <KDABLibFakes>


using namespace KDChart;
using namespace std;


static qreal _trunc( qreal v )
{
    return (( v > 0.0 ) ? floor( v ) : ceil(  v ));
}


AbstractGrid::AbstractGrid ()
    : mPlane( 0 )
{
    //this bloc left empty intentionally
}

AbstractGrid::~AbstractGrid()
{
    //this bloc left empty intentionally
}

void AbstractGrid::setNeedRecalculate()
{
    mCachedRawDataDimensions.clear();
}

DataDimensionsList AbstractGrid::updateData( AbstractCoordinatePlane* plane )
{

    if( plane ){
        const DataDimensionsList rawDataDimensions( plane->getDataDimensionsList() );
        if( mCachedRawDataDimensions.empty() || (rawDataDimensions != mCachedRawDataDimensions) ){
            mCachedRawDataDimensions = rawDataDimensions;
            mPlane = plane;
            mData = calculateGrid( rawDataDimensions );
            //qDebug("AbstractGrid::updateData() returns  mData.first().start: %f   mData.first().end: %f   mData.first().stepWidth: %f", mData.first().start, mData.first().end, mData.first().stepWidth);
        }
    }
    return mData;
}

bool AbstractGrid::isBoundariesValid(const QRectF& r )
{
  return isBoundariesValid(
      QPair<QPointF,QPointF>(r.topLeft(), r.bottomRight()) );
}

bool AbstractGrid::isBoundariesValid(const QPair<QPointF,QPointF>& b )
{
  return isValueValid( b.first.x() )  && isValueValid( b.first.y() ) &&
         isValueValid( b.second.x() ) && isValueValid( b.second.y() );
}

bool AbstractGrid::isBoundariesValid(const DataDimensionsList& l )
{
    for (int i = 0; i < l.size(); ++i)
        if ( ! isValueValid( l.at(i).start ) || ! isValueValid( l.at(i).end ) )
            return false;
    return true;
}

bool AbstractGrid::isValueValid(const qreal& r )
{
  return !(ISNAN(r) || ISINF(r));
}

void AbstractGrid::adjustLowerUpperRange(
        qreal& start, qreal& end,
        qreal stepWidth,
        bool adjustLower, bool adjustUpper )
{
    const qreal startAdjust = ( start >= 0.0 ) ? 0.0 : -1.0;
    const qreal endAdjust   = ( end   >= 0.0 ) ? 1.0 :  0.0;
    if ( adjustLower && (fmod( start, stepWidth ) != 0.0) )
        start = stepWidth * (_trunc( start / stepWidth ) + startAdjust);
    if ( adjustUpper && (fmod( end, stepWidth ) != 0.0) )
        end = stepWidth * (_trunc( end / stepWidth ) + endAdjust);
}

const DataDimension AbstractGrid::adjustedLowerUpperRange(
        const DataDimension& dim,
        bool adjustLower, bool adjustUpper )
{
    DataDimension result( dim );
    adjustLowerUpperRange(
            result.start, result.end,
            result.stepWidth,
            adjustLower, adjustUpper );
    return result;
}
