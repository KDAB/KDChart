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

#include "KDChartLayoutItems.h"
#include "KDTextDocument.h"
#include "KDChartAbstractDiagram.h"
#include "KDChartPainterSaver_p.h"
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextDocumentFragment>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include <QStyle>

KDChart::TextLayoutItem::TextLayoutItem( const QString& text, const QFont& font, const QColor& color,
                                         Qt::Alignment alignment ) :
    LayoutItem( alignment ), mText( text ), mFont( font ), mMetrics( mFont ), mColor( color )
{
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

QSize KDChart::TextLayoutItem::sizeHint() const
{
    if( ! cachedSizeHint.isValid() )
        cachedSizeHint = calcSizeHint();

    return cachedSizeHint;
}


// PENDING(kalle) Support relative size
// PENDING(kalle) Support minimal size
// PENDING(kalle) Support auto rotate
// PENDING(kalle) Support auto shrink
// PENDING(kalle) Support rotation


QSize KDChart::TextLayoutItem::calcSizeHint() const
{
    QSize ret = mMetrics.boundingRect( mText ).toRect().size();
    int frame = QApplication::style()->pixelMetric( QStyle::PM_ButtonMargin, 0, 0 );
    ret += QSize( frame, frame );
    return ret;
}

void KDChart::TextLayoutItem::paint( QPainter* painter )
{
    if( !mRect.isValid() )
        return;

    PainterSaver painterSaver( painter );
    painter->setFont( mFont );
    painter->setPen( mColor );
    painter->drawText( geometry(), Qt::AlignHCenter|Qt::AlignVCenter, mText );
}



KDChart::MarkerLayoutItem::MarkerLayoutItem( KDChart::AbstractDiagram* diagram,
                                             const MarkerAttributes& marker,
                                             const QBrush& brush, const QPen& pen,
                                             Qt::Alignment alignment ) :
    LayoutItem( alignment ), mDiagram( diagram ), mMarker( marker ), mBrush( brush ), mPen( pen )
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

// PENDING(kalle) Marker properties


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

