/* -*- Mode: C++ -*-
   KDChart1 - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart1 library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#include "KDChart1TextPiece.h"

//#include <qstylesheet.h>
//#include <qsimplerichtext.h>
#include <qfontmetrics.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qrect.h>

KDChart1TextPiece::KDChart1TextPiece()
    :QObject(0)
{
    _isRichText = false;
    /*_richText = 0;*/

    _font = QApplication::font();
    // These three are needed for both
    _metrics = new QFontMetrics( _font );
    _dirtyMetrics = true;
    _text = QString("");
}


KDChart1TextPiece::KDChart1TextPiece( const QString& text, const QFont& font )
    :QObject(0)
{
 /*   if( QStyleSheet::mightBeRichText( text ) ) {
        _isRichText = true;
        _richText = new QSimpleRichText( text, font );
        _richText->adjustSize();
} else */{
        _isRichText = false;
        /*_richText = 0;*/
    }

    // These three are needed for both
    _metrics = new QFontMetrics( font );
    _dirtyMetrics = true;
    _text = text;
    _font = font;
}


KDChart1TextPiece::KDChart1TextPiece( QPainter *p, const QString& text, const QFont& font )
    :QObject(0)
{

    /*if( QStyleSheet::mightBeRichText( text ) ) {
        _isRichText = true;
        _richText = new QSimpleRichText( text, font );
        //qDebug( "richtext width %s", QString::number(_richText->width()).latin1());
	//qDebug( "richtext height %s", QString::number(_richText->height()).latin1());
        _richText->adjustSize();
        //qDebug( "richtext width %s", QString::number(_richText->width()).latin1());
	//qDebug( "richtext height %s", QString::number(_richText->height()).latin1());

} else */{
        _isRichText = false;
        /*_richText = 0;*/
    }

    // These three are needed for both
    _dirtyMetrics = (p == 0);
    if( _dirtyMetrics ) {
        _metrics = new QFontMetrics( font );
        //qDebug("dirty metrics text: %s", text.latin1());
    }
    else{
        p->save();
        p->setFont( font );
        _metrics = new QFontMetrics( p->fontMetrics() );
        //qDebug ( "drawing metrics text: %s", text.latin1() );
        //p->drawRect( _metrics->boundingRect( text) );
	//p->drawText( _metrics->boundingRect(text).bottomRight(), text);
        p->restore();
    }
    _text = text;
    _font = font;
}


void KDChart1TextPiece::deepCopy( const KDChart1TextPiece* source )
{
    if( !source || this == source )
        return;
/*    if( _richText )
    delete _richText;*/
    _isRichText = source->_isRichText;
    /*if( source->_richText ) {
        _richText = new QSimpleRichText( source->_text, source->_font );
        _richText->adjustSize();
    }
    else
    _richText = 0;*/

    // used for both
    if( _metrics )
        delete _metrics;
    _metrics = new QFontMetrics( *source->_metrics );
    _dirtyMetrics = source->_dirtyMetrics;
    _text = source->_text;
    _font = source->_font;
}

const KDChart1TextPiece* KDChart1TextPiece::clone() const
{
    KDChart1TextPiece* newPiece = new KDChart1TextPiece();
    newPiece->deepCopy( this );
    return newPiece;
}


KDChart1TextPiece::~KDChart1TextPiece()
{
    /*if( _richText )
    delete _richText;*/
    if( _metrics )
        delete _metrics;
}


int KDChart1TextPiece::width() const
{
    /*if( _isRichText )
        return _richText->widthUsed();
    else*/
        return _metrics->width( _text );
}


int KDChart1TextPiece::height() const
{

  /*if( _isRichText ) {
    //qDebug ("_richText height %s", QString::number(_richText->height()).latin1());
        return _richText->height();
  }
    else {*/

    //qDebug ("_metrics height %s", QString::number(_metrics->height()).latin1());
        return _metrics->height();
	 //}
}


int KDChart1TextPiece::fontLeading() const
{
    return _metrics->leading();
}

QRect KDChart1TextPiece::rect( QPainter *p, const QRect& clipRect) const
{
    QRect rect( clipRect );
    QFont font( _font );

    Q_UNUSED(p);

    /*if( _isRichText ) {

    // Pending Michel make sure the fonts are not too large
      if ( _richText->height() > clipRect.height() || _richText->width() > clipRect.width() )
	font.setPixelSize( qMin( (int)clipRect.width(),(int)clipRect.height() ) );

      _richText->setDefaultFont( font );
      _richText->setWidth( p, clipRect.width() );
      rect.setWidth( _richText->width() );
      rect.setHeight( _richText->height() );
} else */
      rect = clipRect;

    return rect;
}

void KDChart1TextPiece::draw( QPainter *p, int x, int y,
        const QRect& clipRect,
        const QColor& color,
        const QBrush* paper ) const
{

    Q_UNUSED(clipRect);
  /*if( _isRichText ) {
    QColorGroup cg;
    //calculate the text area before drawing
    QRect txtArea = rect( p,clipRect);
    QRect rect;
    cg.setColor( QColorGroup::Text, color );
    // adjust the vertical position of the text within the area - we send a null rectangle to avoid clipping
    //PENDING: Michel - TODO - Let the user set or unset the adjustment factor by himself
   _richText->draw( p, txtArea.x(), txtArea.y() + (int)txtArea.height()/10 , rect, cg, paper );
} else */{
    p->save();
    p->setFont( _font );
    if( paper )
      p->setBrush( *paper );
    p->setPen( color );
    //dont clip to avoid truncated text
    //p->setClipRect( txtArea );
    if( _dirtyMetrics ){
      if( _metrics )
	delete _metrics;
      KDChart1TextPiece* meNotConst = const_cast<KDChart1TextPiece*>(this);
      //KDChart1TextPiece* meNotConst(const_cast<KDChart1TextPiece*>(this));
      meNotConst->_metrics = new QFontMetrics( p->fontMetrics() );
      meNotConst->_dirtyMetrics = false;
    }

    p->drawText( x, y + _metrics->ascent(), _text );
    p->restore();
    }
}


void KDChart1TextPiece::draw( QPainter *p, int x, int y,
        const QRegion& clipRegion,
        const QColor& color,
        const QBrush* paper ) const
{
    /*if( _isRichText ) {
        QColorGroup cg;
        cg.setColor( QColorGroup::Text, color );
        _richText->setDefaultFont( _font );
        _richText->setWidth( p, clipRegion.boundingRect().width() );
        _richText->draw( p, x, y, clipRegion, cg, paper );
} else */{
        p->save();
        p->setFont( _font );
        if( paper )
            p->setBrush( *paper );
        p->setPen( color );
        p->setClipRegion( clipRegion );

        if( _dirtyMetrics ){
            if( _metrics )
                delete _metrics;

            // this line does not compile with MSVC++:
            // KDChart1TextPiece* meNotConst( const_cast<KDChart1TextPiece*>(this) );
            KDChart1TextPiece* meNotConst = const_cast<KDChart1TextPiece*>(this);

            meNotConst->_metrics = new QFontMetrics( p->fontMetrics() );
            meNotConst->_dirtyMetrics = false;
        }

        p->drawText( x, y + _metrics->ascent(), _text );
        p->restore();
    }
}


QString KDChart1TextPiece::text() const
{
    return _text;
}


QFont KDChart1TextPiece::font() const
{
    return _font;
}


bool KDChart1TextPiece::isRichText() const
{
    return false;//_isRichText;
}


