/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "colorslider.h"

#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include <QDebug>

class ColorSlider::Private
{
public:
    Private( ColorSlider *q );
    void drawSliderMarker( QPainter *painter );
    QColor m_startColor;
    QColor m_endColor;
    bool m_dragging;
    ColorSlider *qq;
};

ColorSlider::Private::Private( ColorSlider *q )
    : m_dragging( false )
    , qq( q )
{

}

void ColorSlider::Private::drawSliderMarker( QPainter *painter )
{
    QVector< QPointF > points; points << QPointF( -0.5, 0 ) << QPointF( 0.0, 0.5 ) << QPointF( 0.5, 0 );
    QPolygonF triangle( points );
    painter->drawPolygon( triangle );
}

ColorSlider::ColorSlider( QWidget *parent )
    : QAbstractSlider( parent )
    , d( new Private( this ) )
{
    setSizePolicy( QSizePolicy( QSizePolicy::Minimum ,QSizePolicy::Fixed) );
    setMinimum( 0 );
    setMaximum( 255 );
    setValue( 25 );
}

ColorSlider::~ColorSlider()
{
    delete d;
}

QColor ColorSlider::startColor() const
{
    return d->m_startColor;
}
void ColorSlider::setStartColor( const QColor &color )
{
    if ( d->m_startColor != color )
    {
        d->m_startColor = color;
        Q_EMIT startColorChanged();
        update();
    }
}
QColor ColorSlider::endColor() const
{
    return d->m_endColor;
}
void ColorSlider::setEndColor( const QColor &color )
{
    if ( d->m_endColor != color )
    {
        d->m_endColor = color;
        Q_EMIT startColorChanged();
        update();
    }
}

void ColorSlider::paintEvent( QPaintEvent *event )
{
    QAbstractSlider::paintEvent( event );
    const double percentage = value() / ( double )( maximum() - minimum() );
    QPainter painter( this );
    QLinearGradient gradient;
    gradient.setStart( 0, 0 );
    gradient.setFinalStop( 1, 0 );
    gradient.setCoordinateMode( QGradient::ObjectBoundingMode );
    gradient.setColorAt( 0, d->m_startColor );
    gradient.setColorAt( 1, d->m_endColor );
    painter.setBrush( gradient );
    painter.drawRect( event->rect() );

    painter.setBrush( Qt::white );
    painter.save();
    painter.translate( event->rect().width() * percentage, 0 );
    QPen oldPen = painter.pen();
    QPen linePen = oldPen;
    linePen.setColor( Qt::white );
    painter.setPen( linePen );
    painter.drawLine( 0, 0, 0, rect().height() );
    painter.setPen( oldPen );

    painter.save();
    painter.translate( 0, rect().height() );
    painter.rotate( 180 );
    painter.scale( 15, 15 );
    d->drawSliderMarker( &painter );
    painter.restore();
    painter.scale( 15, 15 );
    d->drawSliderMarker( &painter );
    painter.restore();

}

void ColorSlider::mousePressEvent(QMouseEvent *event )
{
    if ( event->button() == Qt::LeftButton )
    {
        d->m_dragging = true;
#if QT_VERSION < 0x050000
        const double relPos = qMin( 1.0, qMax( 0.0, event->posF().x() / rect().width() ) );
#else
        const double relPos = qMin( 1.0, qMax( 0.0, event->localPos().x() / rect().width() ) );
#endif
        setValue( (maximum() - minimum() ) * relPos );
    }
}

void ColorSlider::mouseReleaseEvent(QMouseEvent *event )
{
    if ( event->button() != Qt::LeftButton )
    {
        d->m_dragging = false;
#if QT_VERSION < 0x050000
        const double relPos = qMin( 1.0, qMax( 0.0, event->posF().x() / rect().width() ) );
#else
	const double relPos = qMin( 1.0, qMax( 0.0, event->localPos().x() / rect().width() ) );
#endif
        setValue( (maximum() - minimum() ) * relPos );
    }
}

void ColorSlider::mouseMoveEvent( QMouseEvent *event )
{
    if ( d->m_dragging )
    {
#if QT_VERSION < 0x050000
        const double relPos = qMin( 1.0, qMax( 0.0, event->posF().x() / rect().width() ) );
#else
        const double relPos = qMin( 1.0, qMax( 0.0, event->localPos().x() / rect().width() ) );
#endif
        setValue( (maximum() - minimum() ) * relPos );
    }
}

QSize ColorSlider::sizeHint() const
{
    return QSize( 300, 50 );
}
