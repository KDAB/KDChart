/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartAbstractPieDiagram.h"
#include "KDChartAbstractPieDiagram_p.h"

#include "KDChartAttributesModel.h"
#include "KDChartPieAttributes.h"
#include "KDChartThreeDPieAttributes.h"

#include <QMap>

#include <KDABLibFakes>

using namespace KDChart;

AbstractPieDiagram::Private::Private()
{
}

AbstractPieDiagram::Private::~Private()
{
}

AbstractPieDiagram::AbstractPieDiagram(QWidget *parent, PolarCoordinatePlane *plane)
    : AbstractPolarDiagram(new Private(), parent, plane)
{
    init();
}

AbstractPieDiagram::~AbstractPieDiagram()
{
}

void AbstractPieDiagram::init()
{
}

bool AbstractPieDiagram::compare(const AbstractPieDiagram *other) const
{
    if (other == this)
        return true;
    if (!other) {
        // qDebug() << "AbstractPieDiagram::compare() cannot compare to Null pointer";
        return false;
    }
    /*
    qDebug() << "\n             AbstractPieDiagram::compare():";
            // compare own properties
    qDebug() <<
            (granularity() == other->granularity()) &&
            (startPosition() == other->startPosition());
    */
    return // compare the base class
        (static_cast<const AbstractPolarDiagram *>(this)->compare(other)) &&
        // compare own properties
        (granularity() == other->granularity()) && (startPosition() == other->startPosition());
}

#define d d_func()

void AbstractPieDiagram::setGranularity(qreal value)
{
    d->granularity = value;
}

qreal AbstractPieDiagram::granularity() const
{
    return (d->granularity < 0.05 || d->granularity > 36.0)
        ? 1.0
        : d->granularity;
}

void AbstractPieDiagram::setStartPosition(int degrees)
{
    Q_UNUSED(degrees);
    qWarning() << "Deprecated AbstractPieDiagram::setStartPosition() called, setting ignored.";
}

int AbstractPieDiagram::startPosition() const
{
    qWarning() << "Deprecated AbstractPieDiagram::startPosition() called.";
    return 0;
}

void AbstractPieDiagram::setAutoRotateLabels(bool autoRotate)
{
    d->autoRotateLabels = autoRotate;
}

bool AbstractPieDiagram::autoRotateLabels() const
{
    return d->autoRotateLabels;
}

void AbstractPieDiagram::setPieAttributes(const PieAttributes &attrs)
{
    d->attributesModel->setModelData(QVariant::fromValue(attrs), PieAttributesRole);
    emit layoutChanged(this);
}

void AbstractPieDiagram::setPieAttributes(int column, const PieAttributes &attrs)
{
    d->setDatasetAttrs(column, QVariant::fromValue(attrs), PieAttributesRole);
    emit layoutChanged(this);
}

void AbstractPieDiagram::setPieAttributes(const QModelIndex &index, const PieAttributes &attrs)
{
    d->attributesModel->setData(index, QVariant::fromValue(attrs), PieAttributesRole);
    emit layoutChanged(this);
}

PieAttributes AbstractPieDiagram::pieAttributes() const
{
    return d->attributesModel->data(PieAttributesRole).value<PieAttributes>();
}

PieAttributes AbstractPieDiagram::pieAttributes(int column) const
{
    const QVariant attrs(d->datasetAttrs(column, PieAttributesRole));
    if (attrs.isValid())
        return attrs.value<PieAttributes>();
    return pieAttributes();
}

PieAttributes AbstractPieDiagram::pieAttributes(const QModelIndex &index) const
{
    return d->attributesModel->data(
                                 d->attributesModel->mapFromSource(index),
                                 PieAttributesRole)
        .value<PieAttributes>();
}

void AbstractPieDiagram::setThreeDPieAttributes(const ThreeDPieAttributes &tda)
{
    d->attributesModel->setModelData(QVariant::fromValue(tda), ThreeDPieAttributesRole);
    emit layoutChanged(this);
}

void AbstractPieDiagram::setThreeDPieAttributes(int column, const ThreeDPieAttributes &tda)
{
    d->setDatasetAttrs(column, QVariant::fromValue(tda), ThreeDPieAttributesRole);
    emit layoutChanged(this);
}

void AbstractPieDiagram::setThreeDPieAttributes(const QModelIndex &index, const ThreeDPieAttributes &tda)
{
    model()->setData(index, QVariant::fromValue(tda), ThreeDPieAttributesRole);
    emit layoutChanged(this);
}

ThreeDPieAttributes AbstractPieDiagram::threeDPieAttributes() const
{
    return d->attributesModel->data(ThreeDPieAttributesRole).value<ThreeDPieAttributes>();
}

ThreeDPieAttributes AbstractPieDiagram::threeDPieAttributes(int column) const
{
    const QVariant attrs(d->datasetAttrs(column, ThreeDPieAttributesRole));
    if (attrs.isValid())
        return attrs.value<ThreeDPieAttributes>();
    return threeDPieAttributes();
}

ThreeDPieAttributes AbstractPieDiagram::threeDPieAttributes(const QModelIndex &index) const
{
    return d->attributesModel->data(
                                 d->attributesModel->mapFromSource(index),
                                 ThreeDPieAttributesRole)
        .value<ThreeDPieAttributes>();
}
