/****************************************************************************
** Copyright (C) 2001-2011 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractCartesianDiagram_p.h"
#include "KDChartPaintContext.h"
#include <QDebug>
#include <QPainter>

#include <KDABLibFakes>


using namespace KDChart;

AbstractCartesianDiagram::Private::Private()
    : referenceDiagram( 0 )
{
    qRegisterMetaType< QModelIndex >( "QModelIndex" );
}

AbstractCartesianDiagram::Private::~Private()
{
}

bool AbstractCartesianDiagram::compare( const AbstractCartesianDiagram* other )const
{
    if( other == this ) return true;
    if( ! other ){
        //qDebug() << "AbstractCartesianDiagram::compare() cannot compare to Null pointer";
        return false;
    }
    /*
    qDebug() << "\n             AbstractCartesianDiagram::compare():";
            // compare own properties
    qDebug() <<
            ((referenceDiagram() == other->referenceDiagram()) &&
            ((! referenceDiagram()) || (referenceDiagramOffset() == other->referenceDiagramOffset())));
    */
    return  // compare the base class
            ( static_cast<const AbstractDiagram*>(this)->compare( other ) ) &&
            // compare own properties
            (referenceDiagram() == other->referenceDiagram()) &&
            ((! referenceDiagram()) || (referenceDiagramOffset() == other->referenceDiagramOffset()));
}


#define d d_func()

AbstractCartesianDiagram::AbstractCartesianDiagram ( QWidget* parent, CartesianCoordinatePlane* plane )
    : AbstractDiagram ( new Private(), parent, plane )
{
    init();
}

KDChart::AbstractCartesianDiagram::~AbstractCartesianDiagram()
{
    Q_FOREACH( CartesianAxis* axis, d->axesList ) {
        axis->deleteObserver( this );
    }
    d->axesList.clear();
}

void AbstractCartesianDiagram::init()
{
    d->compressor.setModel( attributesModel() );
    connect( this, SIGNAL( layoutChanged( AbstractDiagram* ) ),
             &( d->compressor ), SLOT( slotDiagramLayoutChanged( AbstractDiagram* ) ) );
}

void AbstractCartesianDiagram::addAxis( CartesianAxis *axis )
{
    if ( !d->axesList.contains( axis ) ) {
        d->axesList.append( axis );
        axis->createObserver( this );
        layoutPlanes();
    }
}

void AbstractCartesianDiagram::takeAxis( CartesianAxis *axis )
{
    const int idx = d->axesList.indexOf( axis );
    if( idx != -1 )
        d->axesList.takeAt( idx );
    axis->deleteObserver( this );
    axis->setParentWidget( 0 );
    layoutPlanes();
}

KDChart::CartesianAxisList AbstractCartesianDiagram::axes( ) const
{
    return d->axesList;
}

void KDChart::AbstractCartesianDiagram::layoutPlanes()
{
    //qDebug() << "KDChart::AbstractCartesianDiagram::layoutPlanes()";
    AbstractCoordinatePlane* plane = coordinatePlane();
    if( plane ){
        plane->layoutPlanes();
        //qDebug() << "KDChart::AbstractCartesianDiagram::layoutPlanes() OK";
    }
}

void KDChart::AbstractCartesianDiagram::setCoordinatePlane( AbstractCoordinatePlane* plane )
{
    if( coordinatePlane() ) {
        disconnect( attributesModel(), SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ),
                 coordinatePlane(), SLOT( relayout() ) );
        disconnect( attributesModel(), SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
                 coordinatePlane(), SLOT( relayout() ) );
        disconnect( attributesModel(), SIGNAL( columnsRemoved( const QModelIndex&, int, int ) ),
                 coordinatePlane(), SLOT( relayout() ) );
        disconnect( attributesModel(), SIGNAL( columnsInserted( const QModelIndex&, int, int ) ),
                 coordinatePlane(), SLOT( relayout() ) );
        disconnect( coordinatePlane() );
    }
    
    AbstractDiagram::setCoordinatePlane(plane);
    if ( plane ) {
        // Readjust the layout when the dataset count changes
        connect( attributesModel(), SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ),
                 plane, SLOT( relayout() ), Qt::QueuedConnection );
        connect( attributesModel(), SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
                 plane, SLOT( relayout() ), Qt::QueuedConnection );
        connect( attributesModel(), SIGNAL( columnsRemoved( const QModelIndex&, int, int ) ),
                 plane, SLOT( relayout() ), Qt::QueuedConnection );
        connect( attributesModel(), SIGNAL( columnsInserted( const QModelIndex&, int, int ) ),
                 plane, SLOT( relayout() ), Qt::QueuedConnection );
    }
    // show the axes, after all have been layoutPlanes
    // (because they might depend on each other)
    /*
    if( plane )
        Q_FOREACH( CartesianAxis* axis, d->axesList )
            axis->show();
    else
        Q_FOREACH( CartesianAxis* axis, d->axesList )
            axis->hide();
    */
}

void AbstractCartesianDiagram::setReferenceDiagram( AbstractCartesianDiagram* diagram, const QPointF& offset )
{
    d->referenceDiagram = diagram;
    d->referenceDiagramOffset = offset;
}

AbstractCartesianDiagram* AbstractCartesianDiagram::referenceDiagram() const
{
    return d->referenceDiagram;
}

QPointF AbstractCartesianDiagram::referenceDiagramOffset() const
{
    return d->referenceDiagramOffset;
}

void AbstractCartesianDiagram::setRootIndex( const QModelIndex& index )
{
    AbstractDiagram::setRootIndex( index );
    d->compressor.setRootIndex( attributesModel()->mapFromSource( index ) );
}

void AbstractCartesianDiagram::setModel( QAbstractItemModel* model )
{
    AbstractDiagram::setModel( model );
    d->compressor.setModel( attributesModel() );
}

void AbstractCartesianDiagram::setAttributesModel( AttributesModel* model )
{
    AbstractDiagram::setAttributesModel( model );
    d->compressor.setModel( attributesModel() );
}
