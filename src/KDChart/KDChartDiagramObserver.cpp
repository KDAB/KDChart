/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <KDChartAbstractDiagram.h>
#include <KDChartAttributesModel.h>
#include <KDChartDiagramObserver.h>

#include <KDABLibFakes>

#include <QDebug>

using namespace KDChart;

DiagramObserver::DiagramObserver(AbstractDiagram *diagram, QObject *parent)
    : QObject(parent)
    , m_diagram(diagram)
{
    if (m_diagram) {
        connect(m_diagram, &AbstractDiagram::destroyed, this, &DiagramObserver::slotDestroyed);
        connect(m_diagram, &AbstractDiagram::aboutToBeDestroyed, this, &DiagramObserver::slotAboutToBeDestroyed);
        connect(m_diagram, &AbstractDiagram::modelsChanged, this, &DiagramObserver::slotModelsChanged);
    }
    init();
}

DiagramObserver::~DiagramObserver()
{
}

const AbstractDiagram *DiagramObserver::diagram() const
{
    return m_diagram;
}

AbstractDiagram *DiagramObserver::diagram()
{
    return m_diagram;
}

void DiagramObserver::init()
{
    if (!m_diagram)
        return;

    if (m_model)
        disconnect(m_model);

    if (m_attributesmodel)
        disconnect(m_attributesmodel);

    const bool con = connect(m_diagram, &AbstractDiagram::viewportCoordinateSystemChanged,
                             this, qOverload<>(&DiagramObserver::slotDataChanged));
    Q_ASSERT(con);
    Q_UNUSED(con)
    connect(m_diagram, &AbstractDiagram::dataHidden, this, &DiagramObserver::slotDataHidden);

    if (m_diagram->model()) {
        connect(m_diagram->model(), &QAbstractItemModel::dataChanged,
                this, qOverload<QModelIndex, QModelIndex>(&DiagramObserver::slotDataChanged));
        connect(m_diagram->model(), &QAbstractItemModel::rowsInserted,
                this, qOverload<>(&DiagramObserver::slotDataChanged));
        connect(m_diagram->model(), &QAbstractItemModel::columnsInserted,
                this, qOverload<>(&DiagramObserver::slotDataChanged));
        connect(m_diagram->model(), &QAbstractItemModel::rowsRemoved,
                this, qOverload<>(&DiagramObserver::slotDataChanged));
        connect(m_diagram->model(), &QAbstractItemModel::columnsRemoved,
                this, qOverload<>(&DiagramObserver::slotDataChanged));
        connect(m_diagram->model(), &QAbstractItemModel::modelReset,
                this, qOverload<>(&DiagramObserver::slotDataChanged));
        connect(m_diagram->model(), &QAbstractItemModel::headerDataChanged,
                this, &DiagramObserver::slotHeaderDataChanged);
    }

    if (m_diagram->attributesModel())
        connect(m_diagram->attributesModel(), &AttributesModel::attributesChanged,
                this, qOverload<QModelIndex, QModelIndex>(&DiagramObserver::slotAttributesChanged));
    m_model = m_diagram->model();
    m_attributesmodel = m_diagram->attributesModel();
}

void DiagramObserver::slotDestroyed(QObject *)
{
    // qDebug() << this << "emits signal\n"
    //         "    emit diagramDestroyed(" <<  m_diagram << ")";
    AbstractDiagram *diag = m_diagram;
    disconnect(m_diagram, nullptr, this, nullptr);
    m_diagram = nullptr;
    emit diagramDestroyed(diag);
}

void DiagramObserver::slotAboutToBeDestroyed()
{
    emit diagramAboutToBeDestroyed(m_diagram);
}

void DiagramObserver::slotModelsChanged()
{
    init();
    slotDataChanged();
    slotAttributesChanged();
}

void DiagramObserver::slotHeaderDataChanged(Qt::Orientation, int, int)
{
    // qDebug() << "DiagramObserver::slotHeaderDataChanged()";
    emit diagramDataChanged(m_diagram);
}

void DiagramObserver::slotDataChanged(QModelIndex, QModelIndex)
{
    slotDataChanged();
}

void DiagramObserver::slotDataChanged()
{
    // qDebug() << "DiagramObserver::slotDataChanged()";
    emit diagramDataChanged(m_diagram);
}

void DiagramObserver::slotDataHidden()
{
    // qDebug() << "DiagramObserver::slotDataHidden()";
    emit diagramDataHidden(m_diagram);
}

void DiagramObserver::slotAttributesChanged(QModelIndex, QModelIndex)
{
    slotAttributesChanged();
}

void DiagramObserver::slotAttributesChanged()
{
    // qDebug() << "DiagramObserver::slotAttributesChanged()";
    emit diagramAttributesChanged(m_diagram);
}
