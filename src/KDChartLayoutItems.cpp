#include "KDChartLayoutItems.h"
#include "KDTextDocument.h"
#include "KDChartAbstractDiagram.h"
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextDocumentFragment>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QDebug>
#include <QCoreApplication>
#include <QApplication>
#include <QStyle>

KDChart::TextLayoutItem::TextLayoutItem( const QString& text, const QFont& font,
                                         Qt::Alignment alignment ) :
    LayoutItem( alignment ), mText( text ), mFont( font ), mMetrics( mFont )
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
    QSize ret = mMetrics.size( 0, mText ).toSize();
    int frame = QApplication::style()->pixelMetric( QStyle::PM_ButtonMargin, 0, 0 );
    ret += QSize( frame, frame );
    return ret;
}

void KDChart::TextLayoutItem::paint( QPainter* painter )
{
    Q_ASSERT( mRect.isValid() );

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
    return QSize( 16, 16 ); // PENDING(kalle) Variable?
}

// PENDING(kalle) Marker properties


void KDChart::MarkerLayoutItem::paint( QPainter* painter )
{
    Q_ASSERT( mRect.isValid() );

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

