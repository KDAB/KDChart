#include "KDChartTextAttributes.h"
#include <QFont>
#include <QPen>
#include <qglobal.h>
#include <QApplication>
#include <QSharedPointer>
#include <QTextDocument>
#include <KDABLibFakes>
#include <KDChartCartesianCoordinatePlane>

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
    mutable QFont cachedFont;
    mutable qreal cachedFontSize;
    Measure fontSize;
    Measure minimalFontSize;
    bool autoRotate;
    bool autoShrink;
    int rotation;
    QPen pen;
    QSharedPointer<QTextDocument> document;
};

TextAttributes::Private::Private()
{
    cachedFontSize = -1.0;
}

TextAttributes::TextAttributes()
    : _d( new Private() )
{
    setVisible( true );
    setFont( QApplication::font() );
    setAutoRotate( false );
    setAutoShrink( false );
    setRotation( 0 );
    setPen( QPen( Qt::black ) );
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
    // the following works around a bug in gcc 4.3.2
    // causing StyleHint to be set to Zero when copying a QFont
    const QFont myFont( font() );
    QFont r_font( r.font() );
    r_font.setStyleHint( myFont.styleHint(), myFont.styleStrategy() );
    /*
    qDebug() << "\nTextAttributes::operator== :" << ( isVisible() == r.isVisible())
            << " font:"<<(myFont == r_font)
            << " fontSize:"<<(fontSize() == r.fontSize())
            << " minimalFontSize:"<<(minimalFontSize() == r.minimalFontSize())
            << (autoRotate() == r.autoRotate())
            << (autoShrink() == r.autoShrink())
            << (rotation() == rotation())
            << (pen() == r.pen());
    */
    return ( isVisible() == r.isVisible() &&
            myFont == r_font &&
            fontSize() == r.fontSize() &&
            minimalFontSize() == r.minimalFontSize() &&
            autoRotate() == r.autoRotate() &&
            autoShrink() == r.autoShrink() &&
            rotation() == r.rotation() &&
            pen() == r.pen() &&
            textDocument() == r.textDocument() );
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
    d->font       = font;
    d->cachedFont = font; // note: we do not set the font's size here, but in calculatedFont()
    //qDebug() << "resetting cached font size";
    d->cachedFontSize = -1.0;
}

QFont TextAttributes::font() const
{
    return d->font;
}

void TextAttributes::setFontSize( const Measure & measure )
{
    d->fontSize = measure;
}

Measure TextAttributes::fontSize() const
{
    return d->fontSize;
}

void TextAttributes::setMinimalFontSize( const Measure & measure )
{
    d->minimalFontSize = measure;
}

Measure TextAttributes::minimalFontSize() const
{
    return d->minimalFontSize;
}

bool TextAttributes::hasAbsoluteFontSize() const
{
    return d->fontSize.calculationMode() == KDChartEnums::MeasureCalculationModeAbsolute
        && d->minimalFontSize.calculationMode() == KDChartEnums::MeasureCalculationModeAbsolute;
}


#if QT_VERSION < 0x040400 || defined(Q_COMPILER_MANGLES_RETURN_TYPE)
const
#endif
qreal TextAttributes::calculatedFontSize(
        const QObject*                   autoReferenceArea,
        KDChartEnums::MeasureOrientation autoReferenceOrientation ) const
{
    const qreal normalSize  = fontSize().calculatedValue(        autoReferenceArea, autoReferenceOrientation );
    const qreal minimalSize = minimalFontSize().calculatedValue( autoReferenceArea, autoReferenceOrientation );
    //qDebug() << "TextAttributes::calculatedFontSize() finds" << normalSize << "and" << minimalSize;
    return qMax( normalSize, minimalSize );
}


const QFont TextAttributes::calculatedFont(
    const QObject*                   autoReferenceArea,
    KDChartEnums::MeasureOrientation autoReferenceOrientation ) const
{
    const CartesianCoordinatePlane* plane = dynamic_cast<const CartesianCoordinatePlane*>( autoReferenceArea );

    static qreal size = calculatedFontSize( autoReferenceArea, autoReferenceOrientation );
    if ( plane )
    {
        if(!plane->hasFixedDataCoordinateSpaceRelation())
            size = calculatedFontSize( autoReferenceArea, autoReferenceOrientation );
    }
    else
        size = calculatedFontSize( autoReferenceArea, autoReferenceOrientation );

    if( size > 0.0 && d->cachedFontSize != size ){
        //qDebug() << "new into the cache:" << size;
        d->cachedFontSize = size;
        d->cachedFont.setPointSizeF( d->cachedFontSize );
    }

    return d->cachedFont;
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

void TextAttributes::setPen( const QPen& pen )
{
    d->pen = pen;
}

QPen TextAttributes::pen() const
{
    return d->pen;
}

QTextDocument* TextAttributes::textDocument() const
{
    return d->document.data();
}

void TextAttributes::setTextDocument(QTextDocument* document)
{
    d->document = QSharedPointer<QTextDocument>(document);
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::TextAttributes& ta)
{
    dbg << "KDChart::TextAttributes("
    << "visible="<<ta.isVisible()
    << "font="<<ta.font().toString() /* What? No QDebug for QFont? */
    << "fontsize="<<ta.fontSize()
    << "minimalfontsize="<<ta.minimalFontSize()
    << "autorotate="<<ta.autoRotate()
    << "autoshrink="<<ta.autoShrink()
    << "rotation="<<ta.rotation()
    << "pen="<<ta.pen()
    << ")";
    return dbg;
}
#endif /* QT_NO_DEBUG_STREAM */
