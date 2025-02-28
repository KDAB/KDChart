/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartCartesianAxis.h"
#include "KDChartCartesianAxis_p.h"

#include <cmath>

#include <QApplication>
#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QtDebug>

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractDiagram_p.h"
#include "KDChartAbstractGrid.h"
#include "KDChartBarDiagram.h"
#include "KDChartChart.h"
#include "KDChartLayoutItems.h"
#include "KDChartLineDiagram.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartPrintingParameters.h"
#include "KDChartStockDiagram.h"

#include <KDABLibFakes>

using namespace KDChart;

#define d (d_func())

static qreal slightlyLessThan(qreal r)
{
    if (r == 0.0) {
        // scale down the epsilon somewhat arbitrarily
        return r - std::numeric_limits<qreal>::epsilon() * 1e-6;
    }
    // scale the epsilon so that it (hopefully) changes at least the least significant bit of r
    qreal diff = qAbs(r) * std::numeric_limits<qreal>::epsilon() * 2.0;
    return r - diff;
}

static int numSignificantDecimalPlaces(qreal floatNumber)
{
    static const int maxPlaces = 15;
    QString sample = QString::number(floatNumber, 'f', maxPlaces).section(QLatin1Char('.'), 1, 2);
    int ret = maxPlaces;
    for (; ret > 0; ret--) {
        if (sample[ret - 1] != QLatin1Char('0')) {
            break;
        }
    }
    return ret;
}

// Feature idea: In case of numeric labels, consider limiting the possible values of majorThinningFactor
// to something like {1, 2, 5} * 10^n. Or even better, something that achieves round values in the
// remaining labels.

TickIterator::TickIterator(CartesianAxis *a, CartesianCoordinatePlane *plane, uint majorThinningFactor,
                           bool omitLastTick)
    : m_axis(a)
    , m_majorThinningFactor(majorThinningFactor)
    , m_majorLabelCount(0)
    , m_type(NoTick)
{
    // deal with the things that are specific to axes (like annotations), before the generic init().
    const CartesianAxis::Private *axisPriv = CartesianAxis::Private::get(a);
    XySwitch xy(axisPriv->isVertical());
    m_dimension = xy(plane->gridDimensionsList().first(), plane->gridDimensionsList().last());
    if (omitLastTick) {
        // In bar and stock charts the last X tick is a fencepost with no associated value, which is
        // convenient for grid painting. Here we have to manually exclude it to avoid overpainting.
        m_dimension.end -= m_dimension.stepWidth;
    }

    m_annotations = axisPriv->annotations;
    m_customTicks = axisPriv->customTicksPositions;

    const qreal inf = std::numeric_limits<qreal>::infinity();

    if (m_customTicks.count()) {
        std::sort(m_customTicks.begin(), m_customTicks.end());
        m_customTickIndex = 0;
        m_customTick = m_customTicks.at(m_customTickIndex);
    } else {
        m_customTickIndex = -1;
        m_customTick = inf;
    }

    if (m_majorThinningFactor > 1 && hasShorterLabels()) {
        m_manualLabelTexts = m_axis->shortLabels();
    } else {
        m_manualLabelTexts = m_axis->labels();
    }
    m_manualLabelIndex = m_manualLabelTexts.isEmpty() ? -1 : 0;

    if (!m_dimension.isCalculated) {
        // ### depending on the data, it is difficult to impossible to choose anchors (where ticks
        //     corresponding to the header labels are) on the ordinate or even the abscissa with
        //     2-dimensional data. this should be somewhat mitigated by isCalculated only being false
        //     when header data labels should work, at least that seems to be what the code that sets up
        //     the dimensions is trying to do.
        QStringList dataHeaderLabels;
        AbstractDiagram *const dia = plane->diagram();
        dataHeaderLabels = dia->itemRowLabels();
        if (!dataHeaderLabels.isEmpty()) {
            AttributesModel *model = dia->attributesModel();
            const int anchorCount = model->rowCount(QModelIndex());
            if (anchorCount == dataHeaderLabels.count()) {
                for (int i = 0; i < anchorCount; i++) {
                    // ### ordinal number as anchor point generally only works for 1-dimensional data
                    m_dataHeaderLabels.insert(qreal(i), dataHeaderLabels.at(i));
                }
            }
        }
    }

    bool hasMajorTicks = m_axis->rulerAttributes().showMajorTickMarks();
    bool hasMinorTicks = m_axis->rulerAttributes().showMinorTickMarks();

    init(xy.isY, hasMajorTicks, hasMinorTicks, plane);
}

static QMultiMap<qreal, QString> allAxisAnnotations(const AbstractCoordinatePlane *plane, bool isY)
{
    QMultiMap<qreal, QString> annotations;
    const auto constDiagrams = plane->diagrams();
    for (const AbstractDiagram *diagram : constDiagrams) {
        const auto *cd = qobject_cast<const AbstractCartesianDiagram *>(diagram);
        if (!cd) {
            continue;
        }
        const auto axes = cd->axes();
        for (const CartesianAxis *axis : axes) {
            const CartesianAxis::Private *axisPriv = CartesianAxis::Private::get(axis);
            if (axisPriv->isVertical() == isY) {
                annotations.unite(axisPriv->annotations);
            }
        }
    }
    return annotations;
}

TickIterator::TickIterator(bool isY, const DataDimension &dimension, bool useAnnotationsForTicks,
                           bool hasMajorTicks, bool hasMinorTicks, CartesianCoordinatePlane *plane)
    : m_axis(nullptr)
    , m_dimension(dimension)
    , m_majorThinningFactor(1)
    , m_majorLabelCount(0)
    , m_customTickIndex(-1)
    , m_manualLabelIndex(-1)
    , m_type(NoTick)
    , m_customTick(std::numeric_limits<qreal>::infinity())
{
    if (useAnnotationsForTicks) {
        m_annotations = allAxisAnnotations(plane, isY);
    }
    init(isY, hasMajorTicks, hasMinorTicks, plane);
}

void TickIterator::init(bool isY, bool hasMajorTicks, bool hasMinorTicks,
                        CartesianCoordinatePlane *plane)
{
    Q_ASSERT(std::numeric_limits<qreal>::has_infinity);

    m_isLogarithmic = m_dimension.calcMode == AbstractCoordinatePlane::Logarithmic;
    // sanity check against infinite loops
    hasMajorTicks = hasMajorTicks && (m_dimension.stepWidth > 0 || m_isLogarithmic);
    hasMinorTicks = hasMinorTicks && (m_dimension.subStepWidth > 0 || m_isLogarithmic);

    XySwitch xy(isY);

    GridAttributes gridAttributes = plane->gridAttributes(xy(Qt::Horizontal, Qt::Vertical));
    m_isLogarithmic = m_dimension.calcMode == AbstractCoordinatePlane::Logarithmic;
    if (!m_isLogarithmic) {
        // adjustedLowerUpperRange() is intended for use with linear scaling; specifically it would
        // round lower bounds < 1 to 0.

        const bool fixedRange = xy(plane->autoAdjustHorizontalRangeToData(),
                                   plane->autoAdjustVerticalRangeToData())
            >= 100;
        const bool adjustLower = gridAttributes.adjustLowerBoundToGrid() && !fixedRange;
        const bool adjustUpper = gridAttributes.adjustUpperBoundToGrid() && !fixedRange;
        m_dimension = AbstractGrid::adjustedLowerUpperRange(m_dimension, adjustLower, adjustUpper);

        m_decimalPlaces = numSignificantDecimalPlaces(m_dimension.stepWidth);
    } else {
        // the number of significant decimal places for each label naturally varies with logarithmic scaling
        m_decimalPlaces = -1;
    }

    const qreal inf = std::numeric_limits<qreal>::infinity();

    // try to place m_position just in front of the first tick to be drawn so that operator++()
    // can be used to find the first tick
    if (m_isLogarithmic) {
        if (ISNAN(m_dimension.start) || ISNAN(m_dimension.end)) {
            // this can happen in a spurious paint operation before everything is set up;
            // just bail out to avoid an infinite loop in that case.
            m_dimension.start = 0.0;
            m_dimension.end = 0.0;
            m_position = inf;
            m_majorTick = inf;
            m_minorTick = inf;
        } else if (m_dimension.start >= 0) {
            m_position = m_dimension.start ? pow(10.0, floor(log10(m_dimension.start)) - 1.0)
                                           : 1e-6;
            m_majorTick = hasMajorTicks ? m_position : inf;
            m_minorTick = hasMinorTicks ? m_position * 20.0 : inf;
        } else {
            m_position = -pow(10.0, ceil(log10(-m_dimension.start)) + 1.0);
            m_majorTick = hasMajorTicks ? m_position : inf;
            m_minorTick = hasMinorTicks ? m_position * 0.09 : inf;
        }
    } else {
        m_majorTick = hasMajorTicks ? m_dimension.start : inf;
        m_minorTick = hasMinorTicks ? m_dimension.start : inf;
        m_position = slightlyLessThan(m_dimension.start);
    }

    ++(*this);
}

bool TickIterator::areAlmostEqual(qreal r1, qreal r2) const
{
    if (!m_isLogarithmic) {
        qreal span = m_dimension.end - m_dimension.start;
        if (span == 0) {
            // When start == end, we still want to show one tick if possible,
            // which needs this function to perform a reasonable comparison.
            span = qFuzzyIsNull(m_dimension.start) ? 1 : qAbs(m_dimension.start);
        }
        return qAbs(r2 - r1) < (span) * 1e-6;
    } else {
        return qAbs(r2 - r1) < qMax(qAbs(r1), qAbs(r2)) * 0.01;
    }
}

bool TickIterator::isHigherPrecedence(qreal importantTick, qreal unimportantTick) const
{
    return importantTick != std::numeric_limits<qreal>::infinity() && (importantTick <= unimportantTick || areAlmostEqual(importantTick, unimportantTick));
}

void TickIterator::computeMajorTickLabel(int decimalPlaces)
{
    if (m_manualLabelIndex >= 0) {
        m_text = m_manualLabelTexts[m_manualLabelIndex++];
        if (m_manualLabelIndex >= m_manualLabelTexts.count()) {
            // manual label texts repeat if there are less label texts than ticks on an axis
            m_manualLabelIndex = 0;
        }
        m_type = m_majorThinningFactor > 1 ? MajorTickManualShort : MajorTickManualLong;
    } else {
        // if m_axis is null, we are dealing with grid lines. grid lines never need labels.
        if (m_axis && (m_majorLabelCount++ % m_majorThinningFactor) == 0) {
            QMap<qreal, QString>::ConstIterator it =
                m_dataHeaderLabels.lowerBound(slightlyLessThan(m_position));

            if (it != m_dataHeaderLabels.constEnd() && areAlmostEqual(it.key(), m_position)) {
                m_text = it.value();
                m_type = MajorTickHeaderDataLabel;
            } else {
                // 'f' to avoid exponential notation for large numbers, consistent with data value text
                if (decimalPlaces < 0) {
                    decimalPlaces = numSignificantDecimalPlaces(m_position);
                }
                m_text = QString::number(m_position, 'f', decimalPlaces);
                m_type = MajorTick;
            }
        } else {
            m_text.clear();
            m_type = MajorTick;
        }
    }
}

void TickIterator::operator++()
{
    if (isAtEnd()) {
        return;
    }
    const qreal inf = std::numeric_limits<qreal>::infinity();

    // make sure to find the next tick at a value strictly greater than m_position

    if (!m_annotations.isEmpty()) {
        auto it = m_annotations.upperBound(m_position);
        if (it != m_annotations.constEnd()) {
            m_position = it.key();
            m_text = it.value();
            m_type = CustomTick;
        } else {
            m_position = inf;
        }
    } else if (!m_isLogarithmic && m_dimension.stepWidth * 1e6 < qMax(qAbs(m_dimension.start), qAbs(m_dimension.end))) {
        // If the step width is too small to increase m_position at all, we get an infinite loop.
        // This usually happens when m_dimension.start == m_dimension.end and both are very large.
        // When start == end, the step width defaults to 1, and it doesn't scale with start or end.
        // So currently, we bail and show no tick at all for empty ranges > 10^6, but at least we don't hang.
        m_position = inf;
    } else {
        // advance the calculated ticks
        if (m_isLogarithmic) {
            while (m_majorTick <= m_position) {
                m_majorTick *= m_position >= 0 ? 10 : 0.1;
            }
            while (m_minorTick <= m_position) {
                // the next major tick position should be greater than this
                m_minorTick += m_majorTick * (m_position >= 0 ? 0.1 : 1.0);
            }
        } else {
            while (m_majorTick <= m_position) {
                m_majorTick += m_dimension.stepWidth;
            }
            while (m_minorTick <= m_position) {
                m_minorTick += m_dimension.subStepWidth;
            }
        }

        while (m_customTickIndex >= 0 && m_customTick <= m_position) {
            if (++m_customTickIndex >= m_customTicks.count()) {
                m_customTickIndex = -1;
                m_customTick = inf;
                break;
            }
            m_customTick = m_customTicks.at(m_customTickIndex);
        }

        // now see which kind of tick we'll have
        if (isHigherPrecedence(m_customTick, m_majorTick) && isHigherPrecedence(m_customTick, m_minorTick)) {
            m_position = m_customTick;
            computeMajorTickLabel(-1);
            // override the MajorTick type here because those tick's labels are collision-tested, which we don't want
            // for custom ticks. they may be arbitrarily close to other ticks, causing excessive label thinning.
            if (m_type == MajorTick) {
                m_type = CustomTick;
            }
        } else if (isHigherPrecedence(m_majorTick, m_minorTick)) {
            m_position = m_majorTick;
            if (m_minorTick != inf) {
                // realign minor to major
                m_minorTick = m_majorTick;
            }
            computeMajorTickLabel(m_decimalPlaces);
        } else if (m_minorTick != inf) {
            m_position = m_minorTick;
            m_text.clear();
            m_type = MinorTick;
        } else {
            m_position = inf;
        }
    }

    if (m_position > m_dimension.end || ISNAN(m_position)) {
        m_position = inf; // make isAtEnd() return true
        m_text.clear();
        m_type = NoTick;
    }
}

CartesianAxis::CartesianAxis(AbstractCartesianDiagram *diagram)
    : AbstractAxis(new Private(diagram, this), diagram)
{
    init();
}

CartesianAxis::~CartesianAxis()
{
    // when we remove the first axis it will unregister itself and
    // propagate the next one to the primary, thus the while loop
    while (d->mDiagram) {
        auto *cd = qobject_cast<AbstractCartesianDiagram *>(d->mDiagram);
        cd->takeAxis(this);
    }
    for (AbstractDiagram *diagram : std::as_const(d->secondaryDiagrams)) {
        auto *cd = qobject_cast<AbstractCartesianDiagram *>(diagram);
        cd->takeAxis(this);
    }
}

void CartesianAxis::init()
{
    d->customTickLength = 3;
    d->position = Bottom;
    setCachedSizeDirty();
    connect(this, &AbstractAxis::coordinateSystemChanged, this, &CartesianAxis::coordinateSystemChanged);
}

bool CartesianAxis::compare(const CartesianAxis *other) const
{
    if (other == this) {
        return true;
    }
    if (!other) {
        return false;
    }
    return AbstractAxis::compare(other) && (position() == other->position()) && (titleText() == other->titleText()) && (titleTextAttributes() == other->titleTextAttributes());
}

void CartesianAxis::coordinateSystemChanged()
{
    layoutPlanes();
}

void CartesianAxis::setTitleText(const QString &text)
{
    d->titleText = text;
    setCachedSizeDirty();
    layoutPlanes();
}

QString CartesianAxis::titleText() const
{
    return d->titleText;
}

void CartesianAxis::setTitleTextAttributes(const TextAttributes &a)
{
    d->titleTextAttributes = a;
    d->useDefaultTextAttributes = false;
    setCachedSizeDirty();
    layoutPlanes();
}

TextAttributes CartesianAxis::titleTextAttributes() const
{
    if (hasDefaultTitleTextAttributes()) {
        TextAttributes ta(textAttributes());
        Measure me(ta.fontSize());
        me.setValue(me.value() * 1.5);
        ta.setFontSize(me);
        return ta;
    }
    return d->titleTextAttributes;
}

void CartesianAxis::resetTitleTextAttributes()
{
    d->useDefaultTextAttributes = true;
    setCachedSizeDirty();
    layoutPlanes();
}

bool CartesianAxis::hasDefaultTitleTextAttributes() const
{
    return d->useDefaultTextAttributes;
}

void CartesianAxis::setPosition(Position p)
{
    if (d->position == p) {
        return;
    }
    d->position = p;
    // Invalidating size is not always necessary if both old and new positions are horizontal or both
    // vertical, but in practice there could be small differences due to who-knows-what, so always adapt
    // to the possibly new size. Changing position is expensive anyway.
    setCachedSizeDirty();
    layoutPlanes();
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(Q_COMPILER_MANGLES_RETURN_TYPE)
const
#endif
    CartesianAxis::Position
    CartesianAxis::position() const
{
    return d->position;
}

void CartesianAxis::layoutPlanes()
{
    if (!d->diagram() || !d->diagram()->coordinatePlane()) {
        return;
    }
    AbstractCoordinatePlane *plane = d->diagram()->coordinatePlane();
    if (plane) {
        plane->layoutPlanes();
    }
}

static bool referenceDiagramIsBarDiagram(const AbstractDiagram *diagram)
{
    const auto *dia =
        qobject_cast<const AbstractCartesianDiagram *>(diagram);
    if (dia && dia->referenceDiagram())
        dia = dia->referenceDiagram();
    return qobject_cast<const BarDiagram *>(dia) != 0;
}

static bool referenceDiagramNeedsCenteredAbscissaTicks(const AbstractDiagram *diagram)
{
    const auto *dia =
        qobject_cast<const AbstractCartesianDiagram *>(diagram);
    if (dia && dia->referenceDiagram())
        dia = dia->referenceDiagram();
    if (qobject_cast<const BarDiagram *>(dia))
        return true;
    if (qobject_cast<const StockDiagram *>(dia))
        return true;

    const auto *lineDiagram = qobject_cast<const LineDiagram *>(dia);
    return lineDiagram && lineDiagram->centerDataPoints();
}

bool CartesianAxis::isAbscissa() const
{
    const Qt::Orientation diagramOrientation = referenceDiagramIsBarDiagram(d->diagram()) ? (( BarDiagram * )(d->diagram()))->orientation()
                                                                                          : Qt::Vertical;
    return diagramOrientation == Qt::Vertical ? position() == Bottom || position() == Top
                                              : position() == Left || position() == Right;
}

bool CartesianAxis::isOrdinate() const
{
    return !isAbscissa();
}

void CartesianAxis::paint(QPainter *painter)
{
    if (!d->diagram() || !d->diagram()->coordinatePlane()) {
        return;
    }
    PaintContext ctx;
    ctx.setPainter(painter);
    AbstractCoordinatePlane *const plane = d->diagram()->coordinatePlane();
    ctx.setCoordinatePlane(plane);

    ctx.setRectangle(QRectF(areaGeometry()));
    PainterSaver painterSaver(painter);

    // enable clipping only when required due to zoom, because it slows down painting
    // (the alternative to clipping when zoomed in requires much more work to paint just the right area)
    const qreal zoomFactor = d->isVertical() ? plane->zoomFactorY() : plane->zoomFactorX();
    if (zoomFactor > 1.0) {
        painter->setClipRegion(areaGeometry().adjusted(-d->amountOfLeftOverlap - 1, -d->amountOfTopOverlap - 1,
                                                       d->amountOfRightOverlap + 1, d->amountOfBottomOverlap + 1));
    }
    paintCtx(&ctx);
}

const TextAttributes CartesianAxis::Private::titleTextAttributesWithAdjustedRotation() const
{
    TextAttributes titleTA(titleTextAttributes);
    int rotation = titleTA.rotation();
    if (position == Left || position == Right) {
        rotation += 270;
    }
    if (rotation >= 360) {
        rotation -= 360;
    }
    // limit the allowed values to 0, 90, 180, 270
    rotation = (rotation / 90) * 90;
    titleTA.setRotation(rotation);
    return titleTA;
}

QString CartesianAxis::Private::customizedLabelText(const QString &text, Qt::Orientation orientation,
                                                    qreal value) const
{
    // ### like in the old code, using int( value ) as column number...
    QString withUnits = diagram()->unitPrefix(int(value), orientation, true) + text + diagram()->unitSuffix(int(value), orientation, true);
    return axis()->customizedLabel(withUnits);
}

void CartesianAxis::setTitleSpace(qreal axisTitleSpace)
{
    d->axisTitleSpace = axisTitleSpace;
}

qreal CartesianAxis::titleSpace() const
{
    return d->axisTitleSpace;
}

void CartesianAxis::setTitleSize(qreal value)
{
    Q_UNUSED(value)
    // ### remove me
}

qreal CartesianAxis::titleSize() const
{
    // ### remove me
    return 1.0;
}

void CartesianAxis::Private::drawTitleText(QPainter *painter, CartesianCoordinatePlane *plane,
                                           const QRect &geoRect) const
{
    const TextAttributes titleTA(titleTextAttributesWithAdjustedRotation());
    if (titleTA.isVisible()) {
        TextLayoutItem titleItem(titleText, titleTA, plane->parent(), KDChartEnums::MeasureOrientationMinimum,
                                 Qt::AlignHCenter | Qt::AlignVCenter);
        QPointF point;
        QSize size = titleItem.sizeHint();
        switch (position) {
        case Top:
            point.setX(geoRect.left() + geoRect.width() / 2);
            point.setY(geoRect.top() + (size.height() / 2) / axisTitleSpace);
            size.setWidth(qMin(size.width(), axis()->geometry().width()));
            break;
        case Bottom:
            point.setX(geoRect.left() + geoRect.width() / 2);
            point.setY(geoRect.bottom() - (size.height() / 2) / axisTitleSpace);
            size.setWidth(qMin(size.width(), axis()->geometry().width()));
            break;
        case Left:
            point.setX(geoRect.left() + (size.width() / 2) / axisTitleSpace);
            point.setY(geoRect.top() + geoRect.height() / 2);
            size.setHeight(qMin(size.height(), axis()->geometry().height()));
            break;
        case Right:
            point.setX(geoRect.right() - (size.width() / 2) / axisTitleSpace);
            point.setY(geoRect.top() + geoRect.height() / 2);
            size.setHeight(qMin(size.height(), axis()->geometry().height()));
            break;
        }
        const PainterSaver painterSaver(painter);
        painter->setClipping(false);
        painter->translate(point);
        titleItem.setGeometry(QRect(QPoint(-size.width() / 2, -size.height() / 2), size));
        titleItem.paint(painter);
    }
}

bool CartesianAxis::Private::isVertical() const
{
    return axis()->isAbscissa() == AbstractDiagram::Private::get(diagram())->isTransposed();
}

void CartesianAxis::paintCtx(PaintContext *context)
{
    Q_ASSERT_X(d->diagram(), "CartesianAxis::paint",
               "Function call not allowed: The axis is not assigned to any diagram.");

    auto *plane = dynamic_cast<CartesianCoordinatePlane *>(context->coordinatePlane());
    Q_ASSERT_X(plane, "CartesianAxis::paint",
               "Bad function call: PaintContext::coordinatePlane() NOT a cartesian plane.");

    // note: Not having any data model assigned is no bug
    //       but we can not draw an axis then either.
    if (!d->diagram()->model()) {
        return;
    }

    const bool centerTicks = referenceDiagramNeedsCenteredAbscissaTicks(d->diagram()) && isAbscissa();

    XySwitch geoXy(d->isVertical());

    QPainter *const painter = context->painter();

    // determine the position of the axis (also required for labels) and paint it

    qreal transversePosition = signalingNaN; // in data space
    // the next one describes an additional shift in screen space; it is unfortunately required to
    // make axis sharing work, which uses the areaGeometry() to override the position of the axis.
    qreal transverseScreenSpaceShift = signalingNaN;
    {
        // determine the unadulterated position in screen space

        DataDimension dimX = plane->gridDimensionsList().first();
        DataDimension dimY = plane->gridDimensionsList().last();
        QPointF start(dimX.start, dimY.start);
        QPointF end(dimX.end, dimY.end);
        // consider this: you can turn a diagonal line into a horizontal or vertical line on any
        // edge by changing just one of its four coordinates.
        switch (position()) {
        case CartesianAxis::Bottom:
            end.setY(dimY.start);
            break;
        case CartesianAxis::Top:
            start.setY(dimY.end);
            break;
        case CartesianAxis::Left:
            end.setX(dimX.start);
            break;
        case CartesianAxis::Right:
            start.setX(dimX.end);
            break;
        }

        transversePosition = geoXy(start.y(), start.x());

        QPointF transStart = plane->translate(start);
        QPointF transEnd = plane->translate(end);

        // an externally set areaGeometry() moves the axis position transversally; the shift is
        // nonzero only when this is a shared axis

        const QRect geo = areaGeometry();
        switch (position()) {
        case CartesianAxis::Bottom:
            transverseScreenSpaceShift = geo.top() - transStart.y();
            break;
        case CartesianAxis::Top:
            transverseScreenSpaceShift = geo.bottom() - transStart.y();
            break;
        case CartesianAxis::Left:
            transverseScreenSpaceShift = geo.right() - transStart.x();
            break;
        case CartesianAxis::Right:
            transverseScreenSpaceShift = geo.left() - transStart.x();
            break;
        }

        geoXy.lvalue(transStart.ry(), transStart.rx()) += transverseScreenSpaceShift;
        geoXy.lvalue(transEnd.ry(), transEnd.rx()) += transverseScreenSpaceShift;

        if (rulerAttributes().showRulerLine()) {
            bool clipSaved = context->painter()->hasClipping();
            painter->setClipping(false);
            painter->drawLine(transStart, transEnd);
            painter->setClipping(clipSaved);
        }
    }

    // paint ticks and labels

    TextAttributes labelTA = textAttributes();
    RulerAttributes rulerAttr = rulerAttributes();

    int labelThinningFactor = 1;
    // TODO: label thinning also when grid line distance < 4 pixels, not only when labels collide
    auto *tickLabel = new TextLayoutItem(QString(), labelTA, plane->parent(),
                                         KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft);
    auto *prevTickLabel = new TextLayoutItem(QString(), labelTA, plane->parent(),
                                             KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft);
    QPointF prevTickLabelPos;
    enum
    {
        Layout = 0,
        Painting,
        Done
    };
    for (int step = labelTA.isVisible() ? Layout : Painting; step < Done; step++) {
        bool skipFirstTick = !rulerAttr.showFirstTick();
        bool isFirstLabel = true;
        for (TickIterator it(this, plane, labelThinningFactor, centerTicks); !it.isAtEnd(); ++it) {
            if (skipFirstTick) {
                skipFirstTick = false;
                continue;
            }

            const qreal drawPos = it.position() + (centerTicks ? 0.5 : 0.);
            QPointF onAxis = plane->translate(geoXy(QPointF(drawPos, transversePosition),
                                                    QPointF(transversePosition, drawPos)));
            geoXy.lvalue(onAxis.ry(), onAxis.rx()) += transverseScreenSpaceShift;
            const bool isOutwardsPositive = position() == Bottom || position() == Right;

            // paint the tick mark

            QPointF tickEnd = onAxis;
            qreal tickLen = it.type() == TickIterator::CustomTick ? d->customTickLength : tickLength(it.type() == TickIterator::MinorTick);
            geoXy.lvalue(tickEnd.ry(), tickEnd.rx()) += isOutwardsPositive ? tickLen : -tickLen;

            // those adjustments are required to paint the ticks exactly on the axis and of the right length
            if (position() == Top) {
                onAxis.ry() += 1;
                tickEnd.ry() += 1;
            } else if (position() == Left) {
                tickEnd.rx() += 1;
            }

            if (step == Painting) {
                painter->save();
                if (rulerAttr.hasTickMarkPenAt(it.position())) {
                    painter->setPen(rulerAttr.tickMarkPen(it.position()));
                } else {
                    painter->setPen(it.type() == TickIterator::MinorTick ? rulerAttr.minorTickMarkPen()
                                                                         : rulerAttr.majorTickMarkPen());
                }
                painter->drawLine(onAxis, tickEnd);
                painter->restore();
            }

            if (it.text().isEmpty() || !labelTA.isVisible()) {
                // the following code in the loop is only label painting, so skip it
                continue;
            }

            // paint the label

            QString text = it.text();
            if (it.type() == TickIterator::MajorTick) {
                // add unit prefixes and suffixes, then customize
                text = d->customizedLabelText(text, geoXy(Qt::Horizontal, Qt::Vertical), it.position());
            } else if (it.type() == TickIterator::MajorTickHeaderDataLabel) {
                // unit prefixes and suffixes have already been added in this case - only customize
                text = customizedLabel(text);
            }

            tickLabel->setText(text);
            QSizeF size = QSizeF(tickLabel->sizeHint());
            QPolygon labelPoly = tickLabel->boundingPolygon();
            Q_ASSERT(labelPoly.count() == 4);

            // for alignment, find the label polygon edge "most parallel" and closest to the axis

            int axisAngle = 0;
            switch (position()) {
            case Bottom:
                axisAngle = 0;
                break;
            case Top:
                axisAngle = 180;
                break;
            case Right:
                axisAngle = 270;
                break;
            case Left:
                axisAngle = 90;
                break;
            default:
                Q_ASSERT(false);
            }
            // the left axis is not actually pointing down and the top axis not actually pointing
            // left, but their corresponding closest edges of a rectangular unrotated label polygon are.

            int relAngle = axisAngle - labelTA.rotation() + 45;
            if (relAngle < 0) {
                relAngle += 360;
            }
            int polyCorner1 = relAngle / 90;
            QPoint p1 = labelPoly.at(polyCorner1);
            QPoint p2 = labelPoly.at(polyCorner1 == 3 ? 0 : (polyCorner1 + 1));

            QPointF labelPos = tickEnd;

            qreal labelMargin = rulerAttr.labelMargin();
            if (labelMargin < 0) {
                labelMargin = QFontMetricsF(tickLabel->realFont()).height() * 0.5;
            }
            labelMargin -= tickLabel->marginWidth(); // make up for the margin that's already there

            switch (position()) {
            case Left:
                labelPos += QPointF(-size.width() - labelMargin,
                                    -0.45 * size.height() - 0.5 * (p1.y() + p2.y()));
                break;
            case Right:
                labelPos += QPointF(labelMargin,
                                    -0.45 * size.height() - 0.5 * (p1.y() + p2.y()));
                break;
            case Top:
                labelPos += QPointF(-0.45 * size.width() - 0.5 * (p1.x() + p2.x()),
                                    -size.height() - labelMargin);
                break;
            case Bottom:
                labelPos += QPointF(-0.45 * size.width() - 0.5 * (p1.x() + p2.x()),
                                    labelMargin);
                break;
            }

            tickLabel->setGeometry(QRect(labelPos.toPoint(), size.toSize()));

            if (step == Painting) {
                tickLabel->paint(painter);
            }

            // collision check the current label against the previous one

            // like in the old code, we don't shorten or decimate labels if they are already the
            // manual short type, or if they are the manual long type and on the vertical axis
            // ### they can still collide though, especially when they're rotated!
            if (step == Layout) {
                int spaceSavingRotation = geoXy(270, 0);
                bool canRotate = labelTA.autoRotate() && labelTA.rotation() != spaceSavingRotation;
                const bool canShortenLabels = !geoXy.isY && it.type() == TickIterator::MajorTickManualLong && it.hasShorterLabels();
                bool collides = false;
                if (it.type() == TickIterator::MajorTick || it.type() == TickIterator::MajorTickHeaderDataLabel
                    || canShortenLabels || canRotate) {
                    if (isFirstLabel) {
                        isFirstLabel = false;
                    } else {
                        collides = tickLabel->intersects(*prevTickLabel, labelPos, prevTickLabelPos);
                        qSwap(prevTickLabel, tickLabel);
                    }
                    prevTickLabelPos = labelPos;
                }
                if (collides) {
                    // to make room, we try in order: shorten, rotate, decimate
                    if (canRotate && !canShortenLabels) {
                        labelTA.setRotation(spaceSavingRotation);
                        // tickLabel will be reused in the next round
                        tickLabel->setTextAttributes(labelTA);
                    } else {
                        labelThinningFactor++;
                    }
                    step--; // relayout
                    break;
                }
            }
        }
    }
    delete tickLabel;
    tickLabel = nullptr;
    delete prevTickLabel;
    prevTickLabel = nullptr;

    if (!titleText().isEmpty()) {
        d->drawTitleText(painter, plane, geometry());
    }
}

/* pure virtual in QLayoutItem */
bool CartesianAxis::isEmpty() const
{
    return false; // if the axis exists, it has some (perhaps default) content
}

/* pure virtual in QLayoutItem */
Qt::Orientations CartesianAxis::expandingDirections() const
{
    Qt::Orientations ret;
    switch (position()) {
    case Bottom:
    case Top:
        ret = Qt::Horizontal;
        break;
    case Left:
    case Right:
        ret = Qt::Vertical;
        break;
    default:
        Q_ASSERT(false);
        break;
    };
    return ret;
}

void CartesianAxis::setCachedSizeDirty() const
{
    d->cachedMaximumSize = QSize();
}

/* pure virtual in QLayoutItem */
QSize CartesianAxis::maximumSize() const
{
    if (!d->cachedMaximumSize.isValid())
        d->cachedMaximumSize = d->calculateMaximumSize();
    return d->cachedMaximumSize;
}

QSize CartesianAxis::Private::calculateMaximumSize() const
{
    if (!diagram()) {
        return QSize();
    }

    auto *plane = dynamic_cast<CartesianCoordinatePlane *>(diagram()->coordinatePlane());
    Q_ASSERT(plane);
    QObject *refArea = plane->parent();
    const bool centerTicks = referenceDiagramNeedsCenteredAbscissaTicks(diagram())
        && axis()->isAbscissa();

    // we ignore:
    // - label thinning (expensive, not worst case and we want worst case)
    // - label autorotation (expensive, obscure feature(?))
    // - axis length (it is determined by the plane / diagram / chart anyway)
    // - the title's influence on axis length; this one might be TODO. See KDCH-863.

    XySwitch geoXy(isVertical());
    qreal size = 0; // this is the size transverse to the axis direction

    // the following variables describe how much the first and last label stick out over the axis
    // area, so that the geometry of surrounding layout items can be adjusted to make room.
    qreal startOverhang = 0.0;
    qreal endOverhang = 0.0;

    if (mAxis->textAttributes().isVisible()) {
        // these four are used just to calculate startOverhang and endOverhang
        qreal lowestLabelPosition = signalingNaN;
        qreal highestLabelPosition = signalingNaN;
        qreal lowestLabelLongitudinalSize = signalingNaN;
        qreal highestLabelLongitudinalSize = signalingNaN;

        TextLayoutItem tickLabel(QString(), mAxis->textAttributes(), refArea,
                                 KDChartEnums::MeasureOrientationMinimum, Qt::AlignLeft);
        const RulerAttributes rulerAttr = mAxis->rulerAttributes();

        bool showFirstTick = rulerAttr.showFirstTick();
        for (TickIterator it(axis(), plane, 1, centerTicks); !it.isAtEnd(); ++it) {
            const qreal drawPos = it.position() + (centerTicks ? 0.5 : 0.);
            if (!showFirstTick) {
                showFirstTick = true;
                continue;
            }

            qreal labelSizeTransverse = 0.0;
            qreal labelMargin = 0.0;
            QString text = it.text();
            if (!text.isEmpty()) {
                QPointF labelPosition = plane->translate(QPointF(geoXy(drawPos, ( qreal )1.0),
                                                                 geoXy(( qreal )1.0, drawPos)));
                highestLabelPosition = geoXy(labelPosition.x(), labelPosition.y());

                if (it.type() == TickIterator::MajorTick) {
                    // add unit prefixes and suffixes, then customize
                    text = customizedLabelText(text, geoXy(Qt::Horizontal, Qt::Vertical), it.position());
                } else if (it.type() == TickIterator::MajorTickHeaderDataLabel) {
                    // unit prefixes and suffixes have already been added in this case - only customize
                    text = axis()->customizedLabel(text);
                }
                tickLabel.setText(text);

                QSize sz = tickLabel.sizeHint();
                highestLabelLongitudinalSize = geoXy(sz.width(), sz.height());
                if (ISNAN(lowestLabelLongitudinalSize)) {
                    lowestLabelLongitudinalSize = highestLabelLongitudinalSize;
                    lowestLabelPosition = highestLabelPosition;
                }

                labelSizeTransverse = geoXy(sz.height(), sz.width());
                labelMargin = rulerAttr.labelMargin();
                if (labelMargin < 0) {
                    labelMargin = QFontMetricsF(tickLabel.realFont()).height() * 0.5;
                }
                labelMargin -= tickLabel.marginWidth(); // make up for the margin that's already there
            }
            qreal tickLength = it.type() == TickIterator::CustomTick ? customTickLength : axis()->tickLength(it.type() == TickIterator::MinorTick);
            size = qMax(size, tickLength + labelMargin + labelSizeTransverse);
        }

        const DataDimension dimX = plane->gridDimensionsList().first();
        const DataDimension dimY = plane->gridDimensionsList().last();

        QPointF pt = plane->translate(QPointF(dimX.start, dimY.start));
        const qreal lowestPosition = geoXy(pt.x(), pt.y());
        pt = plane->translate(QPointF(dimX.end, dimY.end));
        const qreal highestPosition = geoXy(pt.x(), pt.y());

        // the geoXy( 1.0, -1.0 ) here is necessary because Qt's y coordinate is inverted
        startOverhang = qMax(0.0, (lowestPosition - lowestLabelPosition) * geoXy(1.0, -1.0) + lowestLabelLongitudinalSize * 0.5);
        endOverhang = qMax(0.0, (highestLabelPosition - highestPosition) * geoXy(1.0, -1.0) + highestLabelLongitudinalSize * 0.5);
    }

    amountOfLeftOverlap = geoXy(startOverhang, ( qreal )0.0);
    amountOfRightOverlap = geoXy(endOverhang, ( qreal )0.0);
    amountOfBottomOverlap = geoXy(( qreal )0.0, startOverhang);
    amountOfTopOverlap = geoXy(( qreal )0.0, endOverhang);

    const TextAttributes titleTA = titleTextAttributesWithAdjustedRotation();
    if (titleTA.isVisible() && !axis()->titleText().isEmpty()) {
        TextLayoutItem title(axis()->titleText(), titleTA, refArea, KDChartEnums::MeasureOrientationMinimum,
                             Qt::AlignHCenter | Qt::AlignVCenter);

        QFontMetricsF titleFM(title.realFont(), GlobalMeasureScaling::paintDevice());
        size += geoXy(titleFM.height() * 0.33, titleFM.averageCharWidth() * 0.55); // spacing
        size += geoXy(title.sizeHint().height(), title.sizeHint().width());
    }

    // the size parallel to the axis direction is not determined by us, so we just return 1
    return QSize(geoXy(1, int(size)), geoXy(int(size), 1));
}

/* pure virtual in QLayoutItem */
QSize CartesianAxis::minimumSize() const
{
    return maximumSize();
}

/* pure virtual in QLayoutItem */
QSize CartesianAxis::sizeHint() const
{
    return maximumSize();
}

/* pure virtual in QLayoutItem */
void CartesianAxis::setGeometry(const QRect &r)
{
    if (d->geometry != r) {
        d->geometry = r;
        setCachedSizeDirty();
    }
}

/* pure virtual in QLayoutItem */
QRect CartesianAxis::geometry() const
{
    return d->geometry;
}

void CartesianAxis::setCustomTickLength(int value)
{
    if (d->customTickLength == value) {
        return;
    }
    d->customTickLength = value;
    setCachedSizeDirty();
    layoutPlanes();
}

int CartesianAxis::customTickLength() const
{
    return d->customTickLength;
}

int CartesianAxis::tickLength(bool subUnitTicks) const
{
    const RulerAttributes &rulerAttr = rulerAttributes();
    return subUnitTicks ? rulerAttr.minorTickMarkLength() : rulerAttr.majorTickMarkLength();
}

QMultiMap<qreal, QString> CartesianAxis::annotations() const
{
    return d->annotations;
}

void CartesianAxis::setAnnotations(const QMultiMap<qreal, QString> &annotations)
{
    if (d->annotations == annotations)
        return;

    d->annotations = annotations;
    setCachedSizeDirty();
    layoutPlanes();
}

QList<qreal> CartesianAxis::customTicks() const
{
    return d->customTicksPositions;
}

void CartesianAxis::setCustomTicks(const QList<qreal> &customTicksPositions)
{
    if (d->customTicksPositions == customTicksPositions)
        return;

    d->customTicksPositions = customTicksPositions;
    setCachedSizeDirty();
    layoutPlanes();
}
