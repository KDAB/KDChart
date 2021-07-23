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

#ifndef KDGANTTCONSTRAINTPROXY_H
#define KDGANTTCONSTRAINTPROXY_H

#include "kdganttglobal.h"

#include <QPointer>

QT_BEGIN_NAMESPACE
class QAbstractProxyModel;
QT_END_NAMESPACE

namespace KDGantt {
class Constraint;
class ConstraintModel;

class KDGANTT_EXPORT ConstraintProxy : public QObject
{
    Q_OBJECT
public:
    explicit ConstraintProxy(QObject *parent = nullptr);
    ~ConstraintProxy() override;

    void setSourceModel(ConstraintModel *src);
    void setDestinationModel(ConstraintModel *dest);
    void setProxyModel(QAbstractProxyModel *proxy);

    ConstraintModel *sourceModel() const;
    ConstraintModel *destinationModel() const;
    QAbstractProxyModel *proxyModel() const;

private Q_SLOTS:

    void slotSourceConstraintAdded(const KDGantt::Constraint &);
    void slotSourceConstraintRemoved(const KDGantt::Constraint &);

    void slotDestinationConstraintAdded(const KDGantt::Constraint &);
    void slotDestinationConstraintRemoved(const KDGantt::Constraint &);

    void slotLayoutChanged();

private:
    void copyFromSource();

    QPointer<QAbstractProxyModel> m_proxy;
    QPointer<ConstraintModel> m_source;
    QPointer<ConstraintModel> m_destination;
};
}

#endif /* KDGANTTCONSTRAINTPROXY_H */
