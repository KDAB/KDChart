/****************************************************************************
 ** Copyright (C) 2006 Klar�vdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartLayoutItems.h"
#include "KDTextDocument.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextDocumentFragment>
#include <QAbstractTextDocumentLayout>
#include <QLayout>
#include <QPainter>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include <QStyle>

//FIXME(khz): use an internal libfakes library instead of this internal haeder file
#include "kdchart_platformdef.h"


/**
    Inform the item about its widget: This enables the item,
    to trigger that widget's update, whenever the size of the item's
    contents has changed.

    Thus, you need to call setParentWidget on every item, that
    has a non-fixed size.
  */
void KDChart::AbstractLayoutItem::setParentWidget( QWidget* widget )
{
    mParent = widget;
}

/**
  * Default impl: Paint the complete item using its layouted position and size.
  */
void KDChart::AbstractLayoutItem::paintCtx( PaintContext* context ) const
{
    paint( context->painter() );
}

/**
    Report changed size hint: ask the parent widget to recalculate the layout.
  */
void KDChart::AbstractLayoutItem::sizeHintChanged()const
{
    // This is exactly like what QWidget::updateGeometry does.
    if( mParent ) {
        if ( mParent->layout() )
            mParent->layout()->invalidate();
        else
            QApplication::postEvent( mParent, new QEvent( QEvent::LayoutRequest ) );
    }
}


KDChart::TextLayoutItem::TextLayoutItem( const QString& text,
                                         const KDChart::TextAttributes& attributes,
                                         const QWidget* area,
                                         KDChartEnums::MeasureOrientation orientation,
                                         Qt::Alignment alignment )
    : AbstractLayoutItem( alignment )
    , mText( text )
    , mAttributes( attributes )
    , mAutoReferenceArea( area )
    , mAutoReferenceOrientation(  orientation)
    , cachedSizeHint( QSize() ) // default this to invalid to force real-time calculation before first use of sizeHint()
    , cachedFontSize( 0.0 )
    , cachedFont( attributes.font() )
{
}


void KDChart::TextLayoutItem::setText(const QString & text)
{
    mText = text;
    cachedSizeHint = QSize();
}

QString KDChart::TextLayoutItem::text() const
{
    return mText;
}

KDChart::TextAttributes KDChart::TextLayoutItem::attributes() const
{
    return mAttributes;
}


Qt::Orientations KDChart::TextLayoutItem::expandingDirections() const
{
    return 0; // Grow neither vertically nor horizontally
}

QRect KDChart::TextLayoutItem::geometry() const
{
    return mRect;
}

bool KDChart::TextLayoutItem::isEmpty() const
{
    return false; // never empty, otherwise the layout item would not exist
}

QSize KDChart::TextLayoutItem::maximumSize() const
{
    return sizeHint(); // PENDING(kalle) Review, quite inflexible
}

QSize KDChart::TextLayoutItem::minimumSize() const
{
    return sizeHint(); // PENDING(kalle) Review, quite inflexible
}

void KDChart::TextLayoutItem::setGeometry( const QRect& r )
{
    mRect = r;
}


qreal KDChart::TextLayoutItem::realFontSize() const
{
    return qMax(
        mAttributes.fontSize().calculatedValue(
            mAutoReferenceArea,
            mAutoReferenceOrientation ),
        mAttributes.minimalFontSize().calculatedValue(
            mAutoReferenceArea,
            mAutoReferenceOrientation ) );
}


bool KDChart::TextLayoutItem::realFontWasRecalculated() const
{
    const qreal fntSiz = realFontSize();
    bool bRecalcDone =
        ( ( ! cachedSizeHint.isValid() ) ||
          ( cachedFontSize != fntSiz   ) );

    if( bRecalcDone &&  fntSiz > 0.0 ){
        cachedFontSize = fntSiz;
        cachedFont.setPointSizeF( fntSiz );
    }
    return bRecalcDone;
}


QFont KDChart::TextLayoutItem::realFont() const
{
    realFontWasRecalculated(); // we can safely ignore the boolean return value
    return cachedFont;
}


QSize KDChart::TextLayoutItem::sizeHint() const
{
    if( realFontWasRecalculated() ){
        cachedSizeHint = calcSizeHint( cachedFont );
        sizeHintChanged();
    }
    //qDebug("cachedSizeHint.width(): %i",cachedSizeHint.width());
    return cachedSizeHint;
}


// PENDING(kalle) Support auto rotate
// PENDING(kalle) Support auto shrink
// PENDING(kalle) Support rotation


QSize KDChart::TextLayoutItem::calcSizeHint( QFont fnt ) const
{
    const QFontMetricsF met( fnt );
    QSize ret = met.boundingRect( mText ).toRect().size();
    const int frame = QApplication::style()->pixelMetric( QStyle::PM_ButtonMargin, 0, 0 );
    ret += QSize( frame, frame );
    return ret;
}

void KDChart::TextLayoutItem::paint( QPainter* painter )
{
    // make sure, cached font is updated, if needed:
    sizeHint();

    if( !mRect.isValid() )
        return;

    PainterSaver painterSaver( painter );
    painter->setFont( cachedFont );
    painter->setPen(  mAttributes.pen() );
    painter->drawText( geometry(), Qt::AlignHCenter|Qt::AlignVCenter, mText );
}



KDChart::MarkerLayoutItem::MarkerLayoutItem( KDChart::AbstractDiagram* diagram,
                                             const MarkerAttributes& marker,
                                             const QBrush& brush, const QPen& pen,
                                             Qt::Alignment alignment )
    : AbstractLayoutItem( alignment )
    , mDiagram( diagram )
    , mMarker( marker )
    , mBrush( brush )
    , mPen( pen )
{
}

Qt::Orientations KDChart::MarkerLayoutItem::expandingDirections() const
{
    return 0; // Grow neither vertically nor horizontally
}

QRect KDChart::MarkerLayoutItem::geometry() const
{
    return mRect;
}

bool KDChart::MarkerLayoutItem::isEmpty() const
{
    return false; // never empty, otherwise the layout item would not exist
}

QSize KDChart::MarkerLayoutItem::maximumSize() const
{
    return sizeHint(); // PENDING(kalle) Review, quite inflexible
}

QSize KDChart::MarkerLayoutItem::minimumSize() const
{
    return sizeHint(); // PENDING(kalle) Review, quite inflexible
}

void KDChart::MarkerLayoutItem::setGeometry( const QRect& r )
{
    mRect = r;
}

QSize KDChart::MarkerLayoutItem::sizeHint() const
{
    return mMarker.markerSize().toSize();
}

void KDChart::MarkerLayoutItem::paint( QPainter* painter )
{
    if( !mRect.isValid() )
        return;

    // The layout management may assign a larger rect than what we
    // wanted. We need to adjust the position.
    QPointF pos = geometry().topLeft();
    pos += QPointF( static_cast<qreal>(( geometry().width() - sizeHint().width()) / 2.0 ),
                    static_cast<qreal>(( geometry().height() - sizeHint().height()) / 2.0 ) );

    // And finally, drawMarker() assumes the position to be the center
    // of the marker, adjust again.
    pos += QPointF( static_cast<qreal>( sizeHint().width() ) / 2.0,
                    static_cast<qreal>( sizeHint().height() )/ 2.0 );

    mDiagram->paintMarker( painter, mMarker, mBrush, mPen, pos.toPoint(), sizeHint() );
}


KDChart::HorizontalLineLayoutItem::HorizontalLineLayoutItem() :
    AbstractLayoutItem( Qt::AlignCenter )
{
}

Qt::Orientations KDChart::HorizontalLineLayoutItem::expandingDirections() const
{
    return Qt::Vertical|Qt::Horizontal; // Grow neither vertically nor horizontally
}

QRect KDChart::HorizontalLineLayoutItem::geometry() const
{
    return mRect;
}

bool KDChart::HorizontalLineLayoutItem::isEmpty() const
{
    return false; // never empty, otherwise the layout item would not exist
}

QSize KDChart::HorizontalLineLayoutItem::maximumSize() const
{
    return QSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
}

QSize KDChart::HorizontalLineLayoutItem::minimumSize() const
{
    return QSize( 0, 0 );
}

void KDChart::HorizontalLineLayoutItem::setGeometry( const QRect& r )
{
    mRect = r;
}

QSize KDChart::HorizontalLineLayoutItem::sizeHint() const
{
    return QSize( -1, 3 ); // see qframe.cpp
}


void KDChart::HorizontalLineLayoutItem::paint( QPainter* painter )
{
    if( !mRect.isValid() )
        return;

    painter->drawLine( QPointF( mRect.left(), mRect.center().y() ),
                       QPointF( mRect.right(), mRect.center().y() ) );
}


KDChart::VerticalLineLayoutItem::VerticalLineLayoutItem() :
    AbstractLayoutItem( Qt::AlignCenter )
{
}

Qt::Orientations KDChart::VerticalLineLayoutItem::expandingDirections() const
{
    return Qt::Vertical|Qt::Vertical; // Grow neither vertically nor horizontally
}

QRect KDChart::VerticalLineLayoutItem::geometry() const
{
    return mRect;
}

bool KDChart::VerticalLineLayoutItem::isEmpty() const
{
    return false; // never empty, otherwise the layout item would not exist
}

QSize KDChart::VerticalLineLayoutItem::maximumSize() const
{
    return QSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );
}

QSize KDChart::VerticalLineLayoutItem::minimumSize() const
{
    return QSize( 0, 0 );
}

void KDChart::VerticalLineLayoutItem::setGeometry( const QRect& r )
{
    mRect = r;
}

QSize KDChart::VerticalLineLayoutItem::sizeHint() const
{
    return QSize( 3, -1 ); // see qframe.cpp
}


void KDChart::VerticalLineLayoutItem::paint( QPainter* painter )
{
    if( !mRect.isValid() )
        return;

    painter->drawLine( QPointF( mRect.center().x(), mRect.top() ),
                       QPointF( mRect.center().x(), mRect.bottom() ) );
}

