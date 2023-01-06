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
        connect(m_diagram, SIGNAL(destroyed(QObject *)), SLOT(slotDestroyed(QObject *)));
        connect(m_diagram, SIGNAL(aboutToBeDestroyed()), SLOT(slotAboutToBeDestroyed()));
        connect(m_diagram, SIGNAL(modelsChanged()), SLOT(slotModelsChanged()));
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

    const bool con = connect(m_diagram, SIGNAL(viewportCoordinateSystemChanged()), this, SLOT(slotDataChanged()));
    Q_ASSERT(con);
    Q_UNUSED(con)
    connect(m_diagram, SIGNAL(dataHidden()), SLOT(slotDataHidden()));

    if (m_diagram->model()) {
        connect(m_diagram->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)),
                SLOT(slotDataChanged(QModelIndex, QModelIndex)));
        connect(m_diagram->model(), SIGNAL(rowsInserted(QModelIndex, int, int)),
                SLOT(slotDataChanged()));
        connect(m_diagram->model(), SIGNAL(columnsInserted(QModelIndex, int, int)),
                SLOT(slotDataChanged()));
        connect(m_diagram->model(), SIGNAL(rowsRemoved(QModelIndex, int, int)),
                SLOT(slotDataChanged()));
        connect(m_diagram->model(), SIGNAL(columnsRemoved(QModelIndex, int, int)),
                SLOT(slotDataChanged()));
        connect(m_diagram->model(), SIGNAL(modelReset()),
                SLOT(slotDataChanged()));
        connect(m_diagram->model(), SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
                SLOT(slotHeaderDataChanged(Qt::Orientation, int, int)));
    }

    if (m_diagram->attributesModel())
        connect(m_diagram->attributesModel(), SIGNAL(attributesChanged(QModelIndex, QModelIndex)),
                SLOT(slotAttributesChanged(QModelIndex, QModelIndex)));
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
