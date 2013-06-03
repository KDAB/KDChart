/****************************************************************************
** Copyright (C) 2001-2013 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartAbstractPieDiagram.h"
#include "KDChartAbstractPieDiagram_p.h"

#include "KDChartAttributesModel.h"
#include "KDChartPieAttributes.h"
#include "KDChartThreeDPieAttributes.h"

#include <QMap>

#include <KDABLibFakes>


using namespace KDChart;

AbstractPieDiagram::Private::Private()
    : granularity( 1.0 )
    , autoRotateLabels( false )
{
}

AbstractPieDiagram::Private::~Private() {}

AbstractPieDiagram::AbstractPieDiagram( QWidget* parent, PolarCoordinatePlane *plane ) :
    AbstractPolarDiagram( new Private(), parent, plane )
{
    init();
}

AbstractPieDiagram::~AbstractPieDiagram()
{
}


void AbstractPieDiagram::init()
{
}


bool AbstractPieDiagram::compare( const AbstractPieDiagram* other ) const
{
    if ( other == this ) return true;
    if ( ! other ) {
        //qDebug() << "AbstractPieDiagram::compare() cannot compare to Null pointer";
        return false;
    }
    /*
    qDebug() << "\n             AbstractPieDiagram::compare():";
            // compare own properties
    qDebug() <<
            (granularity() == other->granularity()) &&
            (startPosition() == other->startPosition());
    */
    return  // compare the base class
            ( static_cast<const AbstractPolarDiagram*>(this)->compare( other ) ) &&
            // compare own properties
            (granularity() == other->granularity()) &&
            (startPosition() == other->startPosition());
}


#define d d_func()

void AbstractPieDiagram::setGranularity( qreal value )
{
    d->granularity = value;
}

qreal AbstractPieDiagram::granularity() const
{
    return (d->granularity < 0.05 || d->granularity > 36.0)
            ? 1.0
    : d->granularity;
}


void AbstractPieDiagram::setStartPosition( int degrees )
{
    Q_UNUSED( degrees );
    qWarning() << "Deprecated AbstractPieDiagram::setStartPosition() called, setting ignored.";
}

int AbstractPieDiagram::startPosition() const
{
    qWarning() << "Deprecated AbstractPieDiagram::startPosition() called.";
    return 0;
}

void AbstractPieDiagram::setAutoRotateLabels( bool autoRotate )
{
    d->autoRotateLabels = autoRotate;
}

bool AbstractPieDiagram::autoRotateLabels() const
{
    return d->autoRotateLabels;
}

void AbstractPieDiagram::setPieAttributes( const PieAttributes & attrs )
{
    d->attributesModel->setModelData( qVariantFromValue( attrs ), PieAttributesRole );
    emit layoutChanged( this );
}

void AbstractPieDiagram::setPieAttributes( int column, const PieAttributes & attrs )
{
    d->setDatasetAttrs( column, qVariantFromValue( attrs ), PieAttributesRole );
    emit layoutChanged( this );
}

void AbstractPieDiagram::setPieAttributes( const QModelIndex & index, const PieAttributes & attrs )
{
	d->attributesModel->setData( index, qVariantFromValue( attrs), PieAttributesRole );
	emit layoutChanged( this );
}

// Note: Our users NEED this method - even if
//       we do not need it at drawing time!
//       (khz, 2006-07-28)
PieAttributes AbstractPieDiagram::pieAttributes() const
{
    return d->attributesModel->data( PieAttributesRole ).value<PieAttributes>();
}

// Note: Our users NEED this method - even if
//       we do not need it at drawing time!
//       (khz, 2006-07-28)
PieAttributes AbstractPieDiagram::pieAttributes( int column ) const
{
    const QVariant attrs( d->datasetAttrs( column, PieAttributesRole ) );
    if ( attrs.isValid() )
        return attrs.value< PieAttributes >();
    return pieAttributes();
}

PieAttributes AbstractPieDiagram::pieAttributes( const QModelIndex & index ) const
{
    return d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            PieAttributesRole ).value<PieAttributes>();
}


void AbstractPieDiagram::setThreeDPieAttributes( const ThreeDPieAttributes & tda )
{
    d->attributesModel->setModelData( qVariantFromValue( tda ), ThreeDPieAttributesRole );
    emit layoutChanged( this );
}

void AbstractPieDiagram::setThreeDPieAttributes( int column, const ThreeDPieAttributes & tda )
{
    d->setDatasetAttrs( column, qVariantFromValue( tda ), ThreeDPieAttributesRole );
    emit layoutChanged( this );
}

void AbstractPieDiagram::setThreeDPieAttributes( const QModelIndex & index, const ThreeDPieAttributes & tda )
{
    model()->setData( index, qVariantFromValue( tda ), ThreeDPieAttributesRole );
    emit layoutChanged( this );
}

// Note: Our users NEED this method - even if
//       we do not need it at drawing time!
//       (khz, 2006-07-28)
ThreeDPieAttributes AbstractPieDiagram::threeDPieAttributes() const
{
    return d->attributesModel->data( ThreeDPieAttributesRole ).value<ThreeDPieAttributes>();
}

// Note: Our users NEED this method - even if
//       we do not need it at drawing time!
//       (khz, 2006-07-28)
ThreeDPieAttributes AbstractPieDiagram::threeDPieAttributes( int column ) const
{
    const QVariant attrs( d->datasetAttrs( column, ThreeDPieAttributesRole ) );
    if ( attrs.isValid() )
        return attrs.value< ThreeDPieAttributes >();
    return threeDPieAttributes();
}

ThreeDPieAttributes AbstractPieDiagram::threeDPieAttributes( const QModelIndex & index ) const
{
    return d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            ThreeDPieAttributesRole ).value<ThreeDPieAttributes>();
}

