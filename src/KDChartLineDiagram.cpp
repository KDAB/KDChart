/****************************************************************************
 ** Copyright (C) 2006 Klaravdalens Datakonsult AB.  All rights reserved.
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

#include "KDChartLineDiagram.h"
#include "KDChartBarDiagram.h"
#include "KDChartPalette.h"
#include "KDChartPosition.h"
#include "KDChartTextAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartAbstractGrid.h"
#include "KDChartDataValueAttributes.h"

#include <KDABLibFakes>

#include "KDChartLineDiagram_p.h"
#include "KDChartNormalLineDiagram_p.h"
#include "KDChartStackedLineDiagram_p.h"
#include "KDChartPercentLineDiagram_p.h"

using namespace KDChart;

LineDiagram::Private::Private()
  :lineType ( Normal )
{
}

LineDiagram::Private::~Private() {}


#define d d_func()


LineDiagram::LineDiagram( QWidget* parent, CartesianCoordinatePlane* plane ) :
    AbstractCartesianDiagram( new Private(), parent, plane )
{
    init();
}

void LineDiagram::init()
{
    d->diagram = this;
    d->normalDiagram = new NormalLineDiagram;
    d->stackedDiagram = new StackedLineDiagram;
    d->percentDiagram = new PercentLineDiagram;
}

LineDiagram::~LineDiagram()
{
}

LineDiagram * LineDiagram::clone() const
{
    return new LineDiagram( new Private( *d ) );
}


bool LineDiagram::compare( const LineDiagram* other )const
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
            ( static_cast<const AbstractCartesianDiagram*>(this)->compare( other ) ) &&
            // compare own properties
            (type() == other->type());
}


void LineDiagram::setType( const LineType type )
{
   if ( d->lineType == type ) return;
   if ( type != LineDiagram::Normal && datasetDimension() > 1 ) {
       Q_ASSERT_X ( false, "setType()",
                    "This line chart type can't be used with multi-dimensional data." );
       return;
   }
   d->lineType = type;
   // AbstractAxis settings - see AbstractDiagram and CartesianAxis
   setPercentMode( type == LineDiagram::Percent );
   setDataBoundariesDirty();
   emit layoutChanged( this );
   emit propertiesChanged();
}

LineDiagram::LineType LineDiagram::type() const
{
   return d->lineType;
}

void LineDiagram::setLineAttributes( const LineAttributes & ta )
{
    d->attributesModel->setModelData(
        qVariantFromValue( ta ),
        LineAttributesRole );
    emit propertiesChanged();
}

void LineDiagram::setLineAttributes(
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

void LineDiagram::resetLineAttributes( int column )
{
    d->attributesModel->resetHeaderData(
            column, Qt::Vertical, LineAttributesRole );
    emit propertiesChanged();
}

void LineDiagram::setLineAttributes(
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
void LineDiagram::resetLineAttributes( const QModelIndex & index )
{
    d->attributesModel->resetData(
            d->attributesModel->mapFromSource(index), LineAttributesRole );
    emit propertiesChanged();
}

LineAttributes LineDiagram::lineAttributes() const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data( KDChart::LineAttributesRole ) );
}

LineAttributes LineDiagram::lineAttributes( int column ) const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::LineAttributesRole ) );
}

LineAttributes LineDiagram::lineAttributes(
    const QModelIndex & index ) const
{
    return qVariantValue<LineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource(index),
            KDChart::LineAttributesRole ) );
}

void LineDiagram::setThreeDLineAttributes(
    const ThreeDLineAttributes & ta )
{
    setDataBoundariesDirty();
    d->attributesModel->setModelData(
        qVariantFromValue( ta ),
        ThreeDLineAttributesRole );
   emit propertiesChanged();
}

void LineDiagram::setThreeDLineAttributes(
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

void LineDiagram::setThreeDLineAttributes(
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

ThreeDLineAttributes LineDiagram::threeDLineAttributes() const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data( KDChart::ThreeDLineAttributesRole ) );
}

ThreeDLineAttributes LineDiagram::threeDLineAttributes( int column ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( columnToIndex( column ) ),
            KDChart::ThreeDLineAttributesRole ) );
}

ThreeDLineAttributes LineDiagram::threeDLineAttributes(
    const QModelIndex & index ) const
{
    return qVariantValue<ThreeDLineAttributes>(
        d->attributesModel->data(
            d->attributesModel->mapFromSource( index ),
            KDChart::ThreeDLineAttributesRole ) );
}

double LineDiagram::threeDItemDepth( const QModelIndex & index ) const
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
    // FIXME remove switch, use implementor pointer
    switch ( type() ) {
    case LineDiagram::Normal:
        return d->normalDiagram->calculateDataBoundaries();
        break;
    case LineDiagram::Stacked:
        return d->stackedDiagram->calculateDataBoundaries();
        break;
    case LineDiagram::Percent:
        return d->percentDiagram->calculateDataBoundaries();
    break;
    default:
        Q_ASSERT_X ( false, "calculateDataBoundaries()",
                     "Type item does not match a defined line chart Type." );
        return QPair<QPointF, QPointF>();
    }
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

/*commenting this include: used for testing
notice: Windows compilers need this include to
be written before the #define d(d_func()) above*/
//#include <QTime>

void LineDiagram::paint( PaintContext* ctx )
{
//qDebug() << "    start diag::paint()";
    // note: Not having any data model assigned is no bug
    //       but we can not draw a diagram then either.
    if ( !checkInvariants( true ) ) return;
    if ( !AbstractGrid::isBoundariesValid(dataBoundaries()) ) return;

    const PainterSaver p( ctx->painter() );
    const CartesianCoordinatePlane* plane = dynamic_cast< const CartesianCoordinatePlane* >(
        coordinatePlane()->sharedAxisMasterPlane( ctx->painter() ) );

    // Make sure counted x values (== in diagrams with 1-dimensional data cells)
    // get shifted by 0.5, if the diagram's reference diagram is a BarDiagram.
    // So we get the lines to start/end at the middle of the respective bar groups.
    const bool shiftCountedXValuesByHalfSection =
        (dynamic_cast< BarDiagram* >( referenceDiagram() ) != 0);

    //QTime t = QTime::currentTime();

    const QPair<QPointF, QPointF> boundaries = dataBoundaries();
    const QPointF bottomLeft = boundaries.first;
    const QPointF topRight = boundaries.second;

    int maxFound = 0;
    {   // find the last column number that is not hidden
        const int columnCount = d->attributesModel->columnCount(attributesModelRootIndex());
        for( int iColumn =  datasetDimension()-1;
             iColumn <  columnCount;
             iColumn += datasetDimension() )
            if( ! isHidden( iColumn ) )
                maxFound = iColumn;
    }
    const int lastVisibleColumn = maxFound;
    const int rowCount = d->attributesModel->rowCount(attributesModelRootIndex());

    DataValueTextInfoList list;
    LineAttributesInfoList lineList;
    LineAttributes::MissingValuesPolicy policy;

    if( model()->rowCount() == 0 || model()->columnCount() == 0 )
        return; // nothing to paint for us

    // paint different line types Normal - Stacked - Percent - Default Normal
    switch ( type() )
    {
    case LineDiagram::Normal:
    {
        // temp: finish refactoring
        return d->normalDiagram->paint( ctx );
        break;
    case LineDiagram::Stacked:
        // fall-through intended
        return d->stackedDiagram->paint( ctx );
        break;
    case LineDiagram::Percent:
        return d->percentDiagram->paint( ctx );
        break;
    default:
        Q_ASSERT_X ( false, "paint()",
                     "Type item does not match a defined line chart Type." );
    }
    // paint all lines and their attributes
    {
        PainterSaver painterSaver( ctx->painter() );
        if ( antiAliasing() )
            ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        LineAttributesInfoListIterator itline ( lineList );

        //qDebug() << "Rendering 1 in: " << t.msecsTo( QTime::currentTime() ) << endl;

        QBrush curBrush;
        QPen curPen;
        QPolygonF points;
        while ( itline.hasNext() ) {
            const LineAttributesInfo& lineInfo = itline.next();
            const QModelIndex& index = lineInfo.index;
            const ThreeDLineAttributes td = threeDLineAttributes( index );
            if( td.isEnabled() ){
                d->paintThreeDLines( this, ctx, index, lineInfo.value, lineInfo.nextValue, td.depth() );
            }else{
                const QBrush br( brush( index ) );
                const QPen   pn( pen(   index ) );
                if( points.count() && points.last() == lineInfo.value && curBrush == br && curPen == pn ){
                    points << lineInfo.nextValue;
                }else{
                    if( points.count() )
                        d->paintPolyline( ctx, curBrush, curPen, points );
                    curBrush = br;
                    curPen   = pn;
                    points.clear();
                    points << lineInfo.value << lineInfo.nextValue;
                }
            }
        }
        if( points.count() )
            d->paintPolyline( ctx, curBrush, curPen, points );
    }
    // paint all data value texts and the point markers
    d->paintDataValueTextsAndMarkers( this, ctx, list, true );
    //qDebug() << "Rendering 2 in: " << t.msecsTo( QTime::currentTime() ) << endl;
    }
}

/* old:
void LineDiagram::paintLines( PaintContext* ctx, const QModelIndex& index,
                              const QPointF& from, const QPointF& to )
{
    ThreeDLineAttributes td = threeDLineAttributes(index);
    if ( td.isEnabled() ) {
        const double lineDepth = td.depth();
        d->paintThreeDLines( ctx, index, from, to, lineDepth );
    } else {
        ctx->painter()->setBrush( brush( index ) );
        ctx->painter()->setPen( pen( index ) );
        if ( index.row() + 1 < d->attributesModel->rowCount(attributesModelRootIndex()) ) {
            if ( ctx->rectangle().contains( from ) || ctx->rectangle().contains( to ) ) {
                ctx->painter()->drawLine( from, to );
            }
        }
    }
}
*/

void LineDiagram::resize ( const QSizeF& )
{
}

const int LineDiagram::numberOfAbscissaSegments () const
{
    return d->attributesModel->rowCount(attributesModelRootIndex());
}

const int LineDiagram::numberOfOrdinateSegments () const
{
    return d->attributesModel->columnCount(attributesModelRootIndex());
}

//#endif


