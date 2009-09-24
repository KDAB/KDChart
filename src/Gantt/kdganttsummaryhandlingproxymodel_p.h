/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Gantt library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Gantt licenses may use this file in
 ** accordance with the KD Gantt Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdgantt for
 **   information about KD Gantt Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#ifndef KDGANTTSUMMARYHANDLINGPROXYMODEL_P_H
#define KDGANTTSUMMARYHANDLINGPROXYMODEL_P_H

#include "kdganttsummaryhandlingproxymodel.h"

#include <QDateTime>
#include <QHash>
#include <QPair>
#include <QPersistentModelIndex>

namespace KDGantt {
    class SummaryHandlingProxyModel::Private {
    public:
        bool cacheLookup( const QModelIndex& idx,
                          QPair<QDateTime,QDateTime>* result ) const;
        void insertInCache( const SummaryHandlingProxyModel* model, const QModelIndex& idx ) const;
        void removeFromCache( const QModelIndex& idx ) const;
        void clearCache() const;
		
		inline bool isSummary( const QModelIndex& idx ) const {
			int typ = idx.data( ItemTypeRole ).toInt();
			return (typ==TypeSummary) || (typ==TypeMulti);
		}

        mutable QHash<QModelIndex, QPair<QDateTime, QDateTime> > cached_summary_items;
    };
}

#endif /* KDGANTTSUMMARYHANDLINGPROXYMODEL_P_H */

