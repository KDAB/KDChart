/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartAbstractAxis.h"
#include "KDChartAbstractAxis_p.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartEnums.h"
#include "KDChartMeasure.h"

#include <KDABLibFakes>

using namespace KDChart;

#define d d_func()

AbstractAxis::Private::Private(AbstractDiagram *diagram, AbstractAxis *axis)
    : observer(nullptr)
    , mDiagram(diagram)
    , mAxis(axis)
{
    // Note: We do NOT call setDiagram( diagram, axis );
    //       but it is called in AbstractAxis::delayedInit() instead!
}

AbstractAxis::Private::~Private()
{
    delete observer;
    observer = nullptr;
}

bool AbstractAxis::Private::setDiagram(AbstractDiagram *diagram_, bool delayedInit)
{
    AbstractDiagram *diagram = delayedInit ? mDiagram : diagram_;
    if (delayedInit) {
        mDiagram = nullptr;
    }

    // do not set a diagram again that was already set
    if (diagram && ((diagram == mDiagram) || secondaryDiagrams.contains(diagram)))
        return false;

    bool bNewDiagramStored = false;
    if (!mDiagram) {
        mDiagram = diagram;
        delete observer;
        if (mDiagram) {
            observer = new DiagramObserver(mDiagram, mAxis);
            const bool con = connect(observer, SIGNAL(diagramDataChanged(AbstractDiagram *)),
                                     mAxis, SIGNAL(coordinateSystemChanged()));
            Q_UNUSED(con)
            Q_ASSERT(con);
            bNewDiagramStored = true;
        } else {
            observer = nullptr;
        }
    } else {
        if (diagram)
            secondaryDiagrams.enqueue(diagram);
    }
    return bNewDiagramStored;
}

void AbstractAxis::Private::unsetDiagram(AbstractDiagram *diagram)
{
    if (diagram == mDiagram) {
        mDiagram = nullptr;
        delete observer;
        observer = nullptr;
    } else {
        secondaryDiagrams.removeAll(diagram);
    }
    if (!secondaryDiagrams.isEmpty()) {
        AbstractDiagram *nextDiagram = secondaryDiagrams.dequeue();
        setDiagram(nextDiagram);
    }
}

bool AbstractAxis::Private::hasDiagram(AbstractDiagram *diagram) const
{
    return diagram == mDiagram || secondaryDiagrams.contains(diagram);
}

void AbstractAxis::Private::updateLayouts()
{
    if (auto *cartesianAxis = qobject_cast<CartesianAxis *>(mAxis)) {
        cartesianAxis->layoutPlanes();
    } else {
        mAxis->update();
    }
}

AbstractAxis::AbstractAxis(AbstractDiagram *diagram)
    : AbstractArea(new Private(diagram, this))
{
    init();
    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

AbstractAxis::~AbstractAxis()
{
    d->mDiagram = nullptr;
    d->secondaryDiagrams.clear();
}

void AbstractAxis::init()
{
    Measure m(14, KDChartEnums::MeasureCalculationModeAuto, KDChartEnums::MeasureOrientationAuto);
    d->textAttributes.setFontSize(m);
    m.setValue(6);
    m.setCalculationMode(KDChartEnums::MeasureCalculationModeAbsolute);
    d->textAttributes.setMinimalFontSize(m);
    if (d->diagram())
        createObserver(d->diagram());
}

void AbstractAxis::delayedInit()
{
    // We call setDiagram() here, because the c'tor of Private
    // only has stored the pointers, but it did not call setDiagram().
    if (d)
        d->setDiagram(nullptr, true /* delayedInit */);
}

bool AbstractAxis::compare(const AbstractAxis *other) const
{
    if (other == this) {
        return true;
    }
    if (!other) {
        return false;
    }

    return (static_cast<const AbstractAreaBase *>(this)->compare(other)) && (textAttributes() == other->textAttributes()) && (labels() == other->labels()) && (shortLabels() == other->shortLabels());
}

const QString AbstractAxis::customizedLabel(const QString &label) const
{
    return label;
}

void AbstractAxis::createObserver(AbstractDiagram *diagram)
{
    d->setDiagram(diagram);
}

void AbstractAxis::deleteObserver(AbstractDiagram *diagram)
{
    d->unsetDiagram(diagram);
}

void AbstractAxis::connectSignals()
{
    if (d->observer) {
        const bool con = connect(d->observer, SIGNAL(diagramDataChanged(AbstractDiagram *)),
                                 this, SIGNAL(coordinateSystemChanged()));
        Q_UNUSED(con);
        Q_ASSERT(con);
    }
}

void AbstractAxis::setTextAttributes(const TextAttributes &a)
{
    if (d->textAttributes == a)
        return;

    d->textAttributes = a;
    d->updateLayouts();
}

TextAttributes AbstractAxis::textAttributes() const
{
    return d->textAttributes;
}

void AbstractAxis::setRulerAttributes(const RulerAttributes &a)
{
    d->rulerAttributes = a;
    d->updateLayouts();
}

RulerAttributes AbstractAxis::rulerAttributes() const
{
    return d->rulerAttributes;
}

void AbstractAxis::setLabels(const QStringList &list)
{
    if (d->hardLabels == list)
        return;

    d->hardLabels = list;
    d->updateLayouts();
}

QStringList AbstractAxis::labels() const
{
    return d->hardLabels;
}

void AbstractAxis::setShortLabels(const QStringList &list)
{
    if (d->hardShortLabels == list)
        return;

    d->hardShortLabels = list;
    d->updateLayouts();
}

QStringList AbstractAxis::shortLabels() const
{
    return d->hardShortLabels;
}

const AbstractCoordinatePlane *AbstractAxis::coordinatePlane() const
{
    if (d->diagram())
        return d->diagram()->coordinatePlane();
    return nullptr;
}

const AbstractDiagram *KDChart::AbstractAxis::diagram() const
{
    return d->diagram();
}

bool KDChart::AbstractAxis::observedBy(AbstractDiagram *diagram) const
{
    return d->hasDiagram(diagram);
}

void KDChart::AbstractAxis::update()
{
    if (d->diagram())
        d->diagram()->update();
}
