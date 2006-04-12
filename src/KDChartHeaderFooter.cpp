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
    position( North ),
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
    KDChartArea( new Private(), parent  )
{
}

HeaderFooter::~HeaderFooter()
{
}

void HeaderFooter::init()
{
    TextAttributes textAttrs;
    textAttrs.setColor( Qt::black );
    textAttrs.setFont( QFont( "helvetica", 10, QFont::Bold, false ) );
    textAttrs.setRelativeSize( 16 );
    textAttrs.setUseRelativeSize( true );
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
    KDChartArea::paintEvent( evt );

    QPainter painter( this );

    Q_ASSERT( d->textDoc );
    if( !d->textDoc )
        return;

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

void HeaderFooter::setPosition( HeaderFooterPosition position )
{
    d->position = position;
}

HeaderFooter::HeaderFooterPosition HeaderFooter::position() const
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


