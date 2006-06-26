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

#include "KDChartGridAttributes.h"

#include <QPen>
#include <QDomDocumentFragment>

#define d d_func()

using namespace KDChart;

class GridAttributes::Private
{
    friend class GridAttributes;
public:
    Private();
private:
    bool visible;
    KDChartEnums::GranularitySequence sequence;
    qreal stepWidth;
    QPen pen;
    bool subVisible;
    QPen subPen;
    QPen zeroPen;
};

GridAttributes::Private::Private()
    : visible( true ),
      sequence( KDChartEnums::GranularitySequence_10_20 ),
      stepWidth( 0.0 ),
      pen( QColor(0xa0, 0xa0, 0xa0 ) ),
      subVisible( true ),
      subPen( QColor(0xd0, 0xd0, 0xd0 ) ),
      zeroPen( QColor( 0x00, 0x00, 0x80 ) )
{
    // this bloc left empty intentionally
}


GridAttributes::GridAttributes()
    : _d( new Private() )
{
    // this bloc left empty intentionally
}

GridAttributes::GridAttributes( const GridAttributes& r )
    : _d( new Private( *r.d ) )
{
}

GridAttributes & GridAttributes::operator=( const GridAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

GridAttributes::~GridAttributes()
{
    delete _d; _d = 0;
}


bool GridAttributes::operator==( const GridAttributes& r ) const
{
    return ( isGridVisible() == r.isGridVisible() &&
             gridGranularitySequence() == r.gridGranularitySequence() &&
	     gridPen() == r.gridPen() &&
	     isSubGridVisible() == r.isSubGridVisible() &&
	     subGridPen() == r.subGridPen() &&
	     zeroLinePen() == r.zeroLinePen() );
}


void GridAttributes::setGridVisible( bool visible )
{
    d->visible = visible;
}

bool GridAttributes::isGridVisible() const
{
    return d->visible;
}

/**
  * Specifies the step width to be used for calculating
  * the grid lines.
  *
  * By default the GridAttributes class does not use a fixed step width,
  * but it uses KDChartEnums::GranularitySequence_10_20.
  *
  * \param stepWidth the step width to be used.
  * If this parameter is omitted (or set to Zero, resp.)
  * the automatic step width calculation will be done,
  * using the granularity sequence specified.
  * This is the default.
  *
  * \sa gridStepWidth, setGranularitySequence
  */
void GridAttributes::setGridStepWidth( qreal stepWidth )
{
    d->stepWidth = stepWidth;
}

/**
  * Returns the step width to be used for calculating
  * the grid lines.
  *
  * \sa setGridStepWidth
  */
qreal GridAttributes::gridStepWidth() const
{
    return d->stepWidth;
}

/**
  * Specifies the granularity sequence to be used for calculating
  * the grid lines.
  *
  * By default the GridAttributes class uses KDChartEnums::GranularitySequence_10_20.
  * 
  * \note The sequence specified by this method is ignored, if
  * a fixed step width was specified via setStepWidth.
  * 
  * \param sequence one of the sequences declared in
  * KDChartEnums::GranularitySequence.
  *
  * \sa gridGranularitySequence, setStepWidth
  */
void GridAttributes::setGridGranularitySequence( KDChartEnums::GranularitySequence sequence )
{
    d->sequence = sequence;
}

/**
  * Returns the granularity sequence to be used for calculating
  * the grid lines.
  *
  * \sa setGridGranularitySequence
  */
KDChartEnums::GranularitySequence GridAttributes::gridGranularitySequence() const
{
    return d->sequence;
}


void GridAttributes::setGridPen( const QPen & pen )
{
    d->pen = pen;
}

QPen GridAttributes::gridPen() const
{
    return d->pen;
}

void GridAttributes::setSubGridVisible( bool visible )
{
    d->subVisible = visible;
}

bool GridAttributes::isSubGridVisible() const
{
    return d->subVisible;
}

void GridAttributes::setSubGridPen( const QPen & pen )
{
    d->subPen = pen;
}

QPen GridAttributes::subGridPen() const
{
    return d->subPen;
}

void GridAttributes::setZeroLinePen( const QPen & pen )
{
    d->zeroPen = pen;
}

QPen GridAttributes::zeroLinePen() const
{
    return d->zeroPen;
}

QDomDocumentFragment GridAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: GridAttributes::toXML()" );
    return QDomDocumentFragment();
}
