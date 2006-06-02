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

#include "KDChartHeaderFooter.h"
#include "KDChartHeaderFooter_p.h"
#include <QtXml/QDomDocumentFragment>
#include <KDChartTextAttributes.h>
#include <QFont>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QtDebug>
#include <QLabel>
#include "KDTextDocument.h"

using namespace KDChart;

HeaderFooter::Private::Private() :
    type( Header ),
    position( Position::North ),
    text( QObject::tr( "Header/Footer" ) ),
    textAttributes(),
    textDoc( 0 )
{
}

HeaderFooter::Private::~Private()
{
}



#define d d_func()

HeaderFooter::HeaderFooter( QWidget* parent ) :
    AbstractArea( new Private(), parent  )
{
}

HeaderFooter::~HeaderFooter()
{
    emit destroyedHeaderFooter( this );
}

void HeaderFooter::init()
{
    TextAttributes textAttrs;
    textAttrs.setColor( Qt::black );
    textAttrs.setFont( QFont( "helvetica", 10, QFont::Bold, false ) );
    textAttrs.setFontSize( 16 );
    setTextAttributes( textAttrs );
}

QDomDocumentFragment HeaderFooter::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment HeaderFooter::toXML() const" );
    return QDomDocumentFragment();
}

HeaderFooter * HeaderFooter::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: HeaderFooter * HeaderFooter::clone() const" );
    return (HeaderFooter*)0xdeadbeef;
}

void HeaderFooter::paintEvent( QPaintEvent* evt )
{
    // Paint the background and frame first
    AbstractArea::paintEvent( evt );

//    Q_ASSERT( d->textDoc );
    if( !d->textDoc )
        return;

    QPainter painter( this );

    QAbstractTextDocumentLayout* layout = d->textDoc->documentLayout();
    d->textDoc->setPageSize( size() );

//    painter.drawRect( rect().adjusted( 0, 0, -1, -1 ) );
    painter.setPen( textAttributes().color() );

    if( textAttributes().isVisible() ) {
        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.palette.setColor( QPalette::Text, textAttributes().color() );
        layout->draw( &painter, ctx );
    }

    // PENDING(kalle) Support relative size
    // PENDING(kalle) Support minimal size
    // PENDING(kalle) Support auto rotate
    // PENDING(kalle) Support auto shrink
    // PENDING(kalle) Support rotation
}

void HeaderFooter::setType( HeaderFooterType type )
{
    d->type = type;
    emit positionChanged( this );
}

HeaderFooter::HeaderFooterType HeaderFooter::type() const
{
    return d->type;
}

void HeaderFooter::setPosition( Position position )
{
    d->position = position;
    emit positionChanged( this );
}

Position HeaderFooter::position() const
{
    return d->position;
}

void HeaderFooter::setText( const QString& text )
{
    d->text = text;
}

QString HeaderFooter::text() const
{
    return d->text;
}

void HeaderFooter::setTextAttributes( const TextAttributes &a )
{
    d->textAttributes = a;
}

TextAttributes HeaderFooter::textAttributes() const
{
    return d->textAttributes;
}

QSize HeaderFooter::sizeHint() const
{
    if( d->textDoc )
        delete d->textDoc;
    d->textDoc = new KDTextDocument;
    d->textDoc->setDefaultFont( d->textAttributes.font() );
    QTextCursor cursor( d->textDoc );

    // PENDING(kalle) Other attributes!
    if( !text().isEmpty() ) {
        QTextBlockFormat bf;
        bf.setNonBreakableLines( true );
        cursor.insertBlock( bf );
        cursor.insertFragment( QTextDocumentFragment::fromHtml( text() ) );
    }

    return d->textDoc->sizeHint();
}


