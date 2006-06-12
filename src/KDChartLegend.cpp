/****************************************************************************
 ** Copyright (C) 2006 Klar�vdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

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

Legend::Private::Private() :
    referenceArea(0),
    position( Position::East ),
    orientation( Qt::Vertical ),
    showLines( false ),
    texts(),
    textAttributes(),
    titleText( QObject::tr( "Legend" ) ),
    titleTextAttributes(),
    spacing( 1 ),
    observer( NULL ),
    needRebuild( false )
{
}

Legend::Private::~Private()
{
}



#define d d_func()

Legend::Legend( QWidget* parent ) :
    AbstractArea( new Private(), parent )
{
    d->referenceArea = parent;
    init();
}

Legend::Legend( KDChart::AbstractDiagram* diagram, QWidget* parent ) :
    AbstractArea( new Private(), parent )
{
    d->referenceArea = parent;
    init();
    setDiagram( diagram );
}

Legend::~Legend()
{
    emit destroyedLegend( this );
}

void Legend::init()
{
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    d->layout = new QGridLayout();
    d->layout->setMargin( 2 );
    d->layout->setSpacing( d->spacing );
    setLayout( d->layout );

    TextAttributes textAttrs;
    textAttrs.setColor( Qt::black );
    textAttrs.setFont( QFont( "helvetica", 10, QFont::Normal, false ) );
    textAttrs.setFontSize( 16 );
    setTextAttributes( textAttrs );
    TextAttributes titleTextAttrs;
    titleTextAttrs.setColor( Qt::black );
    titleTextAttrs.setFont( QFont( "helvetica", 12, QFont::Bold, false ) );
    titleTextAttrs.setFontSize( 20 );
    setTitleTextAttributes( titleTextAttrs );

    d->position = Position::NorthEast;
    d->useHorizontalSpace = true;
    d->useVerticalSpace   = false;
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
    // re-calculate/re-position the Legend's layout and contents, if needed:
    buildLegend();

    // Legend always has a frame around it, might be overpainted by Area afterwards.
    QPainter painter( this );
    painter.drawRect( evt->rect().adjusted( 1, 1, -1, -1 ) );

    // Paint the background and frame
    AbstractArea::paintEvent( evt );

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


/**
    Specifies the reference area for font size of title text,
    and for font size of the item texts, IF automatic area
    detection is set.

    \note This parameter is ignored, if the Measure given for
    setTitleTextAttributes (or setTextAttributes, resp.) is
    not specifying automatic area detection.

    If no reference area is specified, but automatic area
    detection is set, then the size of the legend's parent
    widget will be used.

    \sa KDChart::Measure, KDChartEnums::MeasureCalculationMode
 */
void Legend::setReferenceArea( const QWidget* area )
{
    d->referenceArea = area;
    d->needRebuild = true;
}


/**
    Returns the reference area, that is used for font size of title text,
    and for font size of the item texts, IF automatic area
    detection is set.

    \sa setReferenceArea
 */
const QWidget* Legend::referenceArea() const
{
    return (d->referenceArea ? d->referenceArea : static_cast<const QWidget*>(parent()));
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
    d->needRebuild = true;
}

KDChart::AbstractDiagram* Legend::diagram() const
{
    return d->diagram;
}

void Legend::setPosition( Position position )
{
    d->position = position;

    d->needRebuild = true;
    emit positionChanged( this );
}


Position Legend::position() const
{
    return d->position;
}

void Legend::setUseHorizontalSpace( bool value )
{
    d->useHorizontalSpace = value;
    // Since we don't support floating legends yet, we make sure,
    // the legend is allways using space, in at least one direction:
    if( !value )
        d->useVerticalSpace = true;
    d->needRebuild = true;
}

bool Legend::useHorizontalSpace() const
{
    return d->useHorizontalSpace;
}

void Legend::setUseVerticalSpace( bool value )
{
    d->useVerticalSpace = value;
    // Since we don't support floating legends yet, we make sure,
    // the legend is allways using space, in at least one direction:
    if( !value )
        d->useHorizontalSpace = true;
    d->needRebuild = true;
}

bool Legend::useVerticalSpace() const
{
    return d->useVerticalSpace;
}

void Legend::setOrientation( Qt::Orientation orientation )
{
    d->orientation = orientation;
    d->needRebuild = true;
}

Qt::Orientation Legend::orientation() const
{
    return d->orientation;
}

void Legend::setShowLines( bool legendShowLines )
{
    d->showLines = legendShowLines;
    d->needRebuild = true;
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
    d->needRebuild = true;
}

void Legend::setText( uint dataset, const QString& text )
{
    d->texts[dataset] = text;
    d->needRebuild = true;
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
    d->needRebuild = true;
}

void Legend::setBrush( uint dataset, const QBrush& brush )
{
    d->brushes[dataset] = brush;
    d->needRebuild = true;
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
    d->needRebuild = true;
}


void Legend::setPen( uint dataset, const QPen& pen )
{
    d->pens[dataset] = pen;
    d->needRebuild = true;
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
    d->needRebuild = true;
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
    d->needRebuild = true;
}

TextAttributes Legend::textAttributes() const
{
    return d->textAttributes;
}

void Legend::setTitleText( const QString& text )
{
    d->titleText = text;
    d->needRebuild = true;
}

QString Legend::titleText() const
{
    return d->titleText;
}

void Legend::setTitleTextAttributes( const TextAttributes &a )
{
    d->titleTextAttributes = a;
    d->needRebuild = true;
}

TextAttributes Legend::titleTextAttributes() const
{
    return d->titleTextAttributes;
}

void Legend::setSpacing( uint space )
{
    d->spacing = space;
    d->layout->setSpacing( space );
    d->needRebuild = true;
}

uint Legend::spacing() const
{
    return d->spacing;
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
    if( ! d->needRebuild ) return;
    d->needRebuild = false;

    foreach( QLayoutItem* layoutItem, d->layoutItems ) {
        d->layout->removeItem( layoutItem );
    }
    qDeleteAll( d->layoutItems );
    d->layoutItems.clear();

    if( orientation() == Qt::Vertical ) {
        d->layout->setColumnStretch( 4, 1 );
    } else {
        d->layout->setColumnStretch( 4, 0 );
    }

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
        KDChart::TextLayoutItem* titleItem =
            new KDChart::TextLayoutItem( titleText(),
                titleTextAttributes(),
                referenceArea(),
                (orientation() == Qt::Vertical)
                ? KDChartEnums::MeasureOrientationMinimum
                : KDChartEnums::MeasureOrientationHorizontal,
                Qt::AlignCenter );
        titleItem->setParentWidget( this );

        d->layoutItems << titleItem;
        if( orientation() == Qt::Vertical )
            d->layout->addItem( titleItem, 0, 0, 1, 5, Qt::AlignCenter );
        else
            d->layout->addItem( titleItem, 0, 0, 1, d->modelLabels.count()*4, Qt::AlignCenter );

        // The line between the title and the legend items, if any.
        if( showLines() ) {
            KDChart::HorizontalLineLayoutItem* lineItem = new KDChart::HorizontalLineLayoutItem();
            d->layoutItems << lineItem;
            if( orientation() == Qt::Vertical )
                d->layout->addItem( lineItem, 1, 0, 1, 5, Qt::AlignCenter );
            else
                d->layout->addItem( lineItem, 1, 0, 1, d->modelLabels.count()*4, Qt::AlignCenter );
        }

    }

    for ( int dataset = 0; dataset < d->modelLabels.count(); dataset++ ) {
        KDChart::MarkerLayoutItem* markerItem = new KDChart::MarkerLayoutItem( diagram(),
                                                                               markerAttributes( dataset ),
                                                                               brush( dataset ),
                                                                               pen( dataset ),
                                                                               Qt::AlignLeft );
        d->layoutItems << markerItem;
        if( orientation() == Qt::Vertical )
            d->layout->addItem( markerItem,
                                dataset*2+2, // first row is title, second is line
                                1,
                                1, 1, Qt::AlignCenter );
        else
            d->layout->addItem( markerItem,
                                2, // all in row two
                                dataset*4 );

        // PENDING(kalle) Other properties!
        KDChart::TextLayoutItem* labelItem =
            new KDChart::TextLayoutItem( text( dataset ),
                textAttributes(),
                referenceArea(),
                (orientation() == Qt::Vertical)
                ? KDChartEnums::MeasureOrientationMinimum
                : KDChartEnums::MeasureOrientationHorizontal,
                Qt::AlignLeft );
        labelItem->setParentWidget( this );

        d->layoutItems << labelItem;
        if( orientation() == Qt::Vertical )
            d->layout->addItem( labelItem,
                                dataset*2+2, // first row is title, second is line
                                3 );
        else
            d->layout->addItem( labelItem,
                                2, // all in row two
                                dataset*4+1 );

        // horizontal lines (only in vertical mode, and not after the last item)
        if( orientation() == Qt::Vertical && showLines() && dataset != ( d->modelLabels.count()-1 ) ) {
            KDChart::HorizontalLineLayoutItem* lineItem = new KDChart::HorizontalLineLayoutItem();
            d->layoutItems << lineItem;
            d->layout->addItem( lineItem,
                                dataset*2+1+2,
                                0,
                                1, 5, Qt::AlignCenter );
        }

        // vertical lines (only in horizontal mode, and not after the last item)
        if( orientation() == Qt::Horizontal && showLines() && dataset != ( d->modelLabels.count()-1 ) ) {
            KDChart::VerticalLineLayoutItem* lineItem = new KDChart::VerticalLineLayoutItem();
            d->layoutItems << lineItem;
            d->layout->addItem( lineItem,
                                2, // all in row two
                                dataset*4+2,
                                1, 1, Qt::AlignCenter );
        }

        if( orientation() != Qt::Vertical ) { // Horizontal needs a spacer
            d->layout->addItem( new QSpacerItem( spacing(), qMax( markerItem->sizeHint().height(),
                                                                  labelItem->sizeHint().height() ) ),
                                2, // all in row two
                                dataset*4+3 );
        }
    }

    // vertical line (only in vertical mode)
    if( orientation() == Qt::Vertical && showLines() ) {
        KDChart::VerticalLineLayoutItem* lineItem = new KDChart::VerticalLineLayoutItem();
        d->layoutItems << lineItem;
        d->layout->addItem( lineItem, 2, 2, d->modelLabels.count()*2, 1 );
    }

    d->layout->activate();
}

