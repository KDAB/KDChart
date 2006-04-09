#include "KDChartTextAttributes.h"
#include <QFont>
#include <QColor>
#include <QtXml/QDomDocumentFragment>
#include <qglobal.h>
#include <QApplication>

#define d d_func()

using namespace KDChart;

class TextAttributes::Private
{
    friend class TextAttributes;
public:
    Private();
private:
    bool visible;
    QFont font;
    bool useRelativeSize;
    int relativeSize;
    int minimalSize;
    bool autoRotate;
    bool autoShrink;
    int rotation;
    QColor color;
};

TextAttributes::Private::Private()
{
}


TextAttributes::TextAttributes()
    : _d( new Private() )
{
    setVisible( true );
    setFont( QApplication::font() );
    setUseRelativeSize( false );
    setRelativeSize( -1 );
    setMinimalSize( -1 );
    setAutoRotate( false );
    setAutoShrink( false );
    setRotation( 0 );
    setColor( Qt::black );
}

TextAttributes::TextAttributes( const TextAttributes& r )
    : _d( new Private( *r.d ) )
{

}

TextAttributes & TextAttributes::operator=( const TextAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

TextAttributes::~TextAttributes()
{
    delete _d; _d = 0;
}


bool TextAttributes::operator==( const TextAttributes& r ) const
{
    return ( isVisible() == r.isVisible() &&
	     font() == r.font() &&
	     useRelativeSize() == r.useRelativeSize() &&
	     relativeSize() == r.relativeSize() &&
	     minimalSize() == r.minimalSize() &&
	     autoRotate() == r.autoRotate() &&
	     autoShrink() == r.autoShrink() &&
	     rotation() == rotation() &&
	     color() == r.color() );
}


void TextAttributes::setVisible( bool visible )
{
    d->visible = visible;
}

bool TextAttributes::isVisible() const
{
    return d->visible;
}

void TextAttributes::setFont( const QFont& font )
{
    d->font = font;
}

QFont TextAttributes::font() const
{
    return d->font;
}

void TextAttributes::setUseRelativeSize( bool useRelativeSize )
{
    d->useRelativeSize = useRelativeSize;
}

bool TextAttributes::useRelativeSize() const
{
    return d->useRelativeSize;
}

void TextAttributes::setRelativeSize( int relativeSize )
{
    d->relativeSize = relativeSize;
}

int TextAttributes::relativeSize() const
{
    return d->relativeSize;
}

void TextAttributes::setMinimalSize( int minimalSize )
{
    d->minimalSize = minimalSize;
}

int TextAttributes::minimalSize() const
{
    return d->minimalSize;
}

void TextAttributes::setAutoRotate( bool autoRotate )
{
    d->autoRotate = autoRotate;
}

bool TextAttributes::autoRotate() const
{
    return d->autoRotate;
}

void TextAttributes::setAutoShrink( bool autoShrink )
{
    d->autoShrink = autoShrink;
}

bool TextAttributes::autoShrink() const
{
    return d->autoShrink;
}

void TextAttributes::setRotation( int rotation )
{
    d->rotation = rotation;
}

int TextAttributes::rotation() const
{
    return d->rotation;
}

void TextAttributes::setColor( const QColor& color )
{
    d->color = color;
}

QColor TextAttributes::color() const
{
    return d->color;
}

QDomDocumentFragment TextAttributes::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment TextAttributes::toXML() const" );
    return QDomDocumentFragment();
}






































