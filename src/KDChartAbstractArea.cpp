/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartAbstractArea.h"
#include "KDChartAbstractArea_p.h"
#include <KDChartBackgroundAttributes.h>
#include <KDChartFrameAttributes.h>
#include <KDChartTextAttributes.h>
#include "KDChartPainterSaver_p.h"
#include <QtXml/QDomDocumentFragment>
#include <QPainter>

using namespace KDChart;

AbstractArea::Private::Private() :
    visible( true ),
    // PENDING(khz) dockingPointToPadding?, alignToDockingPoint?
    text(),
    textAttributes(),
    frameAttributes(),
    backgroundAttributes()
{
}


AbstractArea::Private::~Private() {}


void AbstractArea::Private::init()
{
}




AbstractArea::AbstractArea( QWidget* parent ) :
    QWidget( parent ), _d( new Private() )
{
}

AbstractArea::~AbstractArea()
{
    delete _d; _d = 0;
}


void AbstractArea::init()
{
}


#define d d_func()

QDomDocumentFragment AbstractArea::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment AbstractArea::toXML() const" );
    return QDomDocumentFragment();
}

#ifdef TEMPORARILY_REMOVED
void AbstractArea::paint( QPainter* painter, const PaintContext & context ) const
{
    Q_UNUSED( context );
    paintBackground( painter, d->backgroundAttributes, geometry() );

    // PENDING(kalle) Paint frame
}
#endif

void AbstractArea::alignToReferencePoint( const RelativePosition& position )
{
    Q_UNUSED( position );
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: void AbstractArea::alignToReferencePoint( const RelativePosition& position )" );
}

void AbstractArea::setText( const QString& text )
{
    d->text = text;
}

QString AbstractArea::text() const
{
    return d->text;
}

void AbstractArea::setTextAttributes( const TextAttributes &a )
{
    d->textAttributes = a;
}

TextAttributes AbstractArea::textAttributes() const
{
    return d->textAttributes;
}

void AbstractArea::setFrameAttributes( const FrameAttributes &a )
{
    d->frameAttributes = a;
}

FrameAttributes AbstractArea::frameAttributes() const
{
    return d->frameAttributes;
}

void AbstractArea::setBackgroundAttributes( const BackgroundAttributes &a )
{
    d->backgroundAttributes = a;
}

BackgroundAttributes AbstractArea::backgroundAttributes() const
{
    return d->backgroundAttributes;
}

void AbstractArea:: paintBackground( QPainter* painter,
                                    BackgroundAttributes attributes,
                                    const QRectF& rect )
{
    if( !attributes.isVisible() )
        return;

    /* first draw the brush (may contain a pixmap)*/
    if( Qt::NoBrush != attributes.brush().style() ) {
        KDChart::PainterSaver painterSaver( painter );
        painter->setPen( Qt::NoPen );
        const QPointF newTopLeft( painter->deviceMatrix().map( rect.topLeft() ) );
        painter->setBrushOrigin( newTopLeft );
        painter->setBrush( attributes.brush() );
        painter->drawRect( rect );
    }
    /* next draw the backPixmap over the brush */
    if( !attributes.pixmap().isNull() &&
        attributes.pixmapMode() != BackgroundAttributes::BackgroundPixmapModeNone ) {
        QPointF ol = rect.topLeft();
        if( BackgroundAttributes::BackgroundPixmapModeCentered == attributes.pixmapMode() )
        {
            ol.setX( rect.center().x() - attributes.pixmap().width() / 2 );
            ol.setY( rect.center().y() - attributes.pixmap().height()/ 2 );
            painter->drawPixmap( ol, attributes.pixmap() );
        } else {
            QMatrix m;
            double zW = (double)rect.width()  / (double)attributes.pixmap().width();
            double zH = (double)rect.height() / (double)attributes.pixmap().height();
            switch( attributes.pixmapMode() ) {
            case BackgroundAttributes::BackgroundPixmapModeScaled:
            {
                double z;
                z = qMin( zW, zH );
                m.scale( z, z );
            }
            break;
            case BackgroundAttributes::BackgroundPixmapModeStretched:
                m.scale( zW, zH );
                break;
            default:
                ; // Cannot happen, previously checked
            }
            QPixmap pm = attributes.pixmap().transformed( m );
            ol.setX( rect.center().x() - pm.width() / 2 );
            ol.setY( rect.center().y() - pm.height()/ 2 );
            painter->drawPixmap( ol, pm );
        }
    }
}


