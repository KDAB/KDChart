/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartPolarCoordinatePlane.h"
#include "KDChartPolarCoordinatePlane_p.h"

#include "KDChartAbstractDiagram.h"
#include "KDChartAbstractPolarDiagram.h"
#include "KDChartChart.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartPolarDiagram.h"

#include <math.h>

#include <QFont>
#include <QList>
#include <QPainter>
#include <QTimer>
#include <QtDebug>

#include <KDABLibFakes>

using namespace KDChart;

#define d d_func()

PolarCoordinatePlane::PolarCoordinatePlane(Chart *parent)
    : AbstractCoordinatePlane(new Private(), parent)
{
    // this block left empty intentionally
}

PolarCoordinatePlane::~PolarCoordinatePlane()
{
    // this block left empty intentionally
}

void PolarCoordinatePlane::init()
{
    // this block left empty intentionally
}

void PolarCoordinatePlane::addDiagram(AbstractDiagram *diagram)
{
    Q_ASSERT_X(dynamic_cast<AbstractPolarDiagram *>(diagram),
               "PolarCoordinatePlane::addDiagram", "Only polar"
                                                   "diagrams can be added to a polar coordinate plane!");
    AbstractCoordinatePlane::addDiagram(diagram);
    connect(diagram, &AbstractDiagram::layoutChanged, this, &PolarCoordinatePlane::slotLayoutChanged);
}

void PolarCoordinatePlane::paint(QPainter *painter)
{
    AbstractDiagramList diags = diagrams();
    if (d->coordinateTransformations.size() != diags.size()) {
        // diagrams have not been set up yet
        return;
    }
    // need at least one so d->currentTransformation can be a valid pointer
    Q_ASSERT(!d->coordinateTransformations.isEmpty());

    PaintContext ctx;
    ctx.setPainter(painter);
    ctx.setCoordinatePlane(this);
    ctx.setRectangle(geometry() /*d->contentRect*/);

    // 1. ask (only!) PolarDiagrams if they need additional space for data labels / data comments

    const qreal oldZoomX = zoomFactorX();
    const qreal oldZoomY = zoomFactorY();
    d->newZoomX = oldZoomX;
    d->newZoomY = oldZoomY;
    for (int i = 0; i < diags.size(); i++) {
        d->currentTransformation = &(d->coordinateTransformations[i]);
        qreal zoomX;
        qreal zoomY;
        auto *polarDia = dynamic_cast<PolarDiagram *>(diags[i]);
        if (polarDia) {
            polarDia->paint(&ctx, true, zoomX, zoomY);
            d->newZoomX = qMin(d->newZoomX, zoomX);
            d->newZoomY = qMin(d->newZoomY, zoomY);
        }
    }

    if (d->newZoomX != oldZoomX || d->newZoomY != oldZoomY) {
        // qDebug() << "new zoom:" << d->newZoomY << " old zoom:" << oldZoomY;
        d->currentTransformation = nullptr; // not painting anymore until we get called again
        QMetaObject::invokeMethod(this, "adjustZoomAndRepaint", Qt::QueuedConnection);
        return;
    }

    // 2. there was room enough for the labels, so start drawing

    // paint the coordinate system rulers:
    d->currentTransformation = &d->coordinateTransformations.first();
    d->grid->drawGrid(&ctx);

    // paint the diagrams which will re-use their DataValueTextInfoList(s) filled in step 1:
    for (int i = 0; i < diags.size(); i++) {
        d->currentTransformation = &(d->coordinateTransformations[i]);
        PainterSaver painterSaver(painter);
        auto *polarDia = dynamic_cast<PolarDiagram *>(diags[i]);
        if (polarDia) {
            qreal dummy1, dummy2;
            polarDia->paint(&ctx, false, dummy1, dummy2);
        } else {
            diags[i]->paint(&ctx);
        }
    }
    d->currentTransformation = nullptr;
}

void PolarCoordinatePlane::adjustZoomAndRepaint()
{
    const qreal newZoom = qMin(d->newZoomX, d->newZoomY);
    setZoomFactors(newZoom, newZoom);
    update();
}

void PolarCoordinatePlane::resizeEvent(QResizeEvent *)
{
    d->initialResizeEventReceived = true;
    layoutDiagrams();
}

void PolarCoordinatePlane::layoutDiagrams()
{
    // the rectangle the diagrams cover in the *plane*:
    // (Why -3? We save 1px on each side for the antialiased drawing, and
    // respect the way QPainter calculates the width of a painted rect (the
    // size is the rectangle size plus the pen width). This way, most clipping
    // for regular pens should be avoided. When pens with a penWidth or larger
    // than 1 are used, this may not b sufficient.
    const QRect rect(areaGeometry());
    d->contentRect = QRectF(1, 1, rect.width() - 3, rect.height() - 3);

    const ZoomParameters zoom = d->coordinateTransformations.isEmpty() ? ZoomParameters()
                                                                       : d->coordinateTransformations.front().zoom;
    // FIXME distribute space according to options:
    const qreal oldStartPosition = startPosition();
    d->coordinateTransformations.clear();
    const auto constDiagrams = diagrams();
    for (AbstractDiagram *diagram : constDiagrams) {
        auto *polarDiagram = dynamic_cast<AbstractPolarDiagram *>(diagram);
        Q_ASSERT(polarDiagram);
        QPair<QPointF, QPointF> dataBoundariesPair = polarDiagram->dataBoundaries();

        const qreal angleUnit = 360 / polarDiagram->valueTotals();
        // qDebug() << "--------------------------------------------------------";
        const qreal radius = qAbs(dataBoundariesPair.first.y()) + dataBoundariesPair.second.y();
        // qDebug() << radius <<"="<<dataBoundariesPair.second.y();
        const qreal diagramWidth = radius * 2; // == height
        const qreal planeWidth = d->contentRect.width();
        const qreal planeHeight = d->contentRect.height();
        const qreal radiusUnit = qMin(planeWidth, planeHeight) / diagramWidth;
        // qDebug() << radiusUnit <<"=" << "qMin( "<<planeWidth<<","<< planeHeight <<") / "<<diagramWidth;
        QPointF coordinateOrigin = QPointF(planeWidth / 2, planeHeight / 2);
        coordinateOrigin += d->contentRect.topLeft();

        CoordinateTransformation diagramTransposition;
        diagramTransposition.originTranslation = coordinateOrigin;
        diagramTransposition.radiusUnit = radiusUnit;
        diagramTransposition.angleUnit = angleUnit;
        diagramTransposition.startPosition = oldStartPosition;
        diagramTransposition.zoom = zoom;
        diagramTransposition.minValue = dataBoundariesPair.first.y() < 0 ? dataBoundariesPair.first.y() : 0.0;
        d->coordinateTransformations.append(diagramTransposition);
    }
    update();
}

const QPointF PolarCoordinatePlane::translate(const QPointF &diagramPoint) const
{
    Q_ASSERT_X(d->currentTransformation != nullptr, "PolarCoordinatePlane::translate",
               "Only call translate() from within paint().");
    return d->currentTransformation->translate(diagramPoint);
}

const QPointF PolarCoordinatePlane::translatePolar(const QPointF &diagramPoint) const
{
    Q_ASSERT_X(d->currentTransformation != nullptr, "PolarCoordinatePlane::translate",
               "Only call translate() from within paint().");
    return d->currentTransformation->translatePolar(diagramPoint);
}

qreal PolarCoordinatePlane::angleUnit() const
{
    Q_ASSERT_X(d->currentTransformation != nullptr, "PolarCoordinatePlane::angleUnit",
               "Only call angleUnit() from within paint().");
    return d->currentTransformation->angleUnit;
}

qreal PolarCoordinatePlane::radiusUnit() const
{
    Q_ASSERT_X(d->currentTransformation != nullptr, "PolarCoordinatePlane::radiusUnit",
               "Only call radiusUnit() from within paint().");
    return d->currentTransformation->radiusUnit;
}

void PolarCoordinatePlane::slotLayoutChanged(AbstractDiagram *)
{
    if (d->initialResizeEventReceived)
        layoutDiagrams();
}

void PolarCoordinatePlane::setStartPosition(qreal degrees)
{
    Q_ASSERT_X(diagram(), "PolarCoordinatePlane::setStartPosition",
               "setStartPosition() needs a diagram to be associated to the plane.");
    for (CoordinateTransformationList::iterator it = d->coordinateTransformations.begin();
         it != d->coordinateTransformations.end();
         ++it) {
        CoordinateTransformation &trans = *it;
        trans.startPosition = degrees;
    }
}

qreal PolarCoordinatePlane::startPosition() const
{
    return d->coordinateTransformations.isEmpty()
        ? 0.0
        : d->coordinateTransformations.first().startPosition;
}

qreal PolarCoordinatePlane::zoomFactorX() const
{
    return d->coordinateTransformations.isEmpty()
        ? 1.0
        : d->coordinateTransformations.first().zoom.xFactor;
}

qreal PolarCoordinatePlane::zoomFactorY() const
{
    return d->coordinateTransformations.isEmpty()
        ? 1.0
        : d->coordinateTransformations.first().zoom.yFactor;
}

void PolarCoordinatePlane::setZoomFactors(qreal factorX, qreal factorY)
{
    setZoomFactorX(factorX);
    setZoomFactorY(factorY);
}

void PolarCoordinatePlane::setZoomFactorX(qreal factor)
{
    for (CoordinateTransformationList::iterator it = d->coordinateTransformations.begin();
         it != d->coordinateTransformations.end();
         ++it) {
        CoordinateTransformation &trans = *it;
        trans.zoom.xFactor = factor;
    }
}

void PolarCoordinatePlane::setZoomFactorY(qreal factor)
{
    for (CoordinateTransformationList::iterator it = d->coordinateTransformations.begin();
         it != d->coordinateTransformations.end();
         ++it) {
        CoordinateTransformation &trans = *it;
        trans.zoom.yFactor = factor;
    }
}

QPointF PolarCoordinatePlane::zoomCenter() const
{
    return d->coordinateTransformations.isEmpty()
        ? QPointF(0.5, 0.5)
        : QPointF(d->coordinateTransformations.first().zoom.xCenter, d->coordinateTransformations.first().zoom.yCenter);
}

void PolarCoordinatePlane::setZoomCenter(const QPointF &center)
{
    for (CoordinateTransformationList::iterator it = d->coordinateTransformations.begin();
         it != d->coordinateTransformations.end();
         ++it) {
        CoordinateTransformation &trans = *it;
        trans.zoom.xCenter = center.x();
        trans.zoom.yCenter = center.y();
    }
}

DataDimensionsList PolarCoordinatePlane::getDataDimensionsList() const
{
    DataDimensionsList l;

    // FIXME(khz): do the real calculation

    return l;
}

void KDChart::PolarCoordinatePlane::setGridAttributes(
    bool circular,
    const GridAttributes &a)
{
    if (circular)
        d->gridAttributesCircular = a;
    else
        d->gridAttributesSagittal = a;
    setHasOwnGridAttributes(circular, true);
    update();
    Q_EMIT propertiesChanged();
}

void KDChart::PolarCoordinatePlane::resetGridAttributes(
    bool circular)
{
    setHasOwnGridAttributes(circular, false);
    update();
}

const GridAttributes KDChart::PolarCoordinatePlane::gridAttributes(
    bool circular) const
{
    if (hasOwnGridAttributes(circular)) {
        if (circular)
            return d->gridAttributesCircular;
        else
            return d->gridAttributesSagittal;
    } else {
        return globalGridAttributes();
    }
}

QRectF KDChart::PolarCoordinatePlane::Private::contentsRect(const KDChart::PolarCoordinatePlane *plane)
{
    QRectF contentsRect;
    QPointF referencePointAtTop = plane->translate(QPointF(1, 0));
    QPointF temp = plane->translate(QPointF(0, 0)) - referencePointAtTop;
    const qreal offset = temp.y();
    referencePointAtTop.setX(referencePointAtTop.x() - offset);
    contentsRect.setTopLeft(referencePointAtTop);
    contentsRect.setBottomRight(referencePointAtTop + QPointF(2.0 * offset, 2.0 * offset));
    return contentsRect;
}

void KDChart::PolarCoordinatePlane::setHasOwnGridAttributes(
    bool circular, bool on)
{
    if (circular)
        d->hasOwnGridAttributesCircular = on;
    else
        d->hasOwnGridAttributesSagittal = on;
    Q_EMIT propertiesChanged();
}

bool KDChart::PolarCoordinatePlane::hasOwnGridAttributes(
    bool circular) const
{
    return (circular)
        ? d->hasOwnGridAttributesCircular
        : d->hasOwnGridAttributesSagittal;
}
