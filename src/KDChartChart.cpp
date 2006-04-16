#include <QList>
#include <QtDebug>
#include <QGridLayout>
#include <QLabel>

#include "KDChartChart.h"
#include "KDChartChart_p.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartHeaderFooter.h"
#include "KDChartLegend.h"

using namespace KDChart;

void Chart::Private::slotUnregisterDestroyedLegend( Legend *l )
{
    legends.removeAll( l );
}

void Chart::Private::slotUnregisterDestroyedHeaderFooter( HeaderFooter* hf )
{
    headerFooters.removeAll( hf );
}

void Chart::Private::slotUnregisterDestroyedPlane( AbstractCoordinatePlane* plane )
{
    coordinatePlanes.removeAll( plane );
}

Chart::Private::Private( Chart* chart_ )
    : chart( chart_ )
    , layout( 0 )
    , vLayout( 0 )
    , planeLayout( 0 )
    , headerLayout( 0 )
    , footerLayout( 0 )
    , dataAndLegendLayout( 0 )
    , globalLeadingLeft( 0 ) 
    , globalLeadingRight( 0 )
    , globalLeadingTop( 0 )
    , globalLeadingBottom( 0 )
{
}

void Chart::Private::layoutHeadersAndFooters()
{
    foreach ( HeaderFooter *hf, headerFooters ) {

        switch( hf->position() ) {
            case HeaderFooter::NorthWest:
                headerLayout->addWidget( hf /*new QLabel( "blah", this )*/, 0, 0, 1, 1, Qt::AlignLeft|Qt::AlignTop );
                break;
            case HeaderFooter::North:
                headerLayout->addWidget( hf, 0, 1, 1, 1, Qt::AlignHCenter|Qt::AlignTop );
                break;
            case HeaderFooter::NorthEast:
                headerLayout->addWidget( hf, 0, 2, 1, 1, Qt::AlignRight|Qt::AlignTop );
                break;
            case HeaderFooter::NorthWest1:
                headerLayout->addWidget( hf, 1, 0, 1, 1, Qt::AlignLeft|Qt::AlignVCenter );
                break;
            case HeaderFooter::North1:
                headerLayout->addWidget( hf, 1, 1, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter );
                break;
            case HeaderFooter::NorthEast1:
                headerLayout->addWidget( hf, 1, 2, 1, 1, Qt::AlignRight|Qt::AlignVCenter );
                break;
            case HeaderFooter::NorthWest2:
                headerLayout->addWidget( hf, 2, 0, 1, 1, Qt::AlignLeft|Qt::AlignBottom );
                break;
            case HeaderFooter::North2:
                headerLayout->addWidget( hf, 2, 1, 1, 1, Qt::AlignHCenter|Qt::AlignBottom );
                break;
            case HeaderFooter::NorthEast2:
                headerLayout->addWidget( hf, 2, 2, 1, 1, Qt::AlignRight|Qt::AlignBottom );
                break;
            case HeaderFooter::SouthWest:
                footerLayout->addWidget( hf, 0, 0, 1, 1, Qt::AlignLeft|Qt::AlignTop );
                break;
            case HeaderFooter::South:
                footerLayout->addWidget( hf, 0, 1, 1, 1, Qt::AlignHCenter|Qt::AlignTop );
                break;
            case HeaderFooter::SouthEast:
                footerLayout->addWidget( hf, 0, 2, 1, 1, Qt::AlignRight|Qt::AlignTop );
                break;
            case HeaderFooter::SouthWest1:
                footerLayout->addWidget( hf, 1, 0, 1, 1, Qt::AlignLeft|Qt::AlignVCenter );
                break;
            case HeaderFooter::South1:
                footerLayout->addWidget( hf, 1, 1, 1, 1, Qt::AlignHCenter|Qt::AlignVCenter );
                break;
            case HeaderFooter::SouthEast1:
                footerLayout->addWidget( hf, 1, 2, 1, 1, Qt::AlignRight|Qt::AlignVCenter );
                break;
            case HeaderFooter::SouthWest2:
                footerLayout->addWidget( hf, 2, 0, 1, 1, Qt::AlignLeft|Qt::AlignBottom );
                break;
            case HeaderFooter::South2:
                footerLayout->addWidget( hf, 2, 1, 1, 1, Qt::AlignHCenter|Qt::AlignBottom );
                break;
            case HeaderFooter::SouthEast2:
                footerLayout->addWidget( hf, 2, 2, 1, 1, Qt::AlignRight|Qt::AlignBottom );
                break;
            default:
                qDebug( "Unknown header/footer position" );
        }
    }
}

void Chart::Private::layoutLegends()
{
    foreach ( Legend *legend, legends ) {
        switch( legend->position() ) {
            case KDChart::Legend::North:
                dataAndLegendLayout->addWidget( legend, 0, 1, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::South:
                dataAndLegendLayout->addWidget( legend, 2, 1, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::West:
                dataAndLegendLayout->addWidget( legend, 1, 0, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::East:
                dataAndLegendLayout->addWidget( legend, 1, 2, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::NorthWest:
                dataAndLegendLayout->addWidget( legend, 0, 0, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::NorthNorthWest:
                dataAndLegendLayout->addWidget( legend, 0, 1, 1, 1, Qt::AlignVCenter|Qt::AlignLeft );
                break;
            case KDChart::Legend::WestNorthWest:
                dataAndLegendLayout->addWidget( legend, 1, 0, 1, 1, Qt::AlignHCenter|Qt::AlignTop );
                break;
            case KDChart::Legend::NorthEast:
                dataAndLegendLayout->addWidget( legend, 0, 2, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::NorthNorthEast:
                dataAndLegendLayout->addWidget( legend, 0, 1, 1, 1, Qt::AlignVCenter|Qt::AlignRight );
                break;
            case KDChart::Legend::EastNorthEast:
                dataAndLegendLayout->addWidget( legend, 1, 2, 1, 1, Qt::AlignHCenter|Qt::AlignTop );
                break;
            case KDChart::Legend::SouthWest:
                dataAndLegendLayout->addWidget( legend, 2, 0, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::SouthSouthWest:
                dataAndLegendLayout->addWidget( legend, 2, 1, 1, 1, Qt::AlignVCenter|Qt::AlignLeft );
                break;
            case KDChart::Legend::WestSouthWest:
                dataAndLegendLayout->addWidget( legend, 1, 0, 1, 1, Qt::AlignHCenter|Qt::AlignBottom );
                break;
            case KDChart::Legend::SouthEast:
                dataAndLegendLayout->addWidget( legend, 2, 2, 1, 1, Qt::AlignCenter );
                break;
            case KDChart::Legend::SouthSouthEast:
                dataAndLegendLayout->addWidget( legend, 2, 1, 1, 1, Qt::AlignVCenter|Qt::AlignRight );
                break;
            case KDChart::Legend::EastSouthEast:
                dataAndLegendLayout->addWidget( legend, 1, 2, 1, 1, Qt::AlignHCenter|Qt::AlignBottom );
                break;
            default:
                Q_ASSERT( !"Undefined legend position" );
        }
    }
}

void Chart::Private::layoutPlanes()
{
    if ( dataAndLegendLayout ) {
        dataAndLegendLayout->removeItem( planeLayout );
    }
    delete planeLayout;
    planeLayout = new QGridLayout();
    foreach (AbstractCoordinatePlane* plane, coordinatePlanes )
    {
        planeLayout->addWidget( plane, 1, 1, Qt::AlignCenter );

        foreach ( AbstractDiagram* abstractDiagram, plane->diagrams() )
        {
            AbstractCartesianDiagram* diagram =
                    dynamic_cast<AbstractCartesianDiagram*> ( abstractDiagram );
            if( diagram ) {
                // If diagram == 0, we are probably polar and do not need any axes anyway.
                foreach ( CartesianAxis* axis, diagram->axes() ) {
                    Q_ASSERT ( axis );
                    switch ( axis->position() )
                    {
                        case CartesianAxis::Top:
                            planeLayout->addWidget( axis, 0, 1 );
                            break;
                        case CartesianAxis::Bottom:
                            planeLayout->addWidget( axis, 2, 1 );
                            break;
                        case CartesianAxis::Left:
                            planeLayout->addWidget( axis, 1, 0 );
                            break;
                        case CartesianAxis::Right:
                            planeLayout->addWidget( axis, 1, 2 );
                            break;
                        default:
                            Q_ASSERT_X( false, "Chart::paintEvent",
                                        "unknown axis position" );
                            break;
                    };
                }
            }
        }
    }
    if ( dataAndLegendLayout )
        dataAndLegendLayout->addLayout( planeLayout, 1, 1 );
}

void Chart::Private::createLayouts( QWidget* w )
{
    // layout for the planes is handled separately, so we don't want to delete it here
    if ( dataAndLegendLayout) {
        dataAndLegendLayout->removeItem( planeLayout );
        planeLayout->setParent( 0 );
    }
    // nuke the old bunch
    delete layout;
    
    // The HBox p->layout provides the left and right global leadings
    layout = new QHBoxLayout( w );
    layout->addSpacing( globalLeadingLeft );

    // The vLayout provides top and bottom global leadings and lays
    // out headers/footers and the data area.
    vLayout = new QVBoxLayout();
    layout->addLayout( vLayout, 2 );
    layout->addSpacing( globalLeadingRight );
    vLayout->addSpacing( globalLeadingTop );
    headerLayout = new QGridLayout();
    vLayout->addLayout( headerLayout );
    dataAndLegendLayout = new QGridLayout();
    vLayout->addLayout( dataAndLegendLayout, 2 );
    footerLayout = new QGridLayout();
    vLayout->addLayout( footerLayout );
    vLayout->addSpacing( globalLeadingBottom );

    // the data+axes area
    dataAndLegendLayout->addLayout( planeLayout, 1, 1 );
    dataAndLegendLayout->setRowStretch( 1, 2 );
    dataAndLegendLayout->setColumnStretch( 1, 2 );

}

void Chart::Private::slotRelayout()
{
    createLayouts( chart );
    layoutHeadersAndFooters();
    layoutLegends();
    layout->activate();
}

// ******** Chart interface implementation *********** 

Chart::Chart ( QWidget* parent )
    : QWidget ( parent )
    , p ( new Private( this ) )
{
    p->coordinatePlanes.append ( new CartesianCoordinatePlane ( this ) );
    p->layoutPlanes();
}

Chart::~Chart()
{
}

AbstractCoordinatePlane* Chart::coordinatePlane()
{
    if ( p->coordinatePlanes.isEmpty() )
    {
        qWarning() << "Chart::coordinatePlane: warning: no coordinate plane defined.";
        return 0;
    } else {
        return p->coordinatePlanes.first();
    }
}

CoordinatePlaneList Chart::coordinatePlanes()
{
    return p->coordinatePlanes;
}

void Chart::addCoordinatePlane( AbstractCoordinatePlane* plane )
{
    connect( plane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
             p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
    p->coordinatePlanes.append( plane );
    plane->setParent( this );
    p->layoutPlanes();
    p->slotRelayout();
}

void Chart::replaceCoordinatePlane( AbstractCoordinatePlane* plane, int position )
{
    if ( position >=0 && position < p->coordinatePlanes.size() ) {
        connect( plane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                 p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        AbstractCoordinatePlane* oldPlane = p->coordinatePlanes.at( position );
        p->coordinatePlanes.replace ( position, plane );
        disconnect( oldPlane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                    p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        delete oldPlane;
    }
    p->layoutPlanes();
    p->slotRelayout();
}

void Chart::removeCoordinatePlane( int position )
{
    if ( position >=0 && position < p->coordinatePlanes.size() ) {
        AbstractCoordinatePlane* oldPlane = p->coordinatePlanes.at( position );
        p->coordinatePlanes.removeAt( position );
        disconnect( oldPlane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                    p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        delete oldPlane;
    }
    p->layoutPlanes();
    p->slotRelayout();
}

void Chart::setGlobalLeading( int left, int top, int right, int bottom )
{
    setGlobalLeadingLeft( left );
    setGlobalLeadingTop( top );
    setGlobalLeadingRight( right );
    setGlobalLeadingBottom( bottom );
    p->slotRelayout();
}

void Chart::setGlobalLeadingLeft( int leading )
{
    p->globalLeadingLeft = leading;
    p->slotRelayout();
}

int Chart::globalLeadingLeft() const
{
    return p->globalLeadingLeft;
}

void Chart::setGlobalLeadingTop( int leading )
{
    p->globalLeadingTop = leading;
    p->slotRelayout();
}

int Chart::globalLeadingTop() const
{
    return p->globalLeadingTop;
}

void Chart::setGlobalLeadingRight( int leading )
{
    p->globalLeadingRight = leading;
    p->slotRelayout();
}

int Chart::globalLeadingRight() const
{
    return p->globalLeadingRight;
}

void Chart::setGlobalLeadingBottom( int leading )
{
    p->globalLeadingBottom = leading;
    p->slotRelayout();
}

int Chart::globalLeadingBottom() const
{
    return p->globalLeadingBottom;
}


void Chart::paintEvent( QPaintEvent* )
{
}


void Chart::addHeaderFooter( HeaderFooter* headerFooter )
{
    p->headerFooters.append( headerFooter );
    headerFooter->setParent( this );
    connect( headerFooter, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
             p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
    connect( headerFooter, SIGNAL( positionChanged( AbstractArea* ) ),
             p, SLOT( slotRelayout() ) );
    p->slotRelayout();
}

void Chart::replaceHeaderFooter( HeaderFooter* headerFooter, int position )
{
    if( p->headerFooters.size() <= position ) {
        addHeaderFooter( headerFooter );
    } else {
        HeaderFooter *old = p->headerFooters.value( position );
        p->headerFooters.replace( position, headerFooter );
        disconnect( old, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
                    p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
        delete old;
        connect( headerFooter, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
                 p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
    }
    p->slotRelayout();
}

void Chart::removeHeaderFooter( int position )
{
    if( position >= 0 && p->headerFooters.size() > position ) {
        HeaderFooter * old = p->headerFooters.at( position );
        disconnect( old, SIGNAL( destroyedHeaderFooter( HeaderFooter* ) ),
                    p, SLOT( slotUnregisterDestroyedHeaderFooter( HeaderFooter* ) ) );
        delete p->headerFooters.takeAt( position );
    }
    p->slotRelayout();
}

HeaderFooter* Chart::headerFooter()
{
    if( p->headerFooters.isEmpty() ) {
        return 0;
    } else {
        return p->headerFooters.first();
    }
}

HeaderFooterList Chart::headerFooters()
{
    return p->headerFooters;
}

void Chart::addLegend( Legend* legend )
{
    p->legends.append( legend );
    legend->setParent( this );
    connect( legend, SIGNAL( destroyedLegend( Legend* ) ),
             p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
    connect( legend, SIGNAL( positionChanged( AbstractArea* ) ),
             p, SLOT( slotRelayout() ) );
    p->slotRelayout();
}

void Chart::replaceLegend( Legend* legend, int position )
{
    if( p->legends.size() <= position ) {
        addLegend( legend );
    } else {
        Legend *old = p->legends.value( position );
        p->legends.replace( position, legend );
        disconnect( old, SIGNAL( destroyedLegend( Legend* ) ),
                    p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
        delete old;
        connect( legend, SIGNAL( destroyedLegend( Legend* ) ),
                 p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
    }
    p->slotRelayout();
}

void Chart::removeLegend( int position )
{
    if( position >= 0 && p->legends.size() > position ) {
        Legend *old = p->legends.value( position );
        disconnect( old, SIGNAL( destroyedLegend( Legend* ) ),
                    p, SLOT( slotUnregisterDestroyedLegend( Legend* ) ) );
        delete old;
    }
    p->slotRelayout();
}

Legend* Chart::legend()
{
    if ( p->legends.isEmpty() )
    {
        return 0;
    } else {
        return p->legends.first();
    }
}

LegendList Chart::legends()
{
    return p->legends;
}


