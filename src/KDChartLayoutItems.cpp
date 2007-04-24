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
#include <QStringList>
#include <QStyle>

#include <KDABLibFakes>

#include <math.h>

#define PI 3.141592653589793



//#define DEBUG_ITEMS_PAINT

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
void KDChart::AbstractLayoutItem::paintCtx( PaintContext* context )
{
    if( context )
        paint( context->painter() );
}

/**
    Report changed size hint: ask the parent widget to recalculate the layout.
  */
void KDChart::AbstractLayoutItem::sizeHintChanged()const
{
    // This is exactly like what QWidget::updateGeometry does.
//  qDebug("KDChart::AbstractLayoutItem::sizeHintChanged() called");
    if( mParent ) {
        if ( mParent->layout() )
            mParent->layout()->invalidate();
        else
            QApplication::postEvent( mParent, new QEvent( QEvent::LayoutRequest ) );
    }
}


KDChart::TextLayoutItem::TextLayoutItem( const QString& text,
                                         const KDChart::TextAttributes& attributes,
                                         const QObject* area,
                                         KDChartEnums::MeasureOrientation orientation,
                                         Qt::Alignment alignment )
    : AbstractLayoutItem( alignment )
    , mText( text )
    , mAttributes( attributes )
    , mAutoReferenceArea( area )
    , mAutoReferenceOrientation( orientation )
    , cachedSizeHint() // default this to invalid to force just-in-time calculation before first use of sizeHint()
    , cachedFontSize( 0.0 )
    , cachedFont( mAttributes.font() )
{
}

KDChart::TextLayoutItem::TextLayoutItem()
    : AbstractLayoutItem( Qt::AlignLeft )
    , mText()
    , mAttributes()
    , mAutoReferenceArea( 0 )
    , mAutoReferenceOrientation( KDChartEnums::MeasureOrientationHorizontal )
    , cachedSizeHint() // default this to invalid to force just-in-time calculation before first use of sizeHint()
    , cachedFontSize( 0.0 )
    , cachedFont( mAttributes.font() )
{

}

void KDChart::TextLayoutItem::setAutoReferenceArea( const QObject* area )
{
    mAutoReferenceArea = area;
    cachedSizeHint = QSize();
    sizeHint();
}

const QObject* KDChart::TextLayoutItem::autoReferenceArea() const
{
    return mAutoReferenceArea;
}

void KDChart::TextLayoutItem::setText(const QString & text)
{
    mText = text;
    cachedSizeHint = QSize();
    sizeHint();
}

QString KDChart::TextLayoutItem::text() const
{
    return mText;
}

/**
  \brief Use this to specify the text attributes to be used for this item.

  \sa textAttributes
*/
void KDChart::TextLayoutItem::setTextAttributes( const TextAttributes &a )
{
    mAttributes = a;
    cachedSizeHint = QSize(); // invalidate size hint
    sizeHint();
}

/**
  Returns the text attributes to be used for this item.

  \sa setTextAttributes
*/
KDChart::TextAttributes KDChart::TextLayoutItem::textAttributes() const
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
    return mAttributes.calculatedFontSize( mAutoReferenceArea, mAutoReferenceOrientation );
}


bool KDChart::TextLayoutItem::realFontWasRecalculated() const
{
    const qreal fntSiz = realFontSize();
    const bool bRecalcDone =
        ( ( ! cachedSizeHint.isValid() ) || ( cachedFontSize != fntSiz   ) );

    if( bRecalcDone && fntSiz > 0.0 ){
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

QPolygon KDChart::TextLayoutItem::rotatedCorners() const
{
    // the angle in rad
    const qreal angle = mAttributes.rotation() * PI / 180.0;
    QSize size = unrotatedSizeHint();

    // my P1 - P4 (the four points of the rotated area)
    QPointF P1( size.height() * sin( angle ), 0 );
    QPointF P2( size.height() * sin( angle ) + size.width() * cos( angle ), size.width() * sin( angle ) );
    QPointF P3( size.width() * cos( angle ), size.width() * sin( angle ) + size.height() * cos( angle ) );
    QPointF P4( 0, size.height() * cos( angle ) );

    QPolygon result;
    result << P1.toPoint() << P2.toPoint() << P3.toPoint() << P4.toPoint();
    return result;
}

bool KDChart::TextLayoutItem::intersects( const TextLayoutItem& other, const QPointF& myPos, const QPointF& otherPos ) const
{
    return intersects( other, myPos.toPoint(), otherPos.toPoint() );
}

bool KDChart::TextLayoutItem::intersects( const TextLayoutItem& other, const QPoint& myPos, const QPoint& otherPos ) const
{
    if ( mAttributes.rotation() != other.mAttributes.rotation() )
    {
        // that's the code for the common case: the rotation angles don't need to match here
        QPolygon myPolygon(          rotatedCorners() );
        QPolygon otherPolygon( other.rotatedCorners() );

        // move the polygons to their positions
        myPolygon.translate( myPos );
        otherPolygon.translate( otherPos );

        // create regions out of it
        QRegion myRegion( myPolygon );
        QRegion otherRegion( otherPolygon );

        // now the question - do they intersect or not?
        return ! myRegion.intersect( otherRegion ).isEmpty();

    } else {
        // and that's the code for the special case: the rotation angles match, which is less time consuming in calculation
        const qreal angle = mAttributes.rotation() * PI / 180.0;
        // both sizes
        const QSizeF mySize(          unrotatedSizeHint() );
        const QSizeF otherSize( other.unrotatedSizeHint() );

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

QSize KDChart::TextLayoutItem::sizeHint() const
{
    if( realFontWasRecalculated() )
    {
        const QSize newSizeHint( calcSizeHint( cachedFont ) );
        if( newSizeHint != cachedSizeHint ){
            cachedSizeHint = newSizeHint;
            sizeHintChanged();
        }
    }
    //qDebug() << "-------- KDChart::TextLayoutItem::sizeHint() returns:"<<cachedSizeHint<<" ----------";
    return cachedSizeHint;
}


// PENDING(kalle) Support auto shrink


QSize KDChart::TextLayoutItem::unrotatedSizeHint( QFont fnt ) const
{
    if ( fnt == QFont() )
        fnt = cachedFont;

    const QFontMetricsF met( fnt, mParent );
    QSize ret(0, 0);
    // note: boundingRect() does NOT take any newlines into account
    //       so we need to calculate the size by combining several
    //       rectangles: one per line.  This fixes bugz issue #3720.
    //       (khz, 2007 04 14)
    QStringList lines = mText.split(QString::fromAscii("\n"));
    for (int i = 0; i < lines.size(); ++i){
        const QSize lSize = met.boundingRect(lines.at(i) ).toRect().size();
        ret.setWidth(qMax( ret.width(), lSize.width() ));
        ret.rheight() += lSize.height();
    }

    int frame = QApplication::style()->pixelMetric( QStyle::PM_ButtonMargin, 0, 0 );
    // fine-tuning for small font sizes: the frame must not be so big, if the font is tiny
    frame = qMin( frame, ret.height() * 2 / 3 );
    //qDebug() << "frame:"<< frame;
    ret += QSize( frame, frame );
    return ret;
    //const QFontMetricsF met( fnt, mParent );
    //const int frame = QApplication::style()->pixelMetric( QStyle::PM_ButtonMargin, 0, 0 );
    //return
    //    met.boundingRect( mText ).size().toSize() + QSize( frame, frame );
}


QSize KDChart::TextLayoutItem::calcSizeHint( QFont fnt ) const
{
    QSize ret = unrotatedSizeHint( fnt );
    //qDebug() << "-------- "<<ret.width();
    const qreal angle = PI * mAttributes.rotation() / 180.0;
    QSize rotated( static_cast<int>( cos( angle ) * ret.width()  + sin( angle ) * ret.height() ),
                   static_cast<int>( cos( angle ) * ret.height() + sin( angle ) * ret.width() ) );
    rotated.setWidth( qMax( rotated.height(), rotated.width() ) );
    //qDebug() << "-------- KDChart::TextLayoutItem::calcSizeHint() returns:"<<rotated<<" ----------";
    return rotated;
}

void KDChart::TextLayoutItem::paint( QPainter* painter )
{
    // make sure, cached font is updated, if needed:
    // sizeHint();

    if( !mRect.isValid() )
        return;

    PainterSaver painterSaver( painter );
    painter->setFont( cachedFont );
    painter->setPen( mAttributes.pen() );
    QRectF rect( geometry() );

    painter->translate( rect.center() );
    rect.moveTopLeft( QPointF( - rect.width() / 2, - rect.height() / 2 ) );
    painter->rotate( mAttributes.rotation() );
#ifdef DEBUG_ITEMS_PAINT
    painter->setPen( Qt::red );
    painter->drawRect( rect );
    painter->setPen( mAttributes.pen() );
#endif
    painter->drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter, mText );
//    if (  calcSizeHint( cachedFont ).width() > rect.width() )
//        qDebug() << "rect.width()" << rect.width() << "text.width()" << calcSizeHint( cachedFont ).width();
//
//    //painter->drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter, mText );
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
    //qDebug() << "KDChart::MarkerLayoutItem::sizeHint() returns:"<<mMarker.markerSize().toSize();
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

#ifdef DEBUG_ITEMS_PAINT
    QPointF oldPos = pos;
#endif

// And finally, drawMarker() assumes the position to be the center
    // of the marker, adjust again.
    pos += QPointF( static_cast<qreal>( sizeHint().width() ) / 2.0,
                    static_cast<qreal>( sizeHint().height() )/ 2.0 );

    mDiagram->paintMarker( painter, mMarker, mBrush, mPen, pos.toPoint(), sizeHint() );

#ifdef DEBUG_ITEMS_PAINT
    const QPen oldPen( painter->pen() );
    painter->setPen( Qt::red );
    painter->drawRect( QRect(oldPos.toPoint(), sizeHint()) );
    painter->setPen( oldPen );
#endif
}


KDChart::HorizontalLineLayoutItem::HorizontalLineLayoutItem()
    : AbstractLayoutItem( Qt::AlignCenter )
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


KDChart::VerticalLineLayoutItem::VerticalLineLayoutItem()
    : AbstractLayoutItem( Qt::AlignCenter )
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


KDChart::LineLayoutItem::LineLayoutItem( KDChart::AbstractDiagram* diagram,
                                         const QPen& pen,
                                         Qt::Alignment alignment )
    : AbstractLayoutItem( alignment )
    , mDiagram( diagram )
    , mPen( pen )
{
}

Qt::Orientations KDChart::LineLayoutItem::expandingDirections() const
{
    return 0; // Grow neither vertically nor horizontally
}

QRect KDChart::LineLayoutItem::geometry() const
{
    return mRect;
}

bool KDChart::LineLayoutItem::isEmpty() const
{
    return false; // never empty, otherwise the layout item would not exist
}

QSize KDChart::LineLayoutItem::maximumSize() const
{
    return sizeHint(); // PENDING(kalle) Review, quite inflexible
}

QSize KDChart::LineLayoutItem::minimumSize() const
{
    return sizeHint(); // PENDING(kalle) Review, quite inflexible
}

void KDChart::LineLayoutItem::setGeometry( const QRect& r )
{
    mRect = r;
}

QSize KDChart::LineLayoutItem::sizeHint() const
{
    return QSize( 10, -1 );
}

void KDChart::LineLayoutItem::paint( QPainter* painter )
{
    if( !mRect.isValid() )
        return;
    painter->save();
    painter->setPen( mPen );
    painter->drawLine( QPointF( mRect.left(), mRect.center().y() ),
                       QPointF( mRect.right(), mRect.center().y() ) );
    painter->restore();
}

