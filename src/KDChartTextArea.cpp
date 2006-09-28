/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#include "KDChartTextArea.h"
#include "KDChartTextArea_p.h"

#include "KDChartPainterSaver_p.h"

#include <QPainter>
#include <QRect>
#include <QApplication>
#include <QStyle>
#include <QWidget>

#include <KDABLibFakes>


using namespace KDChart;

TextArea::Private::Private( TextArea * qq )
    : AbstractAreaBase::Private( /*qq*/ ),
      q( qq ),
      rect(),
      text(),
      attributes(),
      autoReferenceArea( 0 ),
      autoReferenceOrientation( KDChartEnums::MeasureOrientationHorizontal ),
      cachedSizeHint(),
      cachedFontSize( 0.0 ),
      cachedFont( attributes.font() )
{

}


TextArea::Private::~Private() {}


TextArea::TextArea()
    : QObject(), KDChart::AbstractAreaBase( new Private( this ) )
{
    // this bloc left empty intentionally
}

TextArea::~TextArea() {}

#define d d_func()

void TextArea::init() {}

void TextArea::setAutoReferenceArea( const QObject* area )
{
    if ( area == d->autoReferenceArea )
        return;
    d->autoReferenceArea = area;
    d->cachedSizeHint = QSize();
    sizeHint();
}

const QObject* TextArea::autoReferenceArea() const
{
    return d->autoReferenceArea;
}

void TextArea::setAutoReferenceOrientation( KDChartEnums::MeasureOrientation orient )
{
    if ( orient == d->autoReferenceOrientation )
        return;
    d->autoReferenceOrientation = orient;
    d->cachedSizeHint = QSize();
    sizeHint();
}

KDChartEnums::MeasureOrientation TextArea::autoReferenceOrientation() const
{
    return d->autoReferenceOrientation;
}

void TextArea::setText(const QString & text)
{
    if ( text == d->text )
        return;
    d->text = text;
    d->cachedSizeHint = QSize();
    sizeHint();
}

QString TextArea::text() const
{
    return d->text;
}

/**
  \brief Use this to specify the text attributes to be used for this item.

  \sa textAttributes
*/
void TextArea::setTextAttributes( const TextAttributes &a )
{
    if ( a == d->attributes )
        return;
    d->attributes = a;
    d->cachedSizeHint = QSize(); // invalidate size hint
    sizeHint();
}

/**
  Returns the text attributes to be used for this item.

  \sa setTextAttributes
*/
TextAttributes TextArea::textAttributes() const
{
    return d->attributes;
}

void TextArea::paintIntoRect( QPainter& painter, const QRect& rect )
{
    const QRect oldGeometry = d->rect;
    d->rect = rect;

    painter.translate( rect.left(), rect.top() );
    paintAll( painter );
    painter.translate( -rect.left(), -rect.top() );

    d->rect = oldGeometry;
}

void TextArea::paintAll( QPainter& painter )
{
    // Paint the background and frame
    paintBackground( painter, d->rect );
    paintFrame(      painter, d->rect );

    // temporarily adjust the widget size, to be sure all content gets calculated
    // to fit into the inner rectangle
    const QRect oldGeometry = d->rect;
    QRect inner = innerRect() ;
    inner.moveTo(
        oldGeometry.left() + inner.left(),
        oldGeometry.top()  + inner.top() );
    d->rect = inner;
    paint( &painter );
    d->rect = oldGeometry;
}

QRect TextArea::areaGeometry() const
{
    return d->rect;
}

void TextArea::setAreaGeometry( const QRect & rect ) {
    d->rect = rect;
}

void TextArea::positionHasChanged()
{
    emit positionChanged( this );
}

qreal TextArea::realFontSize() const
{
    return
        qMax( d->attributes.fontSize().calculatedValue( d->autoReferenceArea, d->autoReferenceOrientation ),
              d->attributes.minimalFontSize().calculatedValue( d->autoReferenceArea, d->autoReferenceOrientation ) );
}


bool TextArea::Private::realFontWasRecalculated() const
{
    const qreal fntSiz = q->realFontSize();

    if ( !cachedSizeHint.isValid() || cachedFontSize != fntSiz )
        return true;

    if( fntSiz > 0.0 ) {
        cachedFontSize = fntSiz;
        cachedFont.setPointSizeF( fntSiz );
    }
    return false;
}


QFont TextArea::realFont() const
{
    ( void )d->realFontWasRecalculated();
    return d->cachedFont;
}

QPolygon TextArea::Private::rotatedCorners() const
{
    // the angle in rad
    const qreal angle = attributes.rotation() * M_PI / 180.0;
    const QSize size = unrotatedSizeHint();

    // my P1 - P4 (the four points of the rotated area)
    QPointF P1( size.height() * sin( angle ), 0 );
    QPointF P2( size.height() * sin( angle ) + size.width() * cos( angle ), size.width() * sin( angle ) );
    QPointF P3( size.width() * cos( angle ), size.width() * sin( angle ) + size.height() * cos( angle ) );
    QPointF P4( 0, size.height() * cos( angle ) );

    QPolygon result;
    result << P1.toPoint() << P2.toPoint() << P3.toPoint() << P4.toPoint();
    return result;
}

bool TextArea::intersects( const TextArea& other, const QPointF& myPos, const QPointF& otherPos ) const
{
    return intersects( other, myPos.toPoint(), otherPos.toPoint() );
}

bool TextArea::intersects( const TextArea& other, const QPoint& myPos, const QPoint& otherPos ) const
{
    if ( d->attributes.rotation() != other.d->attributes.rotation() )
    {
        // that's the code for the common case: the rotation angles don't need to match here
        QPolygon myPolygon(          d->rotatedCorners() );
        QPolygon otherPolygon( other.d->rotatedCorners() );

        // move the polygons to their positions
        myPolygon.translate( myPos );
        otherPolygon.translate( otherPos );

        // create regions out of it
        const QRegion myRegion( myPolygon );
        const QRegion otherRegion( otherPolygon );

        // now the question - do they intersect or not?
        return ! myRegion.intersect( otherRegion ).isEmpty();

    } else {
        // and that's the code for the special case: the rotation angles match, which is less time consuming in calculation
        const qreal angle = d->attributes.rotation() * M_PI / 180.0;
        // both sizes
        const QSizeF mySize(          d->unrotatedSizeHint() );
        const QSizeF otherSize( other.d->unrotatedSizeHint() );

        // that's myP1 relative to myPos
        QPointF myP1( mySize.height() * sin( angle ), 0.0 );
        // that's otherP1 to myPos
        QPointF otherP1 = QPointF( otherSize.height() * sin( angle ), 0.0 ) + otherPos - myPos;

        // now rotate both points the negative angle around myPos
        myP1 = QPointF( myP1.x() * cos( -angle ), myP1.x() * sin( -angle ) );
        qreal r = sqrt( otherP1.x() * otherP1.x() + otherP1.y() * otherP1.y() );
        otherP1 = QPointF( r * cos( -angle ), r * sin( -angle ) );

        // finally we look, whether both rectangles intersect or even not
        return QRectF( myP1, mySize ).intersects( QRectF( otherP1, otherSize ) );
    }
}

QSize TextArea::sizeHint() const
{
    if ( d->realFontWasRecalculated() )
    {
        d->cachedSizeHint = d->calcSizeHint( d->cachedFont );
        sizeHintChanged();
    }
    return d->cachedSizeHint;
}

namespace {
static QWidget * parentWidget( QObject * o ) {
    for (  ; o ; o = o->parent() )
        if ( o->isWidgetType() )
            return qobject_cast<QWidget*>( o );
    return 0;
}
}

void TextArea::sizeHintChanged() const {
    // This is exactly like what QWidget::updateGeometry does.

    if ( QWidget * const w = parentWidget( const_cast<TextArea*>( this ) ) ) {
        if ( w->layout() )
            w->layout()->invalidate();
        else
            QApplication::postEvent( w, new QEvent( QEvent::LayoutRequest ) );
    }

}

// PENDING(kalle) Support auto shrink


QSize TextArea::Private::unrotatedSizeHint( const QFont & fnt ) const
{
    const QFontMetricsF met( fnt == QFont() ? cachedFont : fnt );
    const QSize ret = met.boundingRect( text ).toRect().size();
    const int frame = QApplication::style()->pixelMetric( QStyle::PM_ButtonMargin, 0, 0 );
    return ret + QSize( frame, frame );
}

QSize TextArea::Private::calcSizeHint( const QFont & fnt ) const
{
    const QSize ret = unrotatedSizeHint( fnt );
    const qreal angle = M_PI * attributes.rotation() / 180.0;
    QSize rotated( static_cast<int>( cos( angle ) * ret.width()  + sin( angle ) * ret.height() ),
                   static_cast<int>( cos( angle ) * ret.height() + sin( angle ) *  ret.width() ) );
    rotated.setWidth( qMax( rotated.height(), rotated.width() ) );
    return rotated;
}


void TextArea::paint( QPainter* painter )
{
    // make sure, cached font is updated, if needed:
    ( void )sizeHint();

    if( !d->rect.isValid() )
        return;

    const PainterSaver painterSaver( painter );
    painter->setFont( d->cachedFont );
    painter->setPen( d->attributes.pen() );
    QRectF rect = d->rect;

    painter->translate( rect.center() );
    rect.moveTopLeft( QPointF( - rect.width() / 2, - rect.height() / 2 ) );
    painter->rotate( d->attributes.rotation() );
    painter->drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter, d->text );
}

#undef d
