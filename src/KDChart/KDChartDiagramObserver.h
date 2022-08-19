/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#ifndef __KDCHARTDIAGRAMOBSERVER_H_
#define __KDCHARTDIAGRAMOBSERVER_H_

#include "KDChartGlobal.h"

#include <QModelIndex>
#include <QObject>
#include <QPointer>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

namespace KDChart {

class AbstractDiagram;

/**
 * \brief A DiagramObserver watches the associated diagram for
 * changes and deletion and emits corresponding signals.
 */
class KDCHART_EXPORT DiagramObserver : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructs a new observer observing the given diagram.
     */
    explicit DiagramObserver(AbstractDiagram *diagram, QObject *parent = nullptr);
    ~DiagramObserver() override;

    const AbstractDiagram *diagram() const;
    AbstractDiagram *diagram();

Q_SIGNALS:
    /** This signal is emitted immediately before the diagram is
     * being destroyed. */
    void diagramDestroyed(AbstractDiagram *diagram);
    /** Emitted when a diagram is being destroyed, but before its data is invalidated **/
    void diagramAboutToBeDestroyed(AbstractDiagram *diagram);
    /** This signal is emitted whenever the data of the diagram changes. */
    void diagramDataChanged(AbstractDiagram *diagram);
    /** This signal is emitted whenever any of the data of the diagram was set (un)hidden. */
    void diagramDataHidden(AbstractDiagram *diagram);
    /** This signal is emitted whenever the attributes of the diagram change. */
    void diagramAttributesChanged(AbstractDiagram *diagram);

private Q_SLOTS:
    void slotDestroyed(QObject *);
    void slotAboutToBeDestroyed();
    void slotHeaderDataChanged(Qt::Orientation, int, int);
    void slotDataChanged(QModelIndex, QModelIndex);
    void slotDataChanged();
    void slotDataHidden();
    void slotAttributesChanged();
    void slotAttributesChanged(QModelIndex, QModelIndex);
    void slotModelsChanged();

private:
    void init();

    AbstractDiagram *m_diagram;
    QPointer<QAbstractItemModel> m_model;
    QPointer<QAbstractItemModel> m_attributesmodel;
};
}

#endif // KDChartDiagramObserver_H
