#include "KDChartRelativePosition.h"
#include <QtXml/QDomDocumentFragment>
#include <KDChartMeasure.h>
#include <KDChartTextAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartBackgroundAttributes.h>

#define d d_func()

class KDChartRelativePosition::Private
{
    friend class ::KDChartRelativePosition;
public:
    Private();
private:
    Qt::Alignment alignment;
    KDChartMeasure horizontalPadding;
    KDChartMeasure verticalPadding;
    int rotation;
};

KDChartRelativePosition::Private::Private() :
    alignment( Qt::AlignCenter ),
    horizontalPadding(),
    verticalPadding(),
    rotation( 0 )
{
}


KDChartRelativePosition::KDChartRelativePosition()
    : _d( new Private() )
{
}

KDChartRelativePosition::KDChartRelativePosition( const KDChartRelativePosition& r )
    : _d( new Private( *r.d ) )
{
}

KDChartRelativePosition & KDChartRelativePosition::operator=( const KDChartRelativePosition& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

KDChartRelativePosition::~KDChartRelativePosition()
{
    delete _d; _d = 0;
}


bool KDChartRelativePosition::operator==( const KDChartRelativePosition& r )
{
    if( alignment() == r.alignment() &&
        horizontalPadding() == r.horizontalPadding() &&
        verticalPadding() == r.verticalPadding() &&
        rotation() == r.rotation() )
        return true;
    else
        return false;
}




QDomDocumentFragment KDChartRelativePosition::toXML() const
{
    // PENDING(kalle) Implement this
    return QDomDocumentFragment();
}

void KDChartRelativePosition::setAlignment( Qt::Alignment flags )
{
    d->alignment = flags;
}

Qt::Alignment KDChartRelativePosition::alignment() const
{
    return d->alignment;
}

void KDChartRelativePosition::setHorizontalPadding( const KDChartMeasure& padding )
{
    d->horizontalPadding = padding;
}

KDChartMeasure KDChartRelativePosition::horizontalPadding() const
{
    return d->horizontalPadding;
}

void KDChartRelativePosition::setVerticalPadding( const KDChartMeasure& padding )
{
    d->verticalPadding = padding;
}

KDChartMeasure KDChartRelativePosition::verticalPadding() const
{
    return d->verticalPadding;
}

void KDChartRelativePosition::setRotation( int rotation )
{
    d->rotation = rotation;
}

int KDChartRelativePosition::rotation() const
{
    return d->rotation;
}

