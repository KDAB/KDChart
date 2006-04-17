
#include <QVariant>
#include <QtXml/QDomDocumentFragment>
#include "KDChartDataValueAttributes.h"
#include "KDChartRelativePosition.h"
#include <KDChartTextAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartBackgroundAttributes.h>
#include <KDChartMarkerAttributes.h>

// FIXME till
#define KDCHART_DATA_VALUE_AUTO_DIGITS 4
#define d d_func()

using namespace KDChart;

class DataValueAttributes::Private
{
    friend class ::DataValueAttributes;
public:
    Private();
private:
    bool visible;
    TextAttributes textAttributes;
    FrameAttributes frameAttributes;
    BackgroundAttributes backgroundAttributes;
    MarkerAttributes markerAttributes;
    int decimalDigits;
    int powerOfTenDivisor;
    bool showInfinite;
    KDChartEnums::PositionFlag negativeAnchorPos;
    KDChartRelativePosition    negativeRelPos;
    KDChartEnums::PositionFlag positiveAnchorPos;
    KDChartRelativePosition    positiveRelPos;
};

DataValueAttributes::Private::Private() :
    visible( false ),
    decimalDigits( KDCHART_DATA_VALUE_AUTO_DIGITS ),
    powerOfTenDivisor( 0 ),
    showInfinite( true ),
    negativeAnchorPos( KDChartEnums::PosCenter   ),
    negativeRelPos(),  // use the default: centered alignment, no padding, no rotation
    positiveAnchorPos( KDChartEnums::PosCenter   ),
    positiveRelPos()
{
  textAttributes.setRotation( -45 );
}


DataValueAttributes::DataValueAttributes()
    : _d( new Private() )
{
}

DataValueAttributes::DataValueAttributes( const DataValueAttributes& r )
    : _d( new Private( *r.d ) )
{
}

DataValueAttributes & DataValueAttributes::operator=( const DataValueAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

DataValueAttributes::~DataValueAttributes()
{
    delete _d; _d = 0;
}


bool DataValueAttributes::operator==( const DataValueAttributes& r ) const
{
    return ( isVisible() == r.isVisible() &&
	     textAttributes() == r.textAttributes() &&
	     frameAttributes() == r.frameAttributes() &&
	     backgroundAttributes() == r.backgroundAttributes() &&
	     markerAttributes() == r.markerAttributes() &&
	     decimalDigits() == r.decimalDigits() &&
	     powerOfTenDivisor() == r.powerOfTenDivisor() &&
	     showInfinite() == r.showInfinite() &&
	     negativeAnchorPosition() == r.negativeAnchorPosition() &&
	     positiveAnchorPosition() == r.positiveAnchorPosition() &&
	     negativeRelativePosition() == r.negativeRelativePosition() &&
	     positiveRelativePosition() == r.positiveRelativePosition() );
}

/*static*/
const DataValueAttributes& DataValueAttributes::defaultAttributes()
{
    static const DataValueAttributes theDefaultDataValueAttributes;
    return theDefaultDataValueAttributes;
}

/*static*/
const QVariant& DataValueAttributes::defaultAttributesAsVariant()
{
    static const QVariant theDefaultDataValueAttributesVariant = QVariant::fromValue(defaultAttributes());
    return theDefaultDataValueAttributesVariant;
}


void DataValueAttributes::setVisible( bool visible )
{
    d->visible = visible;
}

bool DataValueAttributes::isVisible() const
{
    return d->visible;
}

QDomDocumentFragment DataValueAttributes::toXML() const
{
    return QDomDocumentFragment();
}

void DataValueAttributes::setTextAttributes( const TextAttributes &a )
{
    d->textAttributes = a;
}

TextAttributes DataValueAttributes::textAttributes() const
{
    return d->textAttributes;
}

void DataValueAttributes::setFrameAttributes( const FrameAttributes &a )
{
    d->frameAttributes = a;
}

FrameAttributes DataValueAttributes::frameAttributes() const
{
    return d->frameAttributes;
}

void DataValueAttributes::setBackgroundAttributes( const BackgroundAttributes &a )
{
    d->backgroundAttributes = a;
}

BackgroundAttributes DataValueAttributes::backgroundAttributes() const
{
    return d->backgroundAttributes;
}

void DataValueAttributes::setMarkerAttributes( const MarkerAttributes &a )
{
    d->markerAttributes = a;
}

MarkerAttributes DataValueAttributes::markerAttributes() const
{
    return d->markerAttributes;
}


void DataValueAttributes::setDecimalDigits( int digits )
{
    d->decimalDigits = digits;
}

int DataValueAttributes::decimalDigits() const
{
    return d->decimalDigits;
}

void DataValueAttributes::setPowerOfTenDivisor( int powerOfTenDivisor )
{
    d->powerOfTenDivisor = powerOfTenDivisor;
}

int DataValueAttributes::powerOfTenDivisor() const
{
    return d->powerOfTenDivisor;
}

void DataValueAttributes::setShowInfinite( bool infinite )
{
    d->showInfinite = infinite;
}

bool DataValueAttributes::showInfinite() const
{
    return d->showInfinite;
}

void DataValueAttributes::setNegativeAnchorPosition( KDChartEnums::PositionFlag anchorPosition )
{
    d->negativeAnchorPos = anchorPosition;
}

KDChartEnums::PositionFlag DataValueAttributes::negativeAnchorPosition() const
{
    return d->negativeAnchorPos;
}

void DataValueAttributes::setNegativeRelativePosition( const KDChartRelativePosition& relPosition )
{
    d->negativeRelPos = relPosition;
}

KDChartRelativePosition DataValueAttributes::negativeRelativePosition() const
{
    return d->negativeRelPos;
}

void DataValueAttributes::setPositiveAnchorPosition( KDChartEnums::PositionFlag anchorPosition )
{
    d->positiveAnchorPos = anchorPosition;
}

KDChartEnums::PositionFlag DataValueAttributes::positiveAnchorPosition() const
{
    return d->positiveAnchorPos;
}

void DataValueAttributes::setPositiveRelativePosition( const KDChartRelativePosition& relPosition )
{
    d->positiveRelPos = relPosition;
}

KDChartRelativePosition DataValueAttributes::positiveRelativePosition() const
{
    return d->positiveRelPos;
}

