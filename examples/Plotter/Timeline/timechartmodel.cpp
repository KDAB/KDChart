/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "timechartmodel.h"

TimeChartModel::TimeChartModel( QObject* parent )
    : QSortFilterProxyModel( parent )
{
}

QPair< QDateTime, QDateTime > TimeChartModel::visibleRange() const
{
    return range;
}

void TimeChartModel::setVisibleRange( const QDateTime& start, const QDateTime& end )
{
    const QPair< QDateTime, QDateTime > r = qMakePair( start, end );
    if ( r == range )
        return;

    range = r;
    invalidateFilter();
}

void TimeChartModel::setVisibleStart( const QDateTime& start )
{
    setVisibleRange( start, range.second );
}

void TimeChartModel::setVisibleEnd( const QDateTime& end )
{
    setVisibleRange( range.first, end );
}

/*!
 \reimp
 */
QVariant TimeChartModel::data( const QModelIndex& index, int role ) const
{
    const QVariant v = QSortFilterProxyModel::data( index, role );
    if ( index.column() % 2 != 0 || role != Qt::DisplayRole )
        return v;
    else
        return QDateTime::fromTime_t( 0 ).secsTo( v.toDateTime() ) / 3600.0;
}

/*!
 \reimp
 */
bool TimeChartModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    const QModelIndex index = sourceModel()->index( source_row, 0, source_parent );
    const QDateTime date = index.data().toDateTime();
    return ( date >= range.first || range.first.isNull() ) &&
           ( date <= range.second || range.second.isNull() );
}
