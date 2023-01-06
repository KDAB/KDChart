/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDGANTTLEGEND_H
#define KDGANTTLEGEND_H

#include <QAbstractItemView>

#include "kdganttglobal.h"
#include "kdganttstyleoptionganttitem.h"

namespace KDGantt {
class KDGANTT_EXPORT Legend : public QAbstractItemView
{
    Q_OBJECT
    KDGANTT_DECLARE_PRIVATE_BASE_POLYMORPHIC(Legend)
public:
    explicit Legend(QWidget *parent = nullptr);
    ~Legend() override;

    /*reimp*/ QModelIndex indexAt(const QPoint &point) const override;
    /*reimp*/ QRect visualRect(const QModelIndex &index) const override;

    /*reimp*/ void scrollTo(const QModelIndex &, ScrollHint = EnsureVisible) override
    {
    }

    /*reimp*/ QSize sizeHint() const override;
    /*reimp*/ QSize minimumSizeHint() const override;

    /*reimp*/ void setModel(QAbstractItemModel *model) override;

protected:
    virtual QRect drawItem(QPainter *painter, const QModelIndex &index, const QPoint &pos = QPoint()) const;
    virtual QSize measureItem(const QModelIndex &index, bool recursive = true) const;
    virtual StyleOptionGanttItem getStyleOption(const QModelIndex &index) const;

    /*reimp*/ void paintEvent(QPaintEvent *event) override;

    /*reimp*/ int horizontalOffset() const override
    {
        return 0;
    }
    /*reimp*/ bool isIndexHidden(const QModelIndex &) const override
    {
        return false;
    }
    /*reimp*/ QModelIndex moveCursor(CursorAction, Qt::KeyboardModifiers) override
    {
        return QModelIndex();
    }
    /*reimp*/ void setSelection(const QRect &, QItemSelectionModel::SelectionFlags) override
    {
    }
    /*reimp*/ int verticalOffset() const override
    {
        return 0;
    }
    /*reimp*/ QRegion visualRegionForSelection(const QItemSelection &) const override
    {
        return QRegion();
    }

protected Q_SLOTS:
    virtual void modelDataChanged();
};
}

#endif
