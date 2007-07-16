/****************************************************************************
 ** Copyright (C) 2006 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include <QPainter>
#include <QDebug>

#include "KDChartBarDiagram.h"
#include "KDChartBarDiagram_p.h"
#include "KDChartThreeDBarAttributes.h"
#include "KDChartPosition.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractGrid.h"

#include <KDABLibFakes>

#include "KDChartBarDiagram_p.h"
#include "KDChartNormalBarDiagram_p.h"
#include "KDChartStackedBarDiagram_p.h"
#include "KDChartPercentBarDiagram_p.h"


using namespace KDChart;

BarDiagram::Private::Private()
{
}

BarDiagram::Private::~Private() {}

#define d d_func()


BarDiagram::BarDiagram( QWidget* parent, CartesianCoordinatePlane* plane ) :
    AbstractCartesianDiagram( new Private(), parent, plane )
{
    init();
}

void BarDiagram::init()
{
    d->diagram = this;
    d->normalDiagram = new NormalBarDiagram( this );
    d->stackedDiagram = new StackedBarDiagram( this );
    d->percentDiagram = new PercentBarDiagram( this );
    d->implementor = d->normalDiagram;
    d->compressor.setModel( attributesModel() );
}

BarDiagram::~BarDiagram()
{
}

BarDiagram * BarDiagram::clone() const
{

    BarDiagram* newDiagram = new BarDiagram( new Private( *d ) );
    newDiagram->setType( type() );
    return newDiagram;
}

bool BarDiagram::compare( const BarDiagram* other )const
{
    if( other == this ) return true;
    if( ! other ){
        return false;
    }

    return  // compare the base class
            ( static_cast<const AbstractCartesianDiagram*>(this)->compare( other ) ) &&
            // compare own properties
            (type() == other->type());
}


void BarDiagram::setType( const BarType type )
{
    //if ( type == d->barType ) return;
     if ( d->implementor->type() == type ) return;

     switch( type ) {
     case Normal:
         d->implementor = d->normalDiagram;
         break;
     case Stacked:
         d->implementor = d->stackedDiagram;
         break;
     case Percent:
         d->implementor = d->percentDiagram;
         break;
     default:
         Q_ASSERT_X( false, "BarDiagram::setType", "unknown diagram subtype" );
     };

   Q_ASSERT( d->implementor->type() == type );

   //d->barType = type;
    // AbstractAxis settings - see AbstractDiagram and CartesianAxis
    setPercentMode( type == BarDiagram::Percent );
    setDataBoundariesDirty();
    emit layoutChanged( this );
    emit propertiesChanged();
}


BarDiagram::BarType BarDiagram::type() const
{
    return d->implementor->type();
}

void BarDiagram::setBarAttributes( const BarAttributes & ta )
{
    d->attributesModel->setModelData( qVariantFromValue( ta ), BarAttributesRole );
    emit propertiesChanged();
}

void BarDiagram::setBarAttributes( int column, const BarAttributes & ta )
{
    d->attributesModel->setHeaderData(
        column, Qt::Vertical,
        qVariantFromValue( ta ),
        BarAttributesRole );
    emit propertiesChanged();
}

void BarDiagram::setBarAttributes( const QModelIndex & index, const BarAttributes & ta )
{
    attributesModel()->setData(
        d->attributesModel->mapFromSource( index ),
        qVariantFromValue( ta ),
        BarAttributesRole );
    emit propertiesChanged();
}

BarAttributes BarDiagram::barAttributes() const
{
    return qVariantValue<BarAttributes>(
        d->attributesModel->data( KDChart::BarAttributesRole ) );
}

BarAttributes BarDiagram::barAttributes( int column ) const
{
    return qVariantValue<BarAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::BarAttributesRole ) );
}

BarAttributes BarDiagram::barAttributes( const QModelIndex & index ) const
{
    return qVariantValue<BarAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            KDChart::BarAttributesRole ) );
}

void BarDiagram::setThreeDBarAttributes( const ThreeDBarAttributes & threeDAttrs )
{
    setDataBoundariesDirty();
    d->attributesModel->setModelData( qVariantFromValue( threeDAttrs ), ThreeDBarAttributesRole );
    //emit layoutChanged( this );
     emit propertiesChanged();
}

void BarDiagram::setThreeDBarAttributes( int column, const ThreeDBarAttributes & threeDAttrs )
{
    setDataBoundariesDirty();
    d->attributesModel->setHeaderData(
        column, Qt::Vertical,
        qVariantFromValue( threeDAttrs ),
        ThreeDBarAttributesRole );
    //emit layoutChanged( this );
    emit propertiesChanged();

}

void BarDiagram::setThreeDBarAttributes( const QModelIndex & index, const ThreeDBarAttributes & threeDAttrs )
{
    setDataBoundariesDirty();
    d->attributesModel->setData(
        d->attributesModel->mapFromSource(index),
        qVariantFromValue( threeDAttrs ),
        ThreeDBarAttributesRole );
    //emit layoutChanged( this );
    emit propertiesChanged();
}

ThreeDBarAttributes BarDiagram::threeDBarAttributes() const
{
    return qVariantValue<ThreeDBarAttributes>(
        d->attributesModel->data( KDChart::ThreeDBarAttributesRole ) );
}

ThreeDBarAttributes BarDiagram::threeDBarAttributes( int column ) const
{
    return qVariantValue<ThreeDBarAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::ThreeDBarAttributesRole ) );
}

ThreeDBarAttributes BarDiagram::threeDBarAttributes( const QModelIndex & index ) const
{
    return qVariantValue<ThreeDBarAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource(index),
            KDChart::ThreeDBarAttributesRole ) );
}

double BarDiagram::threeDItemDepth( const QModelIndex & index ) const
{
    return threeDBarAttributes( index ).validDepth();
}

double BarDiagram::threeDItemDepth( int column ) const
{
    return qVariantValue<ThreeDBarAttributes>(
        d->attributesModel->headerData (
            column,
            Qt::Vertical,
            KDChart::ThreeDBarAttributesRole ) ).validDepth();
}

void BarDiagram::resizeEvent ( QResizeEvent*)
{

}

const QPair<QPointF, QPointF> BarDiagram::calculateDataBoundaries() const
{
    if ( !checkInvariants(true) ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );

    // note: calculateDataBoundaries() is ignoring the hidden flags.
    // That's not a bug but a feature: Hiding data does not mean removing them.
    // For totally removing data from KD Chart's view people can use e.g. a proxy model
    // calculate boundaries for different line types Normal - Stacked - Percent - Default Normal
    return d->implementor->calculateDataBoundaries();

}


void BarDiagram::paintEvent ( QPaintEvent*)
{
    qDebug() << "BarDiagram::paintEvent";
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}



void BarDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants( true ) ) return;
    if ( !AbstractGrid::isBoundariesValid(dataBoundaries()) ) return;
    const PainterSaver p( ctx->painter() );
    if( model()->rowCount() == 0 || model()->columnCount() == 0 )
        return; // nothing to paint for us

    AbstractCoordinatePlane* const plane = ctx->coordinatePlane();
    ctx->setCoordinatePlane( plane->sharedAxisMasterPlane( ctx->painter() ) );


    // paint different bar types Normal - Stacked - Percent - Default Normal
    d->implementor->paint( ctx );

    ctx->setCoordinatePlane( plane );
}



void BarDiagram::resize ( const QSizeF& )
{
}

const int BarDiagram::numberOfAbscissaSegments () const
{
    return d->attributesModel->rowCount(attributesModelRootIndex());
}

const int BarDiagram::numberOfOrdinateSegments () const
{
    return d->attributesModel->columnCount(attributesModelRootIndex());
}

//#undef d
