#include <QList>
#include <QtDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "KDChartChart.h"
#include "KDChartCartesianCoordinatePlane.h"
#include <KDChartHeaderFooter.h>
#include <KDChartLegend.h>
#include <qlabel.h>

using namespace KDChart;

class Chart::Private
{
public:
    CoordinatePlaneList coordinatePlanes;
    HeaderFooterList headerFooters;
    LegendList legends;

    QHBoxLayout* layout;

    int globalLeadingLeft, globalLeadingRight, globalLeadingTop, globalLeadingBottom;

    Private ()
        : layout( 0 ),
          globalLeadingLeft( 0 ),
          globalLeadingRight( 0 ),
          globalLeadingTop( 0 ),
          globalLeadingBottom( 0 )
    {}

};

/*! Chart is the central widget. */
Chart::Chart ( QWidget* parent )
    : QWidget ( parent )
    , p ( new Private() )
{
    p->coordinatePlanes.append ( new CartesianCoordinatePlane ( this ) );
}

/*! Destructor. */
Chart::~Chart()
{
}

/*! Return the first (default) coordinate plane.
 */
CoordinatePlane* Chart::coordinatePlane()
{
    if ( p->coordinatePlanes.isEmpty() )
    {
        qWarning() << "Chart::coordinatePlane: warning: no coordinate plane defined.";
        return 0;
    } else {
        return p->coordinatePlanes.first();
    }
}

/*! Return all coordinate planes.
 */
CoordinatePlaneList Chart::coordinatePlanes()
{
    return p->coordinatePlanes;
}

void Chart::replaceCoordinatePlane( CoordinatePlane* plane, int position )
{
    if ( position >=0 && position < p->coordinatePlanes.size() ) {
        CoordinatePlane* oldPlane = p->coordinatePlanes.at( position );
        p->coordinatePlanes.replace ( position, plane );
        delete oldPlane;
    }
}
void Chart::setGlobalLeading( int left, int top, int right, int bottom )
{
    setGlobalLeadingLeft( left );
    setGlobalLeadingTop( top );
    setGlobalLeadingRight( right );
    setGlobalLeadingBottom( bottom );
}

void Chart::setGlobalLeadingLeft( int leading )
{
    p->globalLeadingLeft = leading;
}

int Chart::globalLeadingLeft() const
{
    return p->globalLeadingLeft;
}

void Chart::setGlobalLeadingTop( int leading )
{
    p->globalLeadingTop = leading;
}

int Chart::globalLeadingTop() const
{
    return p->globalLeadingTop;
}

void Chart::setGlobalLeadingRight( int leading )
{
    p->globalLeadingRight = leading;
}

int Chart::globalLeadingRight() const
{
    return p->globalLeadingRight;
}

void Chart::setGlobalLeadingBottom( int leading )
{
    p->globalLeadingBottom = leading;
}

int Chart::globalLeadingBottom() const
{
    return p->globalLeadingBottom;
}

void Chart::paintEvent( QPaintEvent* )
{
	// DF: this is ugly as hell, but it fixes the crash in Widget.exe
	// Do *not* do layouting code in a paintEvent, since it triggers a paint event
	// and you get: 
	// QWidget::repaint: recursive repaint detected.
	static bool sInsidePaint = false;
	if (sInsidePaint) return;
	sInsidePaint = true;


    if( p->layout )
        delete p->layout;

    // The HBox p->layout provides the left and right global leadings
    p->layout = new QHBoxLayout( this );
    p->layout->addSpacing( p->globalLeadingLeft );

    // The vLayout provides top and bottom global leadings and lays
    // out headers/footers and the data area.
    QVBoxLayout* vLayout = new QVBoxLayout();
    p->layout->addLayout( vLayout, 2 );
    p->layout->addSpacing( p->globalLeadingRight );
    vLayout->addSpacing( p->globalLeadingTop );
    QGridLayout* headerLayout = new QGridLayout();
    vLayout->addLayout( headerLayout );
    QGridLayout* dataAndLegendLayout = new QGridLayout();
    vLayout->addLayout( dataAndLegendLayout, 2 );
    QGridLayout* footerLayout = new QGridLayout();
    vLayout->addLayout( footerLayout );
    vLayout->addSpacing( p->globalLeadingBottom );

    // Headers and footers
    const QObjectList& childList = children();
    for( QObjectList::const_iterator it = childList.begin(); it != childList.end(); ++it ) {
        if( !*it )
            continue;
        HeaderFooter* hf = qobject_cast<HeaderFooter*>( *it );
        if( !hf )
            continue;
        qDebug() << "Found a header";
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
            qDebug( "Adding Footer2Right" );
            break;
        default:
            qDebug( "Unknown header/footer position" );
        }
    }
    if( headerFooter() )
        qDebug() << "First headerfooter = " << headerFooter() << ", className = " << headerFooter()->metaObject()->className() << ", geometry = " << headerFooter()->geometry() << ", sizeHint = " << headerFooter()->sizeHint();

    // the data+axes area
    dataAndLegendLayout->addWidget( coordinatePlane(), 1, 1, Qt::AlignCenter );
    dataAndLegendLayout->setRowStretch( 1, 2 );
    dataAndLegendLayout->setColumnStretch( 1, 2 );

    // now the legends, depending on their position
    LegendList legendList = legends();
    for ( LegendList::iterator it = legendList.begin(), end = legendList.end() ; it != end ; ++it ) {
        qDebug( "Found legend" );
        switch( (*it)->position() ) {
        case KDChart::Legend::North:
            dataAndLegendLayout->addWidget( *it, 0, 1, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::South:
            dataAndLegendLayout->addWidget( *it, 2, 1, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::West:
            dataAndLegendLayout->addWidget( *it, 1, 0, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::East:
            dataAndLegendLayout->addWidget( *it, 1, 2, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::NorthWest:
            dataAndLegendLayout->addWidget( *it, 0, 0, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::NorthNorthWest:
            dataAndLegendLayout->addWidget( *it, 0, 1, 1, 1, Qt::AlignVCenter|Qt::AlignLeft );
            break;
        case KDChart::Legend::WestNorthWest:
            dataAndLegendLayout->addWidget( *it, 1, 0, 1, 1, Qt::AlignHCenter|Qt::AlignTop );
            break;
        case KDChart::Legend::NorthEast:
            dataAndLegendLayout->addWidget( *it, 0, 2, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::NorthNorthEast:
            dataAndLegendLayout->addWidget( *it, 0, 1, 1, 1, Qt::AlignVCenter|Qt::AlignRight );
            break;
        case KDChart::Legend::EastNorthEast:
            dataAndLegendLayout->addWidget( *it, 1, 2, 1, 1, Qt::AlignHCenter|Qt::AlignTop );
            break;
        case KDChart::Legend::SouthWest:
            dataAndLegendLayout->addWidget( *it, 2, 0, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::SouthSouthWest:
            dataAndLegendLayout->addWidget( *it, 2, 1, 1, 1, Qt::AlignVCenter|Qt::AlignLeft );
            break;
        case KDChart::Legend::WestSouthWest:
            dataAndLegendLayout->addWidget( *it, 1, 0, 1, 1, Qt::AlignHCenter|Qt::AlignBottom );
            break;
        case KDChart::Legend::SouthEast:
            dataAndLegendLayout->addWidget( *it, 2, 2, 1, 1, Qt::AlignCenter );
            break;
        case KDChart::Legend::SouthSouthEast:
            dataAndLegendLayout->addWidget( *it, 2, 1, 1, 1, Qt::AlignVCenter|Qt::AlignRight );
            break;
        case KDChart::Legend::EastSouthEast:
            dataAndLegendLayout->addWidget( *it, 1, 2, 1, 1, Qt::AlignHCenter|Qt::AlignBottom );
            break;
        default:
            Q_ASSERT( !"Undefined legend position" );
        }
    }

    qDebug() << "Before activate";
    p->layout->activate();
    qDebug() << "After activate";

    qDebug() << "Total chart area is " << geometry();
    qDebug() << "Coordinate plane area is " << coordinatePlane()->geometry();
    if( legend() )
        qDebug() << "First legend = " << legend() << ", className = " << legend()->metaObject()->className() << ", geometry = " << legend()->geometry() << "sizeHint = " << legend()->sizeHint() << ", is visible = " << legend()->isVisible();

    sInsidePaint = false;
}


void Chart::addHeaderFooter( HeaderFooter* headerFooter )
{
    Q_ASSERT_X ( headerFooter->parent() == this, "KDChart::addHeaderFooter",
                 "Headers/footers have to be children of the chart." );

    p->headerFooters.append( headerFooter );
    headerFooter->setParent( this );
}

void Chart::replaceHeaderFooter( HeaderFooter* headerFooter, int position )
{
    if( p->headerFooters.size() <= position ) {
        addHeaderFooter( headerFooter );
    } else {
        p->headerFooters.replace( position, headerFooter );
    }
}

void Chart::removeHeaderFooter( int position )
{
    if( position >= 0 && p->headerFooters.size() > position ) {
        p->headerFooters.removeAt( position );
    }
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
    Q_ASSERT_X ( legend->parent() == this, "KDChart::addLegend",
                 "Legends have to be children of the chart." );

    p->legends.append( legend );
    legend->setParent( this );
}

void Chart::replaceLegend( Legend* legend, int position )
{
    if( p->legends.size() <= position ) {
        addLegend( legend );
    } else {
        p->legends.replace( position, legend );
    }
}

void Chart::removeLegend( int position )
{
    if( position >= 0 && p->legends.size() > position ) {
        p->legends.removeAt( position );
    }
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
