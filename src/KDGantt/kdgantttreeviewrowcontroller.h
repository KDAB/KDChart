/****************************************************************************
** Copyright (C) 2001-2019 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDGANTTTREEVIEWROWCONTROLLER_H
#define KDGANTTTREEVIEWROWCONTROLLER_H

#include "kdganttabstractrowcontroller.h"

QT_BEGIN_NAMESPACE
class QAbstractProxyModel;
class QTreeView;
QT_END_NAMESPACE

namespace KDGantt {
    class KDGANTT_EXPORT TreeViewRowController :  public AbstractRowController {
        KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(TreeViewRowController)
    public:
        TreeViewRowController( QTreeView* tv, QAbstractProxyModel* proxy );
        ~TreeViewRowController() override;

        /*reimp*/ int headerHeight() const override;
        /*reimp*/ int maximumItemHeight() const override;
        /*reimp*/ int totalHeight() const override;
        /*reimp*/ bool isRowVisible( const QModelIndex& idx ) const override;
        /*reimp*/ bool isRowExpanded( const QModelIndex& idx ) const override;
        /*reimp*/ Span rowGeometry( const QModelIndex& idx ) const override;
        /*reimp*/ QModelIndex indexAt( int height ) const override;
        /*reimp*/ QModelIndex indexAbove( const QModelIndex& idx ) const override;
        /*reimp*/ QModelIndex indexBelow( const QModelIndex& idx ) const override;
    };
}

#endif /* KDGANTTTREEVIEWROWCONTROLLER_H */

