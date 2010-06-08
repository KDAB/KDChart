#include "KDChartLineAttributes.h"
#include <QDebug>

#include <KDABLibFakes>

#define d d_func()

using namespace KDChart;

class LineAttributes::Private
{
    friend class LineAttributes;
public:
    Private();

private:
    //Areas
    MissingValuesPolicy missingValuesPolicy;
    bool displayArea;
    uint transparency;
    int areaBoundingDataset;
};


LineAttributes::Private::Private()
    : missingValuesPolicy( MissingValuesAreBridged )
    , displayArea( false )
    , transparency( 255 )
    , areaBoundingDataset( -1 )
{
}


LineAttributes::LineAttributes()
    : _d( new Private() )
{
}

LineAttributes::LineAttributes( const LineAttributes& r )
    : _d( new Private( *r.d ) )
{
}

LineAttributes& LineAttributes::operator= ( const LineAttributes& r )
{
    if( this == &r )
        return *this;

    *d = *r.d;

    return *this;
}

LineAttributes::~LineAttributes()
{
    delete _d; _d = 0;
}

bool LineAttributes::operator==( const LineAttributes& r ) const
{
    return
        missingValuesPolicy() == r.missingValuesPolicy() &&
        displayArea() == r.displayArea() &&
        transparency() == r.transparency() &&
        areaBoundingDataset() == r.areaBoundingDataset();
}

void LineAttributes::setMissingValuesPolicy( MissingValuesPolicy policy )
{
    d->missingValuesPolicy = policy;
}

LineAttributes::MissingValuesPolicy LineAttributes::missingValuesPolicy() const
{
    return d->missingValuesPolicy;
}

void LineAttributes::setDisplayArea( bool display )
{
    d->displayArea = display;
}

bool LineAttributes::displayArea() const
{
   return d->displayArea;
}

void LineAttributes::setTransparency( uint alpha )
{
     if ( alpha > 255 )
        alpha = 255;
    d->transparency = alpha;
}

uint LineAttributes::transparency() const
{
     return d->transparency;
}

void LineAttributes::setAreaBoundingDataset( int dataset )
{
   d->areaBoundingDataset = dataset;
}

int LineAttributes::areaBoundingDataset() const
{
    return d->areaBoundingDataset;
}

#if !defined(QT_NO_DEBUG_STREAM)
QDebug operator<<(QDebug dbg, const KDChart::LineAttributes& a)
{
    dbg << "KDChart::LineAttributes("
            //     MissingValuesPolicy missingValuesPolicy;
            << "bool="<<a.displayArea()
            << "transparency="<<a.transparency()
            << "areaBoundingDataset="<<a.areaBoundingDataset()
            << ")";
    return dbg;

}
#endif /* QT_NO_DEBUG_STREAM */
