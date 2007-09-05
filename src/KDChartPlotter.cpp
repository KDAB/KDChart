/****************************************************************************
 ** Copyright (C) 2006 Klaralvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundtion and appearing in the file LICENSE.GPL included in the
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

#include "KDChartPlotter.h"

#include "KDChartAbstractGrid.h"

#include <KDABLibFakes>

#include "KDChartPlotter_p.h"
#include "KDChartNormalPlotter_p.h"

using namespace KDChart;

Plotter::Private::Private()
{
}

Plotter::Private::~Private() {}


#define d d_func()


Plotter::Plotter( QWidget* parent, CartesianCoordinatePlane* plane ) :
    AbstractCartesianDiagram( new Private(), parent, plane )
{
    init();
}

void Plotter::init()
{
    d->diagram = this;
    d->normalPlotter = new NormalPlotter( this );
    d->implementor = d->normalPlotter;

    setDatasetDimension( 2 );
}

Plotter::~Plotter()
{
}

Plotter* Plotter::clone() const
{
    Plotter* newDiagram = new Plotter( new Private( *d ) );
    newDiagram->setType( type() );
    return newDiagram;
}

bool Plotter::compare( const Plotter* other )const
{
    if( other == this )
        return true;
    if( other == 0 )
        return false;
    return  // compare the base class
            ( static_cast< const AbstractCartesianDiagram* >( this )->compare( other ) ) &&
            // compare own properties
            ( type() == other->type() );
}

void Plotter::setType( const PlotType type )
{
    if ( d->implementor->type() == type ) return;
   if ( type != Plotter::Normal && datasetDimension() > 1 ) {
       Q_ASSERT_X ( false, "setType()",
                    "This line chart type can't be used with multi-dimensional data." );
       return;
   }
   switch( type ) {
   case Normal:
       d->implementor = d->normalPlotter;
       break;
   default:
       Q_ASSERT_X( false, "Plotter::setType", "unknown plotter subtype" );
   };

   // d->lineType = type;
   Q_ASSERT( d->implementor->type() == type );

   setDataBoundariesDirty();
   emit layoutChanged( this );
   emit propertiesChanged();
}

Plotter::PlotType Plotter::type() const
{
    return d->implementor->type();
}

void Plotter::setLineAttributes( const LineAttributes & ta )
{
    d->attributesModel->setModelData(
        qVariantFromValue( ta ),
        LineAttributesRole );
    emit propertiesChanged();
}

void Plotter::setLineAttributes(
        int column,
    const LineAttributes & ta )
{
    d->attributesModel->setHeaderData(
            column,
            Qt::Vertical,
            qVariantFromValue( ta ),
            LineAttributesRole );
    emit propertiesChanged();
}

void Plotter::resetLineAttributes( int column )
{
    d->attributesModel->resetHeaderData(
            column, Qt::Vertical, LineAttributesRole );
    emit propertiesChanged();
}

void Plotter::setLineAttributes(
        const QModelIndex & index,
    const LineAttributes & ta )
{
    d->attributesModel->setData(
            d->attributesModel->mapFromSource(index),
    qVariantFromValue( ta ),
    LineAttributesRole );
    emit propertiesChanged();
}

/**
 * Remove any explicit line attributes settings that might have been specified before.
 */
void Plotter::resetLineAttributes( const QModelIndex & index )
{
    d->attributesModel->resetData(
            d->attributesModel->mapFromSource(index), LineAttributesRole );
    emit propertiesChanged();
}

LineAttributes Plotter::lineAttributes() const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data( KDChart::LineAttributesRole ) );
}

LineAttributes Plotter::lineAttributes( int column ) const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::LineAttributesRole ) );
}

LineAttributes Plotter::lineAttributes(
    const QModelIndex & index ) const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource(index),
            KDChart::LineAttributesRole ) );
}

void Plotter::setThreeDLineAttributes(
    const ThreeDLineAttributes & ta )
{
    setDataBoundariesDirty();
    d->attributesModel->setModelData(
        qVariantFromValue( ta ),
        ThreeDLineAttributesRole );
   emit propertiesChanged();
}

void Plotter::setThreeDLineAttributes(
    int column,
    const ThreeDLineAttributes & ta )
{
    setDataBoundariesDirty();
    d->attributesModel->setHeaderData(
        column,
        Qt::Vertical,
        qVariantFromValue( ta ),
        ThreeDLineAttributesRole );
   emit propertiesChanged();
}

void Plotter::setThreeDLineAttributes(
    const QModelIndex & index,
    const ThreeDLineAttributes & ta )
{
    setDataBoundariesDirty();
    d->attributesModel->setData(
        d->attributesModel->mapFromSource(index),
        qVariantFromValue( ta ),
        ThreeDLineAttributesRole );
   emit propertiesChanged();
}

ThreeDLineAttributes Plotter::threeDLineAttributes() const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data( KDChart::ThreeDLineAttributesRole ) );
}

ThreeDLineAttributes Plotter::threeDLineAttributes( int column ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::ThreeDLineAttributesRole ) );
}

ThreeDLineAttributes Plotter::threeDLineAttributes(
    const QModelIndex & index ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            KDChart::ThreeDLineAttributesRole ) );
}

double Plotter::threeDItemDepth( const QModelIndex & index ) const
{
    return threeDLineAttributes( index ).validDepth();
}

double Plotter::threeDItemDepth( int column ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->headerData (
            column,
            Qt::Vertical,
            KDChart::ThreeDLineAttributesRole ) ).validDepth();
}

void Plotter::setValueTrackerAttributes( const QModelIndex & index,
                                             const ValueTrackerAttributes & va )
{
    d->attributesModel->setData( d->attributesModel->mapFromSource(index),
                                 qVariantFromValue( va ),
                                 KDChart::ValueTrackerAttributesRole );
    emit propertiesChanged();
}

ValueTrackerAttributes Plotter::valueTrackerAttributes(
        const QModelIndex & index ) const
{
    return qVariantValue<ValueTrackerAttributes>( d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            KDChart::ValueTrackerAttributesRole ) );
}

void Plotter::resizeEvent ( QResizeEvent* )
{
}

const QPair< QPointF, QPointF > Plotter::calculateDataBoundaries() const
{
    if ( !checkInvariants( true ) ) 
        return QPair< QPointF, QPointF >( QPointF( 0, 0 ), QPointF( 0, 0 ) );

    // note: calculateDataBoundaries() is ignoring the hidden flags.
    //       That's not a bug but a feature: Hiding data does not mean removing them.
    // For totally removing data from KD Chart's view people can use e.g. a proxy model ...

    // calculate boundaries for different line types Normal - Stacked - Percent - Default Normal
    return d->implementor->calculateDataBoundaries();
}


void Plotter::paintEvent ( QPaintEvent*)
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle ( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}

/*double Plotter::valueForCellTesting( int row, int column,
                                         bool& bOK,
                                         bool showHiddenCellsAsInvalid ) const
{
    double value;
    if( showHiddenCellsAsInvalid && isHidden( model()->index( row, column, rootIndex() ) ) )
        bOK = false;
    else
        value = d->attributesModel->data(
                    d->attributesModel->index( row, column, attributesModelRootIndex() )
                ).toDouble( &bOK );
    return bOK ? value : 0.0;
}*/
/*
LineAttributes::MissingValuesPolicy Plotter::getCellValues(
      int row, int column,
      bool shiftCountedXValuesByHalfSection,
      double& valueX, double& valueY ) const
{
    LineAttributes::MissingValuesPolicy policy;

    bool bOK = true;
    valueX = ( datasetDimension() > 1 && column > 0 )
             ? valueForCellTesting( row, column-1, bOK, true )
             : ((shiftCountedXValuesByHalfSection ? 0.5 : 0.0) + row);
    if( bOK )
        valueY = valueForCellTesting( row, column, bOK, true );
    if( bOK ){
        policy = LineAttributes::MissingValuesPolicyIgnored;
    }else{
        // missing value: find out the policy
        QModelIndex index = model()->index( row, column, rootIndex() );
        LineAttributes la = lineAttributes( index );
        policy = la.missingValuesPolicy();
    }
    return policy;
}*/

void Plotter::paint( PaintContext* ctx )
{
    // note: Not having any data model assigned is no bug
    //       but we can not draw a diagram then either.
    if ( !checkInvariants( true ) ) return;
    if ( !AbstractGrid::isBoundariesValid(dataBoundaries()) ) return;
    const PainterSaver p( ctx->painter() );
    if( model()->rowCount() == 0 || model()->columnCount() == 0 )
        return; // nothing to paint for us

    AbstractCoordinatePlane* const plane = ctx->coordinatePlane();
    ctx->setCoordinatePlane( plane->sharedAxisMasterPlane( ctx->painter() ) );


    // paint different line types Normal - Stacked - Percent - Default Normal
    d->implementor->paint( ctx );

    ctx->setCoordinatePlane( plane );
}

void Plotter::resize ( const QSizeF& size )
{
    d->compressor.setResolution( static_cast<int>( size.width() ),
                                 static_cast<int>( size.height() ) );
    setDataBoundariesDirty();
}

const int Plotter::numberOfAbscissaSegments () const
{
    return d->attributesModel->rowCount( attributesModelRootIndex() );
}

const int Plotter::numberOfOrdinateSegments () const
{
    return d->attributesModel->columnCount( attributesModelRootIndex() );
}
