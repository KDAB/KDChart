/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#ifndef ENTRYDELEGATE_H
#define ENTRYDELEGATE_H

#include <QItemDelegate>

namespace KDGantt {
class ConstraintModel;
}

class EntryDelegate : public QItemDelegate
{
public:
    EntryDelegate(KDGantt::ConstraintModel *constraintModel, QObject *parent = 0);

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    void addConstraint(const QModelIndex &index1, const QModelIndex &index2);
    void setReadOnly(const QModelIndex &index, bool readOnly);

    KDGantt::ConstraintModel *constraintModel;
};

#endif /* ENTRYDELEGATE_H */
