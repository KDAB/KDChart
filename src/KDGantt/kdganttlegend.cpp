/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "kdganttlegend.h"
#include "kdganttlegend_p.h"

#include "kdganttitemdelegate.h"

#include <QApplication>
#include <QPainter>

#include <cassert>

using namespace KDGantt;

/*!\class KDGantt::Legend kdganttlegend.h KDGanttLegend
 * \ingroup KDGantt
 * \brief Legend showing an image and a description for Gantt items
 *
 * This is an item view class showing a small Gantt item and it's
 * text defined by LegendRole.
 */

/*! Constructor. Creates a Legend with parent \a parent.
 * The QObject parent is not used for anything internally. */
Legend::Legend(QWidget *parent)
    : QAbstractItemView(parent)
    , _d(new Private)
{
    setItemDelegate(new ItemDelegate(this));
    setFrameStyle(QFrame::NoFrame);
}

/*! Destructor. Does nothing */
Legend::~Legend()
{
    delete _d;
}

#define d d_func()

QModelIndex Legend::indexAt(const QPoint &point) const
{
    Q_UNUSED(point);
    return QModelIndex();
}

QRect Legend::visualRect(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QRect();
}

QSize Legend::sizeHint() const
{
    return measureItem(rootIndex());
}

QSize Legend::minimumSizeHint() const
{
    return measureItem(rootIndex());
}

void Legend::setModel(QAbstractItemModel *newModel)
{
    QAbstractItemModel *oldModel = model();
    if (oldModel != nullptr) {
        disconnect(oldModel, &QAbstractItemModel::dataChanged, this, &Legend::modelDataChanged);
        disconnect(oldModel, &QAbstractItemModel::rowsRemoved, this, &Legend::modelDataChanged);
        disconnect(oldModel, &QAbstractItemModel::columnsRemoved, this, &Legend::modelDataChanged);
    }

    QAbstractItemView::setModel(newModel);
    d->proxyModel.setSourceModel(newModel);

    if (newModel != nullptr) {
        connect(newModel, &QAbstractItemModel::dataChanged, this, &Legend::modelDataChanged);
        connect(newModel, &QAbstractItemModel::rowsRemoved, this, &Legend::modelDataChanged);
        connect(newModel, &QAbstractItemModel::columnsRemoved, this, &Legend::modelDataChanged);
    }
}

/*! Triggers repainting of the legend.
 */
void Legend::modelDataChanged()
{
    updateGeometry();
    viewport()->update();
}

void Legend::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // no model, no legend...
    if (model() == nullptr)
        return;

    QPainter p(viewport());
    p.fillRect(viewport()->rect(), palette().color(QPalette::Window));
    drawItem(&p, rootIndex());
}

/*! Creates a StyleOptionGanttItem with all style options filled in
 *  except the target rectangles.
 */
StyleOptionGanttItem Legend::getStyleOption(const QModelIndex &index) const
{
    StyleOptionGanttItem opt;
    opt.displayPosition = StyleOptionGanttItem::Right;
    opt.displayAlignment = Qt::Alignment(d->proxyModel.data(index, Qt::TextAlignmentRole).toInt());
    opt.text = index.model()->data(index, LegendRole).toString();
    opt.font = (index.model()->data(index, Qt::FontRole)).value<QFont>();
    return opt;
}

/*! Draws the legend item at \a index and all of it's children recursively
 *  at \a pos onto \a painter.
 *  Reimplement this if you want to draw items in an user defined way.
 *  \returns the rectangle drawn.
 */
QRect Legend::drawItem(QPainter *painter, const QModelIndex &index, const QPoint &pos) const
{
    int xPos = pos.x();
    int yPos = pos.y();

    if (index.isValid() && index.model() == &d->proxyModel) {
        auto *const delegate = qobject_cast<ItemDelegate *>(itemDelegate(index));
        assert(delegate != nullptr);
        const QRect r(pos, measureItem(index, false));
        StyleOptionGanttItem opt = getStyleOption(index);
        opt.rect = r;
        opt.rect.setWidth(r.height());

        const ItemType typ = static_cast<ItemType>(index.model()->data(index, ItemTypeRole).toInt());
        const int dx = (typ == TypeEvent) ? (r.height() / 2) : 0;

        opt.itemRect = opt.rect.adjusted(dx, 0, dx, 0);
        opt.boundingRect = r;
        opt.boundingRect.setWidth(r.width() + r.height());
        if (!opt.text.isNull())
            delegate->paintGanttItem(painter, opt, index);

        xPos = r.right();
        yPos = r.bottom();
    }

    const int rowCount = d->proxyModel.rowCount(index);
    for (int row = 0; row < rowCount; ++row) {
        const QRect r = drawItem(painter, d->proxyModel.index(row, 0, index), QPoint(pos.x(), yPos));
        xPos = qMax(xPos, r.right());
        yPos = qMax(yPos, r.bottom());
    }

    return QRect(pos, QPoint(xPos, yPos));
}

/*! Calculates the needed space for the legend item at \a index and, if \a recursive is true,
 *  all child items.
 */
QSize Legend::measureItem(const QModelIndex &index, bool recursive) const
{
    if (model() == nullptr)
        return QSize();

    QSize baseSize;
    if (index.model() != nullptr) {
        QFontMetrics fm((index.model()->data(index, Qt::FontRole)).value<QFont>());
        const QString text = index.model()->data(index, LegendRole).toString();
        if (!text.isEmpty())
            baseSize += QSize(fm.horizontalAdvance(text) + fm.height() + 2, fm.height() + 2);
    }

    if (!recursive)
        return baseSize;

    QSize childrenSize;

    const int rowCount = d->proxyModel.rowCount(index);
    for (int row = 0; row < rowCount; ++row) {
        const QSize childSize = measureItem(d->proxyModel.index(row, 0, index));
        childrenSize.setWidth(qMax(childrenSize.width(), childSize.width()));
        childrenSize.rheight() += childSize.height();
    }
    return baseSize + childrenSize;
}
