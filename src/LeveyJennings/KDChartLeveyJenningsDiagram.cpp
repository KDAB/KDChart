/****************************************************************************
 ** Copyright (C) 2007 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#include <QDebug>
#include <QPainter>
#include <QString>
#include <QPainterPath>
#include <QPen>
#include <QVector>

#include "KDChartLeveyJenningsDiagram.h"
#include "KDChartBarDiagram.h"
#include "KDChartPalette.h"
#include "KDChartPosition.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractGrid.h"
#include "KDChartDataValueAttributes.h"

#include <KDABLibFakes>

#include "KDChartLeveyJenningsDiagram_p.h"

using namespace KDChart;

LeveyJenningsDiagram::Private::Private()
{
}

LeveyJenningsDiagram::Private::~Private() {}


#define d d_func()


LeveyJenningsDiagram::LeveyJenningsDiagram( QWidget* parent, LeveyJenningsCoordinatePlane* plane ) :
    LineDiagram( new Private(), parent, plane )
{
    init();
}

void LeveyJenningsDiagram::init()
{
    d->lotChangedPosition = Qt::AlignTop;
    d->diagram = this;
}

LeveyJenningsDiagram::~LeveyJenningsDiagram()
{
}

/**
  * Creates an exact copy of this diagram.
  */
LineDiagram * LeveyJenningsDiagram::clone() const
{
    LeveyJenningsDiagram* newDiagram = new LeveyJenningsDiagram( new Private( *d ) );
    return newDiagram;
}

bool LeveyJenningsDiagram::compare( const LeveyJenningsDiagram* other )const
{
    if( other == this ) return true;
    if( ! other ){
        return false;
    }
    /*
    qDebug() <<"\n             LineDiagram::compare():";
            // compare own properties
    qDebug() << (type() == other->type());
    */
    return  // compare the base class
            ( static_cast<const LineDiagram*>(this)->compare( other ) );
}

void LeveyJenningsDiagram::setLotChangedSymbolPosition( Qt::Alignment pos )
{
    d->lotChangedPosition = pos;
}

Qt::Alignment LeveyJenningsDiagram::lotChangedSymbolPosition() const
{
    return d->lotChangedPosition;
}

void LeveyJenningsDiagram::setExpectedMeanValue( float meanValue )
{
    d->expectedMeanValue = meanValue;
    d->setYAxisRange();
}

float LeveyJenningsDiagram::expectedMeanValue() const
{
    return d->expectedMeanValue;
}

void LeveyJenningsDiagram::setExpectedStandardDeviation( float sd )
{
    d->expectedStandardDeviation = sd;
    d->setYAxisRange();
}

float LeveyJenningsDiagram::expectedStandardDeviation() const
{
    return d->expectedStandardDeviation;
}

float LeveyJenningsDiagram::calculatedMeanValue() const
{
    return d->calculatedMeanValue;
}

float LeveyJenningsDiagram::calculatedStandardDeviation() const
{
    return d->calculatedStandardDeviation;
}

void LeveyJenningsDiagram::setModel( QAbstractItemModel* model )
{
    if( this->model() != 0 )
    {
        disconnect( this->model(), SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
        disconnect( this->model(), SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
        disconnect( this->model(), SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
        disconnect( this->model(), SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
        disconnect( this->model(), SIGNAL( columnsInserted( const QModelIndex&, int, int ) ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
        disconnect( this->model(), SIGNAL( columnsRemoved( const QModelIndex&, int, int ) ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
        disconnect( this->model(), SIGNAL( columnsAboutToBeRemoved( const QModelIndex&, int, int ) ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
        disconnect( this->model(), SIGNAL( modelReset() ),
                                   this, SLOT( calculateMeanAndStandardDeviation() ) );
    }
    LineDiagram::setModel( model ); 
    if( this->model() != 0 )
    {
        connect( this->model(), SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );
        connect( this->model(), SIGNAL( rowsInserted( const QModelIndex&, int, int ) ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );
        connect( this->model(), SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );
        connect( this->model(), SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );
        connect( this->model(), SIGNAL( columnsInserted( const QModelIndex&, int, int ) ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );
        connect( this->model(), SIGNAL( columnsRemoved( const QModelIndex&, int, int ) ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );
        connect( this->model(), SIGNAL( columnsAboutToBeRemoved( const QModelIndex&, int, int ) ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );
        connect( this->model(), SIGNAL( modelReset() ),
                                this, SLOT( calculateMeanAndStandardDeviation() ) );

        calculateMeanAndStandardDeviation();
    }
}

// TODO: This is the 'easy' solution
// eruate, wheter this is enough or we need some better one or even boost here
void LeveyJenningsDiagram::calculateMeanAndStandardDeviation() const
{
    QVector< double > values;
    // first fetch all values
    const QAbstractItemModel& m = *model();
    const int rowCount = m.rowCount( rootIndex() );

    for( int row = 0; row < rowCount; ++row )
    {
        const double value = m.data( m.index( row, 1, rootIndex() ) ).toDouble();
        if( static_cast< int >( value ) == 0 )
            continue;
        values << value;
    }

    double sum = 0.0;
    double sumSquares = 0.0;
    KDAB_FOREACH( double value, values )
    {
        sum += value;
        sumSquares += value * value;
    }
    
    const int N = values.count();

    d->calculatedMeanValue = sum / N;
    d->calculatedStandardDeviation = sqrt( ( static_cast< double >( N ) * sumSquares - sum * sum ) / ( N * ( N - 1 ) ) );
}

/** \reimpl */
const QPair<QPointF, QPointF> LeveyJenningsDiagram::calculateDataBoundaries() const
{
    const double xMin = 0;
    const double xMax = model() ? model()->rowCount( rootIndex() ) - 1 : 0;
    const double yMin = d->expectedMeanValue - 4 * d->expectedStandardDeviation;
    const double yMax = d->expectedMeanValue + 4 * d->expectedStandardDeviation;

    const QPointF bottomLeft( QPointF( xMin, qMin( 0.0, yMin ) ) );
    const QPointF topRight( QPointF( xMax, yMax ) );

    d->setYAxisRange();

    return QPair< QPointF, QPointF >( bottomLeft, topRight );
}


#if 0
/**
  * Sets the global line attributes to \a la
  */
void LineDiagram::setLineAttributes( const LineAttributes& la )
{
    d->attributesModel->setModelData(
        qVariantFromValue( la ),
        LineAttributesRole );
    emit propertiesChanged();
}

/** 
  * Sets the line attributes of data set \a column to \a la
  */
void LineDiagram::setLineAttributes(
        int column,
    const LineAttributes& la )
{
    d->attributesModel->setHeaderData(
            column,
            Qt::Vertical,
            qVariantFromValue( la ),
            LineAttributesRole );
    emit propertiesChanged();
}

/**
  * Resets the line attributes of data set \a column
  */
void LineDiagram::resetLineAttributes( int column )
{
    d->attributesModel->resetHeaderData(
            column, Qt::Vertical, LineAttributesRole );
    emit propertiesChanged();
}

/**
  * Sets the line attributes for the model index \a index to \a la
  */
void LineDiagram::setLineAttributes(
        const QModelIndex& index,
    const LineAttributes& la )
{
    d->attributesModel->setData(
            d->attributesModel->mapFromSource(index),
    qVariantFromValue( la ),
    LineAttributesRole );
    emit propertiesChanged();
}

/**
 * Remove any explicit line attributes settings that might have been specified before.
 */
void LineDiagram::resetLineAttributes( const QModelIndex & index )
{
    d->attributesModel->resetData(
            d->attributesModel->mapFromSource(index), LineAttributesRole );
    emit propertiesChanged();
}

/**
  * @return the global line attribute set
  */
LineAttributes LineDiagram::lineAttributes() const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data( KDChart::LineAttributesRole ) );
}

/**
  * @return the line attribute set of data set \a column
  */
LineAttributes LineDiagram::lineAttributes( int column ) const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::LineAttributesRole ) );
}

/**
  * @return the line attribute set of the model index \a index
  */
LineAttributes LineDiagram::lineAttributes(
    const QModelIndex& index ) const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource(index),
            KDChart::LineAttributesRole ) );
}

/**
  * Sets the global 3D line attributes to \a la
  */
void LineDiagram::setThreeDLineAttributes(
    const ThreeDLineAttributes& la )
{
    setDataBoundariesDirty();
    d->attributesModel->setModelData(
        qVariantFromValue( la ),
        ThreeDLineAttributesRole );
   emit propertiesChanged();
}

/**
  * Sets the 3D line attributes of data set \a column to \a ta
  */
void LineDiagram::setThreeDLineAttributes(
    int column,
    const ThreeDLineAttributes& la )
{
    setDataBoundariesDirty();
    d->attributesModel->setHeaderData(
        column,
        Qt::Vertical,
        qVariantFromValue( la ),
        ThreeDLineAttributesRole );
   emit propertiesChanged();
}

/**
  * Sets the 3D line attributes of model index \a index to \a la
  */
void LineDiagram::setThreeDLineAttributes(
    const QModelIndex & index,
    const ThreeDLineAttributes& la )
{
    setDataBoundariesDirty();
    d->attributesModel->setData(
        d->attributesModel->mapFromSource(index),
        qVariantFromValue( la ),
        ThreeDLineAttributesRole );
   emit propertiesChanged();
}

/**
  * @return the global 3D line attributes
  */
ThreeDLineAttributes LineDiagram::threeDLineAttributes() const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data( KDChart::ThreeDLineAttributesRole ) );
}

/**
  * @return the 3D line attributes of data set \a column
  */
ThreeDLineAttributes LineDiagram::threeDLineAttributes( int column ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::ThreeDLineAttributesRole ) );
}

/**
  * @return the 3D line attributes of the model index \a index
  */
ThreeDLineAttributes LineDiagram::threeDLineAttributes( const QModelIndex& index ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            KDChart::ThreeDLineAttributesRole ) );
}

double LineDiagram::threeDItemDepth( const QModelIndex& index ) const
{
    return threeDLineAttributes( index ).validDepth();
}

double LineDiagram::threeDItemDepth( int column ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->headerData (
            column,
            Qt::Vertical,
            KDChart::ThreeDLineAttributesRole ) ).validDepth();
}

/**
  * Sets the value tracker attributes of the model index \a index to \a va
  */
void LineDiagram::setValueTrackerAttributes( const QModelIndex & index,
                                             const ValueTrackerAttributes & va )
{
    d->attributesModel->setData( d->attributesModel->mapFromSource(index),
                                 qVariantFromValue( va ),
                                 KDChart::ValueTrackerAttributesRole );
    emit propertiesChanged();
}

/**
  * Returns the value tracker attributes of the model index \a index
  */
ValueTrackerAttributes LineDiagram::valueTrackerAttributes(
        const QModelIndex & index ) const
{
    return qVariantValue<ValueTrackerAttributes>( d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            KDChart::ValueTrackerAttributesRole ) );
}

void LineDiagram::resizeEvent ( QResizeEvent* )
{
}

const QPair<QPointF, QPointF> LineDiagram::calculateDataBoundaries() const
{
    if ( !checkInvariants( true ) ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );

    // note: calculateDataBoundaries() is ignoring the hidden flags.
    //       That's not a bug but a feature: Hiding data does not mean removing them.
    // For totally removing data from KD Chart's view people can use e.g. a proxy model ...

    // calculate boundaries for different line types Normal - Stacked - Percent - Default Normal
    return d->implementor->calculateDataBoundaries();
}


void LineDiagram::paintEvent ( QPaintEvent*)
{
//qDebug() << "starting LineDiagram::paintEvent ( QPaintEvent*)";
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle ( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
//qDebug() << "         LineDiagram::paintEvent ( QPaintEvent*) ended.";
}


double LineDiagram::valueForCellTesting( int row, int column,
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
}

LineAttributes::MissingValuesPolicy LineDiagram::getCellValues(
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
}
#endif

void LeveyJenningsDiagram::paint( PaintContext* ctx )
{
    // note: Not having any data model assigned is no bug
    //       but we can not draw a diagram then either.
    if ( !checkInvariants( true ) ) return;
    if ( !AbstractGrid::isBoundariesValid(dataBoundaries()) ) return;

    QPainter* const painter = ctx->painter();
    const PainterSaver p( painter );
    if( model()->rowCount( rootIndex() ) == 0 || model()->columnCount( rootIndex() ) < 2 )
        return; // nothing to paint for us

    AbstractCoordinatePlane* const plane = ctx->coordinatePlane();
    ctx->setCoordinatePlane( plane->sharedAxisMasterPlane( painter ) );

    const QAbstractItemModel& m = *model();
    const int rowCount = m.rowCount( rootIndex() );

    painter->setRenderHint( QPainter::Antialiasing, true );

    int prevLot = -1;
    QPointF prevPoint;
    bool hadMissingValue = false;

    for( int row = 0; row < rowCount; ++row )
    {
        const QModelIndex lotIndex = m.index( row, 0, rootIndex() );
        const QModelIndex valueIndex = m.index( row, 1, rootIndex() );
        const QModelIndex okIndex = m.index( row, 2, rootIndex() );

        painter->setPen( pen( valueIndex ) );

        const int lot = m.data( lotIndex ).toInt();
        const double value = m.data( valueIndex ).toDouble();
        const bool ok = m.data( okIndex ).toBool();

        const QPointF point = ctx->coordinatePlane()->translate( QPointF( row, value ) );

        if( static_cast< int >( value ) == 0 )
        {
            hadMissingValue = true;
            continue;
        }

        if( prevLot == lot )
        {
            const QPen pen = painter->pen();
            QPen newPen = pen;
            
            if( hadMissingValue )
            {
                newPen.setDashPattern( QVector< qreal >() << 4.0 << 4.0 );
            }
            
            painter->setPen( newPen );
            painter->drawLine( prevPoint, point );
            painter->setPen( pen );
        }
        else if( row > 0 )
        {
            drawLotChangeSymbol( ctx, QPointF( row, value ) );
        }

        drawDataPointSymbol( ctx, QPointF( row, value ), ok );

        if( selectionModel()->currentIndex() == lotIndex )
        {
            const QPen pen = ctx->painter()->pen();
            painter->setPen( Qt::blue );
            painter->drawLine( ctx->coordinatePlane()->translate( QPointF( row, d->expectedMeanValue - 4 * d->expectedStandardDeviation ) ),
                               ctx->coordinatePlane()->translate( QPointF( row, d->expectedMeanValue + 4 * d->expectedStandardDeviation ) ) );
            painter->setPen( pen );
        }

        prevLot = lot;
        prevPoint = point;
        hadMissingValue = false;
    }

    ctx->setCoordinatePlane( plane );
}

void LeveyJenningsDiagram::drawDataPointSymbol( PaintContext* ctx, const QPointF& pos, bool ok )
{
    // TODO: This has to be a SVG image
    QPainter* const painter = ctx->painter();
    const PainterSaver ps( painter );
    const QPointF transPos = ctx->coordinatePlane()->translate( pos ).toPoint();
    const QRectF rect( transPos.x() - 6, transPos.y() - 6 , 11, 11 );
    painter->drawEllipse( rect );
    if( ok )
        return;
    painter->setBrush( QBrush( Qt::red ) );
    painter->setPen( Qt::transparent );
    const QRectF dotRect( transPos.x() - 4, transPos.y() - 4, 7, 7 );
    painter->drawEllipse( dotRect );
}

void LeveyJenningsDiagram::drawLotChangeSymbol( PaintContext* ctx, const QPointF& pos )
{
    // TODO: This has to be a SVG image
    const QPointF transPos = ctx->coordinatePlane()->translate( QPointF( pos.x(), 
                                                                d->lotChangedPosition & Qt::AlignTop ? d->expectedMeanValue + 4 * d->expectedStandardDeviation
                                                                                                     : d->expectedMeanValue - 4 * d->expectedStandardDeviation ) );
    QPainter* const painter = ctx->painter();
    const PainterSaver ps( painter );
    painter->setClipping( false );
    painter->translate( transPos );
    painter->rotate( 45 );
    painter->setPen( Qt::white );
    painter->setBrush( QBrush( Qt::black ) );
    const QRectF rect( -5, -5, 9, 9 );
    painter->drawRect( rect );
}
