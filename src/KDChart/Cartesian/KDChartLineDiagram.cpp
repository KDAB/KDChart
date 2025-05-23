/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartLineDiagram.h"
#include "KDChartLineDiagram_p.h"

#include "KDChartAbstractGrid.h"
#include "KDChartAttributesModel.h"
#include "KDChartBarDiagram.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartPalette.h"

#include <KDABLibFakes>

#include "KDChartNormalLineDiagram_p.h"
#include "KDChartPercentLineDiagram_p.h"
#include "KDChartStackedLineDiagram_p.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QString>
#include <QVector>

using namespace KDChart;

LineDiagram::Private::Private()
{
}

LineDiagram::Private::~Private()
{
}

#define d d_func()

LineDiagram::LineDiagram(QWidget *parent, CartesianCoordinatePlane *plane)
    : AbstractCartesianDiagram(new Private(), parent, plane)
{
    init();
}

void LineDiagram::init()
{
    d->normalDiagram = new NormalLineDiagram(this);
    d->stackedDiagram = new StackedLineDiagram(this);
    d->percentDiagram = new PercentLineDiagram(this);
    d->implementor = d->normalDiagram;
    d->centerDataPoints = false;
    d->reverseDatasetOrder = false;
}

LineDiagram::~LineDiagram()
{
    delete d->normalDiagram;
    delete d->stackedDiagram;
    delete d->percentDiagram;
}

/**
 * Creates an exact copy of this diagram.
 */
LineDiagram *LineDiagram::clone() const
{
    auto *newDiagram = new LineDiagram(new Private(*d));
    newDiagram->setType(type());
    return newDiagram;
}

bool LineDiagram::compare(const LineDiagram *other) const
{
    if (other == this)
        return true;
    if (!other) {
        return false;
    }
    return // compare the base class
        (static_cast<const AbstractCartesianDiagram *>(this)->compare(other)) &&
        // compare own properties
        (type() == other->type()) && (centerDataPoints() == other->centerDataPoints()) && (reverseDatasetOrder() == other->reverseDatasetOrder());
}

/**
 * Sets the line diagram's type to \a type
 * \sa LineDiagram::LineType
 */
void LineDiagram::setType(const LineType type)
{
    if (d->implementor->type() == type)
        return;
    if (type != LineDiagram::Normal && datasetDimension() > 1) {
        Q_ASSERT_X(false, "setType()",
                   "This line chart type can't be used with multi-dimensional data.");
        return;
    }
    switch (type) {
    case Normal:
        d->implementor = d->normalDiagram;
        break;
    case Stacked:
        d->implementor = d->stackedDiagram;
        break;
    case Percent:
        d->implementor = d->percentDiagram;
        break;
    default:
        Q_ASSERT_X(false, "LineDiagram::setType", "unknown diagram subtype");
    };

    // d->lineType = type;
    Q_ASSERT(d->implementor->type() == type);

    // AbstractAxis settings - see AbstractDiagram and CartesianAxis
    setPercentMode(type == LineDiagram::Percent);
    setDataBoundariesDirty();
    Q_EMIT layoutChanged(this);
    Q_EMIT propertiesChanged();
}

/**
 * @return the type of the line diagram
 */
LineDiagram::LineType LineDiagram::type() const
{
    return d->implementor->type();
}

void LineDiagram::setCenterDataPoints(bool center)
{
    if (d->centerDataPoints == center) {
        return;
    }

    d->centerDataPoints = center;
    // The actual data boundaries haven't changed, but the axis will have one more or less tick
    //  A      B    =\        A      B
    //  1......2    =/    1......2......3
    setDataBoundariesDirty();
    Q_EMIT layoutChanged(this);
    Q_EMIT propertiesChanged();
}

bool LineDiagram::centerDataPoints() const
{
    return d->centerDataPoints;
}

void LineDiagram::setReverseDatasetOrder(bool reverse)
{
    d->reverseDatasetOrder = reverse;
}

bool LineDiagram::reverseDatasetOrder() const
{
    return d->reverseDatasetOrder;
}

/**
 * Sets the global line attributes to \a la
 */
void LineDiagram::setLineAttributes(const LineAttributes &la)
{
    d->attributesModel->setModelData(
        QVariant::fromValue(la),
        LineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * Sets the line attributes of data set \a column to \a la
 */
void LineDiagram::setLineAttributes(
    int column,
    const LineAttributes &la)
{
    d->setDatasetAttrs(column, QVariant::fromValue(la), LineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * Resets the line attributes of data set \a column
 */
void LineDiagram::resetLineAttributes(int column)
{
    d->resetDatasetAttrs(column, LineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * Sets the line attributes for the model index \a index to \a la
 */
void LineDiagram::setLineAttributes(
    const QModelIndex &index,
    const LineAttributes &la)
{
    d->attributesModel->setData(
        d->attributesModel->mapFromSource(index),
        QVariant::fromValue(la),
        LineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * Remove any explicit line attributes settings that might have been specified before.
 */
void LineDiagram::resetLineAttributes(const QModelIndex &index)
{
    d->attributesModel->resetData(
        d->attributesModel->mapFromSource(index), LineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * @return the global line attribute set
 */
LineAttributes LineDiagram::lineAttributes() const
{
    return d->attributesModel->data(KDChart::LineAttributesRole).value<LineAttributes>();
}

/**
 * @return the line attribute set of data set \a column
 */
LineAttributes LineDiagram::lineAttributes(int column) const
{
    const QVariant attrs(d->datasetAttrs(column, LineAttributesRole));
    if (attrs.isValid())
        return attrs.value<LineAttributes>();
    return lineAttributes();
}

/**
 * @return the line attribute set of the model index \a index
 */
LineAttributes LineDiagram::lineAttributes(
    const QModelIndex &index) const
{
    return d->attributesModel->data(
                                 d->attributesModel->mapFromSource(index),
                                 KDChart::LineAttributesRole)
        .value<LineAttributes>();
}

/**
 * Sets the global 3D line attributes to \a la
 */
void LineDiagram::setThreeDLineAttributes(
    const ThreeDLineAttributes &la)
{
    setDataBoundariesDirty();
    d->attributesModel->setModelData(
        QVariant::fromValue(la),
        ThreeDLineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * Sets the 3D line attributes of data set \a column to \a ta
 */
void LineDiagram::setThreeDLineAttributes(
    int column,
    const ThreeDLineAttributes &la)
{
    setDataBoundariesDirty();
    d->setDatasetAttrs(column, QVariant::fromValue(la), ThreeDLineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * Sets the 3D line attributes of model index \a index to \a la
 */
void LineDiagram::setThreeDLineAttributes(
    const QModelIndex &index,
    const ThreeDLineAttributes &la)
{
    setDataBoundariesDirty();
    d->attributesModel->setData(
        d->attributesModel->mapFromSource(index),
        QVariant::fromValue(la),
        ThreeDLineAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * @return the global 3D line attributes
 */
ThreeDLineAttributes LineDiagram::threeDLineAttributes() const
{
    return d->attributesModel->data(KDChart::ThreeDLineAttributesRole).value<ThreeDLineAttributes>();
}

/**
 * @return the 3D line attributes of data set \a column
 */
ThreeDLineAttributes LineDiagram::threeDLineAttributes(int column) const
{
    const QVariant attrs(d->datasetAttrs(column, ThreeDLineAttributesRole));
    if (attrs.isValid())
        return attrs.value<ThreeDLineAttributes>();
    return threeDLineAttributes();
}

/**
 * @return the 3D line attributes of the model index \a index
 */
ThreeDLineAttributes LineDiagram::threeDLineAttributes(const QModelIndex &index) const
{
    return d->attributesModel->data(
                                 d->attributesModel->mapFromSource(index),
                                 KDChart::ThreeDLineAttributesRole)
        .value<ThreeDLineAttributes>();
}

qreal LineDiagram::threeDItemDepth(const QModelIndex &index) const
{
    return threeDLineAttributes(index).validDepth();
}

qreal LineDiagram::threeDItemDepth(int column) const
{
    return threeDLineAttributes(column).validDepth();
}

/**
 * Sets the value tracker attributes of the model index \a index to \a va
 */
void LineDiagram::setValueTrackerAttributes(const QModelIndex &index,
                                            const ValueTrackerAttributes &va)
{
    d->attributesModel->setData(d->attributesModel->mapFromSource(index),
                                QVariant::fromValue(va),
                                KDChart::ValueTrackerAttributesRole);
    Q_EMIT propertiesChanged();
}

/**
 * Returns the value tracker attributes of the model index \a index
 */
ValueTrackerAttributes LineDiagram::valueTrackerAttributes(
    const QModelIndex &index) const
{
    return d->attributesModel->data(
                                 d->attributesModel->mapFromSource(index),
                                 KDChart::ValueTrackerAttributesRole)
        .value<ValueTrackerAttributes>();
}

/**
 * Returns whether the lines are drawn smoothed
 */
qreal LineDiagram::lineTension() const
{
    return d->tension;
}

void LineDiagram::setLineTension(qreal tension)
{
    d->tension = tension;
    Q_EMIT propertiesChanged();
}

void LineDiagram::resizeEvent(QResizeEvent *)
{
}

const QPair<QPointF, QPointF> LineDiagram::calculateDataBoundaries() const
{
    d->compressor.setResolution(static_cast<int>(this->size().width() * coordinatePlane()->zoomFactorX()),
                                static_cast<int>(this->size().height() * coordinatePlane()->zoomFactorY()));

    if (!checkInvariants(true))
        return QPair<QPointF, QPointF>(QPointF(0, 0), QPointF(0, 0));

    // note: calculateDataBoundaries() is ignoring the hidden flags.
    //       That's not a bug but a feature: Hiding data does not mean removing them.
    // For totally removing data from KD Chart's view people can use e.g. a proxy model ...

    // calculate boundaries for different line types Normal - Stacked - Percent - Default Normal
    return d->implementor->calculateDataBoundaries();
}

void LineDiagram::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());
    PaintContext ctx;
    ctx.setPainter(&painter);
    ctx.setRectangle(QRectF(0, 0, width(), height()));
    paint(&ctx);
}

void LineDiagram::paint(PaintContext *ctx)
{
    // note: Not having any data model assigned is no bug
    //       but we can not draw a diagram then either.
    if (!checkInvariants(true))
        return;
    if (!AbstractGrid::isBoundariesValid(dataBoundaries()))
        return;
    const PainterSaver p(ctx->painter());
    if (model()->rowCount(rootIndex()) == 0 || model()->columnCount(rootIndex()) == 0)
        return; // nothing to paint for us

    AbstractCoordinatePlane *const plane = ctx->coordinatePlane();
    ctx->setCoordinatePlane(plane->sharedAxisMasterPlane(ctx->painter()));

    // paint different line types Normal - Stacked - Percent - Default Normal
    d->implementor->paint(ctx);

    ctx->setCoordinatePlane(plane);
}

void LineDiagram::resize(const QSizeF &size)
{
    d->compressor.setResolution(static_cast<int>(size.width() * coordinatePlane()->zoomFactorX()),
                                static_cast<int>(size.height() * coordinatePlane()->zoomFactorY()));
    setDataBoundariesDirty();
    QAbstractItemView::resize(size.toSize());
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(Q_COMPILER_MANGLES_RETURN_TYPE)
const
#endif
    int
    LineDiagram::numberOfAbscissaSegments() const
{
    return d->attributesModel->rowCount(attributesModelRootIndex());
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(Q_COMPILER_MANGLES_RETURN_TYPE)
const
#endif
    int
    LineDiagram::numberOfOrdinateSegments() const
{
    return d->attributesModel->columnCount(attributesModelRootIndex());
}
