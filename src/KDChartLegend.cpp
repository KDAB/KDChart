#include "KDChartLegend.h"
#include "KDChartLegend_p.h"
#include <QtXml/QDomDocumentFragment>
#include <KDChartTextAttributes.h>
#include <KDChartMarkerAttributes.h>
#include <QFont>
#include <QPainter>
#include <QTextTableCell>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextDocumentFragment>
#include <QAbstractTextDocumentLayout>
#include <QtDebug>
#include <QLabel>
#include <KDChartAbstractDiagram.h>
#include "KDTextDocument.h"
#include <KDChartDiagramObserver.h>
#include <QGridLayout>
#include "KDChartLayoutItems.h"

using namespace KDChart;

static const int MarkerObjectType = 0x1000; // Qt 4.1.1 does not define a value from
                                            // which user-defined object types start, but
                                            // currently 0, 1, 2 are the only predefined
                                            // ones, so this seems pretty safe.

Legend::Private::Private() :
    position( East ),
    orientation( Qt::Vertical ),
    showLines( false ),
    texts(),
    textAttributes(),
    titleText( QObject::tr( "Legend" ) ),
    titleTextAttributes(),
    spacing( 1 ),
    observer( NULL ),
    blockBuildLegend( false )
{
}

Legend::Private::~Private()
{
}



#define d d_func()

Legend::Legend( QWidget* parent ) :
    KDChartArea( new Private(), parent )
{
    init();
}

Legend::Legend( KDChart::AbstractDiagram* diagram, QWidget* parent ) :
    KDChartArea( new Private(), parent )
{
    init();
    setDiagram( diagram );
}

Legend::~Legend()
{
}

void Legend::init()
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    d->layout = new QGridLayout();
    d->layout->setMargin( 2 );
    d->layout->setSpacing( d->spacing );
    d->layout->setColumnStretch( 0, 0 ); // markers don't stretch
    d->layout->setColumnStretch( 3, 1 ); // texts stretch
    setLayout( d->layout );

    d->blockBuildLegend = true;
    TextAttributes textAttrs;
    textAttrs.setColor( Qt::black );
    textAttrs.setFont( QFont( "helvetica", 10, QFont::Normal, false ) );
    textAttrs.setRelativeSize( 16 );
    textAttrs.setUseRelativeSize( true );
    setTextAttributes( textAttrs );
    TextAttributes titleTextAttrs;
    titleTextAttrs.setColor( Qt::black );
    titleTextAttrs.setFont( QFont( "helvetica", 12, QFont::Bold, false ) );
    titleTextAttrs.setRelativeSize( 20 );
    titleTextAttrs.setUseRelativeSize( true );
    setTitleTextAttributes( titleTextAttrs );
    d->blockBuildLegend = false;
}

QDomDocumentFragment Legend::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment Legend::toXML() const" );
    return QDomDocumentFragment();
}

Legend* Legend::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: Legend* Legend::clone() const" );
    return (Legend*)0xdeadbeef;
}

void Legend::paintEvent( QPaintEvent* evt )
{
    // Legend always has a frame around it, might be overpainted by Area afterwards.
    QPainter painter( this );
    painter.drawRect( evt->rect().adjusted( 1, 1, -1, -1 ) );

    // Paint the background and frame
    KDChartArea::paintEvent( evt );

    // PENDING(kalle) Support palette

    foreach( KDChart::LayoutItem* layoutItem, d->layoutItems ) {
        layoutItem->paint( &painter );
    }
}


uint Legend::datasetCount() const
{
    Q_ASSERT( d->modelLabels.count() == d->modelBrushes.count() );
    return d->modelLabels.count();
}


void Legend::setDiagram( KDChart::AbstractDiagram* diagram )
{
    d->diagram = diagram;
    if ( d->observer != NULL )
    {
        delete d->observer; d->observer = NULL;
    }
    if ( diagram != NULL )
    {
        d->observer = new DiagramObserver( diagram, this );
        connect( d->observer, SIGNAL( diagramDestroyed(AbstractDiagram*) ),
                        SLOT( resetDiagram() ));
        connect( d->observer, SIGNAL( diagramDataChanged(AbstractDiagram*) ),
			SLOT( buildLegend() ));
        connect( d->observer, SIGNAL( diagramAttributesChanged(AbstractDiagram*) ),
			SLOT( buildLegend() ));
    }

    buildLegend();
}

KDChart::AbstractDiagram* Legend::diagram() const
{
    return d->diagram;
}

void Legend::setPosition( LegendPosition position )
{
    d->position = position;

    buildLegend();
}

Legend::LegendPosition Legend::position() const
{
    return d->position;
}

void Legend::setOrientation( Qt::Orientation orientation )
{
    d->orientation = orientation;

    buildLegend();
}

Qt::Orientation Legend::orientation() const
{
    return d->orientation;
}

void Legend::setShowLines( bool legendShowLines )
{
    d->showLines = legendShowLines;

    buildLegend();
}

bool Legend::showLines() const
{
    return d->showLines;
}

/**
    \brief Removes all legend texts that might have been set by setText.

    This resets the Legend to default behaviour: Texts are created automatically.
*/
void Legend::resetTexts()
{
    d->texts.clear();
}

void Legend::setText( uint dataset, const QString& text )
{
    d->texts[dataset] = text;

    buildLegend();
}

QString Legend::text( uint dataset ) const
{
    if( d->texts.find( dataset ) != d->texts.end() )
        return d->texts[dataset];
    else
        return d->modelLabels[ dataset ];
}

void Legend::setColor( uint dataset, const QColor& color )
{
    d->brushes[dataset] = color;
}

void Legend::setBrush( uint dataset, const QBrush& brush )
{
    d->brushes[dataset] = brush;
}

QBrush Legend::brush( uint dataset ) const
{
    if( d->brushes.find( dataset ) != d->brushes.end() )
        return d->brushes[dataset];
    else
        return d->modelBrushes[ dataset ];
}


void Legend::setBrushesFromDiagram( KDChart::AbstractDiagram* diagram )
{
    QList<QBrush> datasetBrushes = diagram->datasetBrushes();
    for( int i = 0; i < datasetBrushes.count(); i++ )
        d->brushes[i] = datasetBrushes[i];
}


void Legend::setPen( uint dataset, const QPen& pen )
{
    d->pens[dataset] = pen;
}

QPen Legend::pen( uint dataset ) const
{
    if( d->pens.find( dataset ) != d->pens.end() )
        return d->pens[dataset];
    else
        return d->modelPens[ dataset ];
}


void Legend::setMarkerAttributes( uint dataset, const MarkerAttributes& markerAttributes )
{
    d->markerAttributes[dataset] = markerAttributes;

    buildLegend();
}

MarkerAttributes Legend::markerAttributes( uint dataset ) const
{
    if( d->markerAttributes.find( dataset ) != d->markerAttributes.end() )
        return d->markerAttributes[dataset];
    else
        return d->modelMarkers[ dataset ];
}

void Legend::setTextAttributes( const TextAttributes &a )
{
    d->textAttributes = a;

    buildLegend();
}

TextAttributes Legend::textAttributes() const
{
    return d->textAttributes;
}

void Legend::setTitleText( const QString& text )
{
    d->titleText = text;

    buildLegend();
}

QString Legend::titleText() const
{
    return d->titleText;
}

void Legend::setTitleTextAttributes( const TextAttributes &a )
{
    d->titleTextAttributes = a;

    buildLegend();
}

TextAttributes Legend::titleTextAttributes() const
{
    return d->titleTextAttributes;
}

void Legend::setSpacing( uint space )
{
    d->spacing = space;
    d->layout->setSpacing( space );

    buildLegend();
}

uint Legend::spacing() const
{
    return d->spacing;
}

bool Legend::mustDrawVerticalLegend() const
{
    return orientation() == Qt::Vertical ||
      position() == West ||
      position() == East ||
      position() == NorthWest ||
      position() == NorthEast ||
      position() == SouthWest ||
      position() == SouthEast;

    // PENDING(hansen)
    /* TODO: Review */
    /* Those positions seem not to be
       part of the new API /steffen */
    /* position() == LegendTopLeftLeft ||
       position() == LegendTopRightRight ||
        position() == LegendBottomLeftLeft ||
        position() == LegendBottomRightRight;
    */
}


void Legend::setDefaultColors()
{
    setColor(  0, Qt::red );
    setColor(  1, Qt::green );
    setColor(  2, Qt::blue );
    setColor(  3, Qt::cyan );
    setColor(  4, Qt::magenta );
    setColor(  5, Qt::yellow );
    setColor(  6, Qt::darkRed );
    setColor(  7, Qt::darkGreen );
    setColor(  8, Qt::darkBlue );
    setColor(  9, Qt::darkCyan );
    setColor( 10, Qt::darkMagenta );
    setColor( 11, Qt::darkYellow );
}

void Legend::setRainbowColors()
{
    setColor(  0, QColor(255,  0,196) );
    setColor(  1, QColor(255,  0, 96) );
    setColor(  2, QColor(255, 128,64) );
    setColor(  3, Qt::yellow );
    setColor(  4, Qt::green );
    setColor(  5, Qt::cyan );
    setColor(  6, QColor( 96, 96,255) );
    setColor(  7, QColor(160,  0,255) );
    for( int i=8; i<16; ++i )
        setColor( i, brush(i-8).color().light() );
}

void Legend::setSubduedColors( bool ordered )
{
static const int NUM_SUBDUEDCOLORS = 18;
static const QColor SUBDUEDCOLORS[ NUM_SUBDUEDCOLORS ] = {
    QColor( 0xe0,0x7f,0x70 ),
    QColor( 0xe2,0xa5,0x6f ),
    QColor( 0xe0,0xc9,0x70 ),
    QColor( 0xd1,0xe0,0x70 ),
    QColor( 0xac,0xe0,0x70 ),
    QColor( 0x86,0xe0,0x70 ),
    QColor( 0x70,0xe0,0x7f ),
    QColor( 0x70,0xe0,0xa4 ),
    QColor( 0x70,0xe0,0xc9 ),
    QColor( 0x70,0xd1,0xe0 ),
    QColor( 0x70,0xac,0xe0 ),
    QColor( 0x70,0x86,0xe0 ),
    QColor( 0x7f,0x70,0xe0 ),
    QColor( 0xa4,0x70,0xe0 ),
    QColor( 0xc9,0x70,0xe0 ),
    QColor( 0xe0,0x70,0xd1 ),
    QColor( 0xe0,0x70,0xac ),
    QColor( 0xe0,0x70,0x86 ),
};
    if( ordered )
        for(int i=0; i<NUM_SUBDUEDCOLORS; ++i)
            setColor( i, SUBDUEDCOLORS[i] );
    else{
        setColor( 0, SUBDUEDCOLORS[ 0] );
        setColor( 1, SUBDUEDCOLORS[ 5] );
        setColor( 2, SUBDUEDCOLORS[10] );
        setColor( 3, SUBDUEDCOLORS[15] );
        setColor( 4, SUBDUEDCOLORS[ 2] );
        setColor( 5, SUBDUEDCOLORS[ 7] );
        setColor( 6, SUBDUEDCOLORS[12] );
        setColor( 7, SUBDUEDCOLORS[17] );
        setColor( 8, SUBDUEDCOLORS[ 4] );
        setColor( 9, SUBDUEDCOLORS[ 9] );
        setColor(10, SUBDUEDCOLORS[14] );
        setColor(11, SUBDUEDCOLORS[ 1] );
        setColor(12, SUBDUEDCOLORS[ 6] );
        setColor(13, SUBDUEDCOLORS[11] );
        setColor(14, SUBDUEDCOLORS[16] );
        setColor(15, SUBDUEDCOLORS[ 3] );
        setColor(16, SUBDUEDCOLORS[ 8] );
        setColor(17, SUBDUEDCOLORS[13] );
    }
}

void Legend::resetDiagram()
{
    setDiagram( NULL );
}


void Legend::buildLegend()
{
    qDebug() << "************buildLegend()";

    if( d->blockBuildLegend )
        return;

    foreach( QLayoutItem* layoutItem, d->layoutItems ) {
        d->layout->removeItem( layoutItem );
    }
    qDeleteAll( d->layoutItems );
    d->layoutItems.clear();

    if( diagram() ) {
        d->modelLabels = diagram()->datasetLabels();
        d->modelBrushes = diagram()->datasetBrushes();
        d->modelPens = diagram()->datasetPens();
        d->modelMarkers = diagram()->datasetMarkers();
    }
    Q_ASSERT( d->modelLabels.count() == d->modelBrushes.count() );

    // legend caption
    if( !titleText().isEmpty() && titleTextAttributes().isVisible() ) {
        // PENDING(kalle) Other properties!
        KDChart::TextLayoutItem* titleItem = new KDChart::TextLayoutItem( titleText(),
                                                                          titleTextAttributes().font(),
                                                                          titleTextAttributes().color(),
                                                                          Qt::AlignCenter );
        d->layoutItems << titleItem;
        d->layout->addItem( titleItem, 0, 0, 1, 4, Qt::AlignCenter );
    }

    for ( int dataset = 0; dataset < d->modelLabels.count(); dataset++ ) {
        // PENDING(kalle) Properties
        KDChart::MarkerLayoutItem* markerItem = new KDChart::MarkerLayoutItem( diagram(),
                                                                               markerAttributes( dataset ),
                                                                               brush( dataset ),
                                                                               pen( dataset ),
                                                                               Qt::AlignLeft );
        d->layoutItems << markerItem;
        d->layout->addItem( markerItem, dataset+1 /*first row is title*/, 1, 1, 1, Qt::AlignCenter );


        // PENDING(kalle) Other properties!
        KDChart::TextLayoutItem* labelItem = new KDChart::TextLayoutItem( text( dataset ),
                                                                          textAttributes().font(),
                                                                          textAttributes().color(),
                                                                          Qt::AlignLeft );
        d->layoutItems << labelItem;
        d->layout->addItem( labelItem, dataset+1 /*first row is title*/, 2 );
    }

}

QSize KDChart::Legend::sizeHint() const
{
    QSize s = KDChartArea::sizeHint();
    return s;
}


QSize KDChart::Legend::minimumSizeHint() const
{
    QSize s = KDChartArea::minimumSizeHint();
    return s;
}
