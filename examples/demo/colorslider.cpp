/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "colorslider.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

#include <QDebug>

class ColorSlider::Private
{
public:
    Private(ColorSlider *q);
    void drawSliderMarker(QPainter *painter);
    QColor m_startColor;
    QColor m_endColor;
    bool m_dragging;
    ColorSlider *qq;
};

ColorSlider::Private::Private(ColorSlider *q)
    : m_dragging(false)
    , qq(q)
{
}

void ColorSlider::Private::drawSliderMarker(QPainter *painter)
{
    QVector<QPointF> points;
    points << QPointF(-0.5, 0) << QPointF(0.0, 0.5) << QPointF(0.5, 0);
    QPolygonF triangle(points);
    painter->drawPolygon(triangle);
}

ColorSlider::ColorSlider(QWidget *parent)
    : QAbstractSlider(parent)
    , d(new Private(this))
{
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
    setMinimum(0);
    setMaximum(255);
    setValue(25);
}

ColorSlider::~ColorSlider()
{
    delete d;
}

QColor ColorSlider::startColor() const
{
    return d->m_startColor;
}
void ColorSlider::setStartColor(const QColor &color)
{
    if (d->m_startColor != color) {
        d->m_startColor = color;
        Q_EMIT startColorChanged();
        update();
    }
}
QColor ColorSlider::endColor() const
{
    return d->m_endColor;
}
void ColorSlider::setEndColor(const QColor &color)
{
    if (d->m_endColor != color) {
        d->m_endColor = color;
        Q_EMIT startColorChanged();
        update();
    }
}

void ColorSlider::paintEvent(QPaintEvent *event)
{
    QAbstractSlider::paintEvent(event);
    const double percentage = value() / ( double )(maximum() - minimum());
    QPainter painter(this);
    QLinearGradient gradient;
    gradient.setStart(0, 0);
    gradient.setFinalStop(1, 0);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient.setColorAt(0, d->m_startColor);
    gradient.setColorAt(1, d->m_endColor);
    painter.setBrush(gradient);
    painter.drawRect(event->rect());

    painter.setBrush(Qt::white);
    painter.save();
    painter.translate(event->rect().width() * percentage, 0);
    QPen oldPen = painter.pen();
    QPen linePen = oldPen;
    linePen.setColor(Qt::white);
    painter.setPen(linePen);
    painter.drawLine(0, 0, 0, rect().height());
    painter.setPen(oldPen);

    painter.save();
    painter.translate(0, rect().height());
    painter.rotate(180);
    painter.scale(15, 15);
    d->drawSliderMarker(&painter);
    painter.restore();
    painter.scale(15, 15);
    d->drawSliderMarker(&painter);
    painter.restore();
}

void ColorSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        d->m_dragging = true;
        const qreal relPos = qMin(( qreal )1.0, qMax(( qreal )0.0, event->localPos().x() / rect().width()));
        setValue((maximum() - minimum()) * relPos);
    }
}

void ColorSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        d->m_dragging = false;
        const qreal relPos = qMin(( qreal )1.0, qMax(( qreal )0.0, event->localPos().x() / rect().width()));
        setValue((maximum() - minimum()) * relPos);
    }
}

void ColorSlider::mouseMoveEvent(QMouseEvent *event)
{
    if (d->m_dragging) {
        const qreal relPos = qMin(( qreal )1.0, qMax(( qreal )0.0, event->localPos().x() / rect().width()));
        setValue((maximum() - minimum()) * relPos);
    }
}

QSize ColorSlider::sizeHint() const
{
    return QSize(300, 50);
}
