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

#include <QMap>
#include <QtXml/QDomDocumentFragment>

#include "KDChartAbstractPieDiagram.h"
#include "KDChartAbstractPieDiagram_p.h"
#include "KDChartAttributesModel.h"
#include "KDChartThreeDPieAttributes.h"

using namespace KDChart;

AbstractPieDiagram::Private::Private() :
    explode( false ),
    startPosition( 0.0 )
{
}

AbstractPieDiagram::Private::~Private() {}

AbstractPieDiagram::AbstractPieDiagram( PolarCoordinatePlane *parent ) :
    AbstractPolarDiagram( new Private(), parent )
{
    init();
}

AbstractPieDiagram::~AbstractPieDiagram()
{
}


void AbstractPieDiagram::init()
{
}


#define d d_func()


QDomDocumentFragment AbstractPieDiagram::toXML() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: QDomDocumentFragment AbstractPieDiagram::toXML() const" );
    return QDomDocumentFragment();
}

void AbstractPieDiagram::setExplode( bool explode )
{
    d->explode = explode;
}

bool AbstractPieDiagram::explode() const
{
    return d->explode;
}

void AbstractPieDiagram::setExplodeFactor( double factor )
{
    d->attributesModel->setModelData( factor, ExplodeFactorRole );
    emit layoutChanged( this );
}

double AbstractPieDiagram::explodeFactor() const
{
    return d->attributesModel->modelData( ExplodeFactorRole ).toDouble();
}

void AbstractPieDiagram::setExplodeFactor( int dataset, double factor )
{
    d->attributesModel->setHeaderData( dataset, Qt::Vertical, factor, ExplodeFactorRole );
    emit layoutChanged( this );
}

double AbstractPieDiagram::explodeFactor( int dataset ) const
{
    return d->attributesModel->headerData( dataset, Qt::Vertical, ExplodeFactorRole ).toDouble();
}

void AbstractPieDiagram::setStartPosition( double degrees )
{
    d->startPosition = degrees;
}

double AbstractPieDiagram::startPosition() const
{
    return d->startPosition;
}


void AbstractPieDiagram::setThreeDPieAttributes( const ThreeDPieAttributes & tda )
{
    d->attributesModel->setModelData( qVariantFromValue( tda ), ThreeDPieAttributesRole );
    emit layoutChanged( this );
}

void AbstractPieDiagram::setThreeDPieAttributes( int column, const ThreeDPieAttributes & tda )
{
    d->attributesModel->setHeaderData( column, Qt::Vertical, qVariantFromValue( tda ), ThreeDPieAttributesRole );
    emit layoutChanged( this );
}

void AbstractPieDiagram::setThreeDPieAttributes( const QModelIndex & index, const ThreeDPieAttributes & tda )
{
    model()->setData( index, qVariantFromValue( tda ), ThreeDPieAttributesRole );
    emit layoutChanged( this );
}

ThreeDPieAttributes AbstractPieDiagram::threeDPieAttributes( const QModelIndex & index ) const
{
    return qVariantValue<ThreeDPieAttributes>( model()->data( index, KDChart::ThreeDPieAttributesRole ) );
}

























