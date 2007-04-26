/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include <qglobal.h>

#include "KDChartAbstractGrid.h"
#include "KDChartPaintContext.h"

#include <KDABLibFakes>


using namespace KDChart;


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
