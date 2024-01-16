/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractCartesianDiagram_p.h"

#include <KDABLibFakes>

using namespace KDChart;

AbstractCartesianDiagram::Private::Private()
{
}

AbstractCartesianDiagram::Private::~Private()
{
}

bool AbstractCartesianDiagram::compare(const AbstractCartesianDiagram *other) const
{
    if (other == this)
        return true;
    if (!other) {
        return false;
    }
    return // compare the base class
        (static_cast<const AbstractDiagram *>(this)->compare(other)) &&
        // compare own properties
        (referenceDiagram() == other->referenceDiagram()) && ((!referenceDiagram()) || (referenceDiagramOffset() == other->referenceDiagramOffset()));
}

#define d d_func()

AbstractCartesianDiagram::AbstractCartesianDiagram(QWidget *parent, CartesianCoordinatePlane *plane)
    : AbstractDiagram(new Private(), parent, plane)
{
    init();
}

KDChart::AbstractCartesianDiagram::~AbstractCartesianDiagram()
{
    for (CartesianAxis *axis : qAsConst(d->axesList)) {
        axis->deleteObserver(this);
    }
    d->axesList.clear();
}

void AbstractCartesianDiagram::init()
{
    d->compressor.setModel(attributesModel());
    connect(this, &AbstractCartesianDiagram::layoutChanged,
            &d->compressor, &CartesianDiagramDataCompressor::slotDiagramLayoutChanged);
    connect(this, &AbstractCartesianDiagram::attributesModelAboutToChange,
            this, &AbstractCartesianDiagram::connectAttributesModel);

    if (d->plane) {
        connect(d->plane, &AbstractCoordinatePlane::viewportCoordinateSystemChanged,
                this, &AbstractCartesianDiagram::viewportCoordinateSystemChanged);
    }
}

void AbstractCartesianDiagram::addAxis(CartesianAxis *axis)
{
    if (!d->axesList.contains(axis)) {
        d->axesList.append(axis);
        axis->createObserver(this);
        layoutPlanes();
    }
}

void AbstractCartesianDiagram::takeAxis(CartesianAxis *axis)
{
    const int idx = d->axesList.indexOf(axis);
    if (idx != -1)
        d->axesList.takeAt(idx);
    axis->deleteObserver(this);
    axis->setParentWidget(nullptr);
    layoutPlanes();
}

KDChart::CartesianAxisList AbstractCartesianDiagram::axes() const
{
    return d->axesList;
}

void KDChart::AbstractCartesianDiagram::layoutPlanes()
{
    AbstractCoordinatePlane *plane = coordinatePlane();
    if (plane) {
        plane->layoutPlanes();
    }
}

void KDChart::AbstractCartesianDiagram::setCoordinatePlane(AbstractCoordinatePlane *plane)
{
    if (coordinatePlane()) {
        disconnect(attributesModel(), &AttributesModel::rowsRemoved,
                   coordinatePlane(), &AbstractCoordinatePlane::relayout);
        disconnect(attributesModel(), &AttributesModel::rowsInserted,
                   coordinatePlane(), &AbstractCoordinatePlane::relayout);
        disconnect(attributesModel(), &AttributesModel::columnsRemoved,
                   coordinatePlane(), &AbstractCoordinatePlane::relayout);
        disconnect(attributesModel(), &AttributesModel::columnsInserted,
                   coordinatePlane(), &AbstractCoordinatePlane::relayout);
        disconnect(coordinatePlane());
    }

    AbstractDiagram::setCoordinatePlane(plane);
    if (plane) {
        // Readjust the layout when the dataset count changes
        connect(attributesModel(), &AttributesModel::rowsRemoved,
                plane, &AbstractCoordinatePlane::relayout);
        connect(attributesModel(), &AttributesModel::rowsInserted,
                plane, &AbstractCoordinatePlane::relayout);
        connect(attributesModel(), &AttributesModel::columnsRemoved,
                plane, &AbstractCoordinatePlane::relayout);
        connect(attributesModel(), &AttributesModel::columnsInserted,
                plane, &AbstractCoordinatePlane::relayout);
        connect(plane, &AbstractCoordinatePlane::viewportCoordinateSystemChanged,
                this, &AbstractCartesianDiagram::viewportCoordinateSystemChanged);
        connect(plane, &AbstractCoordinatePlane::viewportCoordinateSystemChanged,
                this, &AbstractCartesianDiagram::update);
    }
}

void AbstractCartesianDiagram::setReferenceDiagram(AbstractCartesianDiagram *diagram, const QPointF &offset)
{
    d->referenceDiagram = diagram;
    d->referenceDiagramOffset = offset;
}

AbstractCartesianDiagram *AbstractCartesianDiagram::referenceDiagram() const
{
    return d->referenceDiagram;
}

QPointF AbstractCartesianDiagram::referenceDiagramOffset() const
{
    return d->referenceDiagramOffset;
}

void AbstractCartesianDiagram::setRootIndex(const QModelIndex &index)
{
    d->compressor.setRootIndex(attributesModel()->mapFromSource(index));
    AbstractDiagram::setRootIndex(index);
}

void AbstractCartesianDiagram::setModel(QAbstractItemModel *m)
{
    if (m == model()) {
        return;
    }
    AbstractDiagram::setModel(m);
}

void AbstractCartesianDiagram::setAttributesModel(AttributesModel *model)
{
    if (model == attributesModel()) {
        return;
    }
    AbstractDiagram::setAttributesModel(model);
}

void AbstractCartesianDiagram::connectAttributesModel(AttributesModel *newModel)
{
    // The compressor must receive model signals before the diagram because the diagram will ask the
    // compressor for data upon receiving dataChanged() et al. from the model, at which point the
    // compressor must be up to date already.
    // Starting from Qt 4.6, the invocation order of slots is guaranteed to be equal to connection
    // order (and probably was before).
    // This is our opportunity to connect to the AttributesModel before the AbstractDiagram does.

    // ### A better design would be to properly recognize that the compressor is the real data interface
    // for Cartesian diagrams and make diagrams listen to updates from the *compressor*, not the model.
    // However, this would change the outside interface of AbstractCartesianDiagram which would be bad.
    // So we're stuck with the complication of this slot and the corresponding signal.
    d->compressor.setModel(newModel);
}
