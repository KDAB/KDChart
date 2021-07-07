/****************************************************************************
** Copyright (C) 2001-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDGANTTLEGEND_H
#define KDGANTTLEGEND_H

#include <QAbstractItemView>

#include "kdganttglobal.h"
#include "kdganttstyleoptionganttitem.h"

namespace KDGantt 
{
    class KDGANTT_EXPORT Legend : public QAbstractItemView
    {
        Q_OBJECT
        KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC( Legend )
    public:
        explicit Legend( QWidget* parent = nullptr );
        ~Legend() override;

        /*reimp*/ QModelIndex indexAt( const QPoint& point ) const override;
        /*reimp*/ QRect visualRect( const QModelIndex& index ) const override;

        /*reimp*/ void scrollTo( const QModelIndex&, ScrollHint = EnsureVisible ) override {}

        /*reimp*/ QSize sizeHint() const override;
        /*reimp*/ QSize minimumSizeHint() const override;

        /*reimp*/ void setModel( QAbstractItemModel* model ) override;

    protected:
        virtual QRect drawItem( QPainter* painter, const QModelIndex& index, const QPoint& pos = QPoint() ) const;
        virtual QSize measureItem( const QModelIndex& index, bool recursive = true ) const;
        virtual StyleOptionGanttItem getStyleOption( const QModelIndex& index ) const;

        /*reimp*/ void paintEvent( QPaintEvent* event ) override;

        /*reimp*/ int horizontalOffset() const override { return 0; }
        /*reimp*/ bool isIndexHidden( const QModelIndex& ) const override { return false; }
        /*reimp*/ QModelIndex moveCursor( CursorAction, Qt::KeyboardModifiers ) override { return QModelIndex(); }
        /*reimp*/ void setSelection( const QRect&, QItemSelectionModel::SelectionFlags ) override {}
        /*reimp*/ int verticalOffset() const override { return 0; }
        /*reimp*/ QRegion visualRegionForSelection( const QItemSelection& ) const override { return QRegion(); }

    protected Q_SLOTS:
        virtual void modelDataChanged();
    };
}

#endif
