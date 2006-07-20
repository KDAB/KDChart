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

#include "KDChartChart.h"
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

//FIXME(khz): use an internal libfakes library instead of this internal haeder file
#include "kdchart_platformdef.h"

using namespace KDChart;

HeaderFooter::Private::Private( Chart* parent_ ) :
    parent( parent_ ),
    type( Header ),
    position( Position::North ),
    text( QObject::tr( "Header/Footer" ) ),
    textAttrs(),
    textDoc( 0 ),
    cachedSize(0,0)
{
}

HeaderFooter::Private::~Private()
{
    delete textDoc;
}

void HeaderFooter::Private::updateTextDoc()
{
    qreal fntSize = qMax(
        textAttrs.fontSize().calculatedValue(
            parent,
            KDChartEnums::MeasureOrientationMinimum ),
        textAttrs.minimalFontSize().calculatedValue(
            parent,
            KDChartEnums::MeasureOrientationMinimum ) );
    QFont fnt( textAttrs.font() );
    fnt.setPointSizeF( fntSize );
    if( textDoc )
        delete textDoc;
    textDoc = new KDTextDocument;
    textDoc->setDefaultFont( fnt );
    QTextCursor cursor( textDoc );

    // PENDING(kalle) Other attributes!
    if( !text.isEmpty() ) {
        QTextBlockFormat bf;
        bf.setNonBreakableLines( true );
        cursor.insertBlock( bf );
        cursor.insertFragment( QTextDocumentFragment::fromHtml( text ) );
    }
}

#define d d_func()

HeaderFooter::HeaderFooter( Chart* parent ) :
    AbstractArea( new Private( parent ) )
{
    init();
}

HeaderFooter::~HeaderFooter()
{
    emit destroyedHeaderFooter( this );
}

void HeaderFooter::init()
{
    TextAttributes ta;
    ta.setPen( QPen(Qt::black) );
    ta.setFont( QFont( "helvetica", 10, QFont::Bold, false ) );

    Measure m( 35.0 );
    m.setRelativeMode( d->parent, KDChartEnums::MeasureOrientationMinimum );
    ta.setFontSize( m );

    m.setValue( 8.0 );
    m.setCalculationMode( KDChartEnums::MeasureCalculationModeAbsolute );
    ta.setMinimalFontSize( m  );

    setTextAttributes( ta );
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


void HeaderFooter::paint( QPainter* painter )
{
    if( ! d->textAttrs.hasAbsoluteFontSize() && d->parent->size() != d->cachedSize ){
        d->cachedSize = d->parent->size();
        d->updateTextDoc();
    }

//    Q_ASSERT( d->textDoc );
    if( !d->textDoc )
        return;

    QAbstractTextDocumentLayout* layout = d->textDoc->documentLayout();
    d->textDoc->setPageSize( geometry().size() );

//    painter->drawRect( rect().adjusted( 0, 0, -1, -1 ) );
    painter->setPen( textAttributes().pen() );

    if( textAttributes().isVisible() ) {
        QAbstractTextDocumentLayout::PaintContext ctx;
        ctx.palette.setColor( QPalette::Text, textAttributes().pen().color() );
        layout->draw( painter, ctx );
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
    d->updateTextDoc();
    update();
}

QString HeaderFooter::text() const
{
    return d->text;
}

/**
  \brief Use this to specify the text attributes to be used for this header
  (or footer, resp.).

  By default, the reference area will be set at painting time.
  It will be the parent widget of this header/footer,
  so normally, it will be the KDChart::Chart.
  Thus all of your headers, and all of your footers
  within that Chart will be drawn in same font size, by default.

  \sa textAttributes, setLabels
*/
void HeaderFooter::setTextAttributes( const TextAttributes &a )
{
    d->textAttrs = a;
    d->updateTextDoc();
    update();
}

/**
  Returns the text attributes to be used for this header (or footer, resp.).

  \sa setTextAttributes
*/
TextAttributes HeaderFooter::textAttributes() const
{
    return d->textAttrs;
}

void HeaderFooter::setParent( Chart* parent )
{
    d->parent = parent;
}

/* pure virtual in QLayoutItem */
bool HeaderFooter::isEmpty() const
{
    return ( d->textDoc != 0 );
}
/* pure virtual in QLayoutItem */
Qt::Orientations HeaderFooter::expandingDirections() const
{
    return Qt::Vertical | Qt::Horizontal;
}
/* pure virtual in QLayoutItem */
QSize HeaderFooter::maximumSize() const
{
    QSize s( d->textDoc ? d->textDoc->sizeHint() : QSize(0,0) );
    qDebug() << "KDChart::HeaderFooter::maximumSize() returning" << s;
    return s;
}
/* pure virtual in QLayoutItem */
QSize HeaderFooter::minimumSize() const
{
    return maximumSize();
}
/* pure virtual in QLayoutItem */
QSize HeaderFooter::sizeHint() const
{
    return maximumSize();
}
/* pure virtual in QLayoutItem */
void HeaderFooter::setGeometry( const QRect& r )
{
    qDebug() << "KDChart::HeaderFooter::setGeometry(" << r << ") called";
    d->geometry = r;
    if( d->textDoc )
        d->textDoc->setPageSize( QSize( r.size() ) );
}
/* pure virtual in QLayoutItem */
QRect HeaderFooter::geometry() const
{
    QRect r( d->geometry.topLeft(), QSize(1,1) );
    if( d->textDoc ){
        const QSizeF siz( d->textDoc->pageSize() );
        r.setSize(
            QSize( static_cast<int>(siz.width()),
                   static_cast<int>(siz.height()) ) );
    }
    r.setSize( sizeHint() );
    qDebug() << "KDChart::HeaderFooter::geometry() returning" << r;
    return r;
}

void HeaderFooter::update()
{
    //qDebug("KDChart::HeaderFooter::update() called");
    if( d->parent )
        d->parent->update();
}
