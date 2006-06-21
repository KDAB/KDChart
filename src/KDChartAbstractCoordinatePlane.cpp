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


#include <QGridLayout>

#include "KDChartAbstractCoordinatePlane.h"
#include "KDChartGridAttributes.h"

using namespace KDChart;

class AbstractCoordinatePlane::Private
{
public:
    Private()
    :referenceCoordinatePlane(0)
    {}

    AbstractDiagramList diagrams;
    GridAttributes gridAttributes;
    AbstractCoordinatePlane *referenceCoordinatePlane;
};

AbstractCoordinatePlane::AbstractCoordinatePlane ( QWidget* parent )
    : QWidget ( parent )
    , d ( new Private() )
{
}

AbstractCoordinatePlane::~AbstractCoordinatePlane()
{
    emit destroyedCoordinatePlane( this );
}

void AbstractCoordinatePlane::addDiagram ( AbstractDiagram* diagram )
{
    // diagrams are invisible and paint through their paint() method
    diagram->hide();

    d->diagrams.append ( diagram );
    diagram->setParent ( this );
    diagram->setCoordinatePlane( this );
    layoutDiagrams();
    emit diagramsChanged();
}

/*virtual*/
void AbstractCoordinatePlane::replaceDiagram ( AbstractDiagram* diagram, AbstractDiagram* oldDiagram )
{
    if( diagram && oldDiagram != diagram ){
        if( d->diagrams.count() ){
            if( ! oldDiagram )
                takeDiagram( d->diagrams.first() );
            else
                takeDiagram( oldDiagram );
        }
        delete oldDiagram;
        addDiagram( diagram );
    }
}

/*virtual*/
void AbstractCoordinatePlane::takeDiagram ( AbstractDiagram* diagram )
{
    const int idx = d->diagrams.indexOf( diagram );
    if( idx != -1 ){
        d->diagrams.takeAt( idx );
        diagram->setParent ( 0 );
        diagram->setCoordinatePlane( 0 );
    }
}


AbstractDiagram* AbstractCoordinatePlane::diagram()
{
    if ( d->diagrams.isEmpty() )
    {
        return 0;
    } else {
        return d->diagrams.first();
    }
}

AbstractDiagramList AbstractCoordinatePlane::diagrams()
{
    return d->diagrams;
}

QSize KDChart::AbstractCoordinatePlane::sizeHint() const
{
    return QSize( 1000, 1000 );
}

QSize KDChart::AbstractCoordinatePlane::minimumSizeHint() const
{
    return QSize( 200, 200 );
}


QSizePolicy KDChart::AbstractCoordinatePlane::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}

void KDChart::AbstractCoordinatePlane::setGridAttributes( const GridAttributes& a )
{
    d->gridAttributes = a;
    update();
}

GridAttributes KDChart::AbstractCoordinatePlane::gridAttributes() const
{
    return d->gridAttributes;
}

void KDChart::AbstractCoordinatePlane::setReferenceCoordinatePlane( AbstractCoordinatePlane * plane )
{
    d->referenceCoordinatePlane = plane;
}

AbstractCoordinatePlane * KDChart::AbstractCoordinatePlane::referenceCoordinatePlane( ) const
{
    return d->referenceCoordinatePlane;
}
