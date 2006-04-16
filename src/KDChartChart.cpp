#include <QList>
#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QHash>

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
    , planesLayout( 0 )
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



QHash<AbstractCoordinatePlane*, Chart::Private::PlaneInfo> Chart::Private::buildPlaneLayoutInfos()
{
    QHash<CartesianAxis*, AxisInfo> axisInfos;
    QHash<AbstractCoordinatePlane*, PlaneInfo> planeInfos;
    foreach (AbstractCoordinatePlane* plane, coordinatePlanes )
    {
        PlaneInfo p;
        // first check if we share space with another plane
       p.referencePlane = plane->referenceCoordinatePlane();
       planeInfos.insert( plane, p );

        foreach ( AbstractDiagram* abstractDiagram, plane->diagrams() ) {
            AbstractCartesianDiagram* diagram =
                    dynamic_cast<AbstractCartesianDiagram*> ( abstractDiagram );
            if( !diagram ) continue;

            foreach ( CartesianAxis* axis, diagram->axes() ) {
                if ( !axisInfos.contains( axis ) ) {
                    AxisInfo i;
                    i.plane = plane;
                    axisInfos.insert( axis, i );
                } else {
                    AxisInfo i = axisInfos[axis];
                    if ( i.plane == plane ) continue; // we don't want duplicates, only shared
 
                    /* The user expects diagrams to be added on top, and to the right
                     * so that horizontally we need to move the new diagram, vertically
                     * the reference one. */
                    PlaneInfo pi = planeInfos[plane];
                    if ( !pi.referencePlane ) { // plane-to-plane overrides linking via axes
                        pi.referencePlane = i.plane;
                        if ( axis->position() == CartesianAxis::Left
                            ||  axis->position() == CartesianAxis::Right )
                            pi.horizontalOffset += 1;
                        planeInfos[plane] = pi;

                        pi = planeInfos[i.plane];
                        if ( axis->position() == CartesianAxis::Top
                            || axis->position() == CartesianAxis::Bottom  )
                            pi.verticalOffset += 1;

                        planeInfos[i.plane] = pi;
                    }
                }
            }
        }
        p = planeInfos[plane];
        if ( p.referencePlane == 0 ) {
            p.grid = new QGridLayout();
            planeInfos[plane] = p;
        }
    }
    return planeInfos;
}

void Chart::Private::slotLayoutPlanes()
{
    if ( dataAndLegendLayout ) {
        dataAndLegendLayout->removeItem( planesLayout );
    }
    delete planesLayout;
    planesLayout = new QVBoxLayout(); // FIXME is this the right default, I wonder?

    QHash<AbstractCoordinatePlane*, PlaneInfo> planeInfos = buildPlaneLayoutInfos();
    QHash<AbstractAxis*, AxisInfo> axisInfos;
    foreach ( AbstractCoordinatePlane* plane, coordinatePlanes ) {
        Q_ASSERT( planeInfos.contains(plane) );
        const PlaneInfo pi = planeInfos[plane];
        int column = pi.horizontalOffset;
        int row = pi.verticalOffset;
        QGridLayout *planeLayout = pi.grid;
        if ( !planeLayout ) {
            // this plane is sharing an axis with another one
            planeLayout = planeInfos[pi.referencePlane].grid;
        } else {
            planesLayout->addLayout( planeLayout );
        }
        Q_ASSERT( planeLayout );
        planeLayout->addWidget( plane, row, column, Qt::AlignCenter );

        foreach ( AbstractDiagram* abstractDiagram, plane->diagrams() )
        {
            AbstractCartesianDiagram* diagram =
                    dynamic_cast<AbstractCartesianDiagram*> ( abstractDiagram );
            if( !diagram ) continue;  // FIXME polar ?
            // collect all axes of a kind into sublayouts
            QVBoxLayout *topAxes = new QVBoxLayout();
            QVBoxLayout *bottomAxes = new QVBoxLayout();
            QHBoxLayout *leftAxes = new QHBoxLayout();
            QHBoxLayout *rightAxes = new QHBoxLayout();

            foreach ( CartesianAxis* axis, diagram->axes() ) {
                if ( axisInfos.contains( axis ) ) continue; // already layed this one out
                Q_ASSERT ( axis );
                switch ( axis->position() )
                {
                    case CartesianAxis::Top:
                        topAxes->addWidget( axis );
                        break;
                    case CartesianAxis::Bottom:
                        bottomAxes->addWidget( axis );
                        break;
                    case CartesianAxis::Left:
                        leftAxes->addWidget( axis );
                        break;
                    case CartesianAxis::Right:
                        rightAxes->addWidget( axis );
                        break;
                    default:
                        Q_ASSERT_X( false, "Chart::paintEvent",
                                    "unknown axis position" );
                        break;
                };
                axisInfos.insert( axis, AxisInfo() );
            }
            planeLayout->addLayout( topAxes, 0, 1 );
            planeLayout->addLayout( bottomAxes, row + 1, 1 );
            planeLayout->addLayout( leftAxes, row, 0 );
            planeLayout->addLayout( rightAxes, row, column + 1);
        }

    }
    if ( dataAndLegendLayout )
        dataAndLegendLayout->addLayout( planesLayout, 1, 1 );
    slotRelayout();
}

void Chart::Private::createLayouts( QWidget* w )
{
    // layout for the planes is handled separately, so we don't want to delete it here
    if ( dataAndLegendLayout) {
        dataAndLegendLayout->removeItem( planesLayout );
        planesLayout->setParent( 0 );
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
    dataAndLegendLayout->addLayout( planesLayout, 1, 1 );
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
    addCoordinatePlane( new CartesianCoordinatePlane ( this ) );
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
    connect( plane, SIGNAL( diagramsChanged() ),
             p, SLOT( slotLayoutPlanes() ) );
    p->coordinatePlanes.append( plane );
    plane->setParent( this );
    p->slotLayoutPlanes();
}

void Chart::replaceCoordinatePlane( AbstractCoordinatePlane* plane, int position )
{
    if ( position >=0 && position < p->coordinatePlanes.size() ) {
        connect( plane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                 p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        connect( plane, SIGNAL( diagramsChanged() ),
                 p, SLOT( slotLayoutPlanes() ) );
        AbstractCoordinatePlane* oldPlane = p->coordinatePlanes.at( position );
        p->coordinatePlanes.replace ( position, plane );
        disconnect( oldPlane, SIGNAL( destroyedCoordinatePlane( AbstractCoordinatePlane* ) ),
                    p, SLOT( slotUnregisterDestroyedPlane( AbstractCoordinatePlane* ) ) );
        delete oldPlane;
    }
    p->slotLayoutPlanes();
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
    p->slotLayoutPlanes();
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


