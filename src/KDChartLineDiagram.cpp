/****************************************************************************
 ** Copyright (C) 2006 Klarävdalens Datakonsult AB.  All rights reserved.
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

#include <math.h>

#include <QtXml/QDomDocumentFragment>
#include <QDebug>
#include <QPainter>
#include <QString>
#include <QPen>
#include <QVector>

#include "KDChartLineDiagram.h"
#include "KDChartLineDiagram_p.h"
#include "KDChartPaintContext.h"
#include "KDChartPalette.h"
#include "KDChartTextAttributes.h"
#include "KDChartThreeDLineAttributes.h"
#include "KDChartAttributesModel.h"
#include "KDChartPainterSaver_p.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


using namespace KDChart;

LineDiagram::Private::Private()
  :lineType ( Normal )
{
}

LineDiagram::Private::~Private() {}


#define d d_func()


LineDiagram::LineDiagram( CartesianCoordinatePlane* parent ) :
    AbstractCartesianDiagram( new Private(), parent )
{
    init();
}

void LineDiagram::init()
{
}

LineDiagram::~LineDiagram()
{
}

LineDiagram * LineDiagram::clone() const
{
    // PENDING(kalle) FIXME
    qWarning( "Sorry, not implemented: LineDiagram * LineDiagram::clone() const" );
    return (LineDiagram*)0xdeadbeef;
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
   emit layoutChanged( this );
}

LineDiagram::LineType LineDiagram::type() const
{
   return d->lineType;
}

void LineDiagram::setLineAttributes( const LineAttributes & ta )
{
     d->attributesModel->setModelData( qVariantFromValue( ta ), LineAttributesRole );
}

void LineDiagram::setLineAttributes( int column, const LineAttributes & ta )
{
    d->attributesModel->setHeaderData( column, Qt::Vertical, qVariantFromValue( ta ), LineAttributesRole );
}

void LineDiagram::setLineAttributes( const QModelIndex & index, const LineAttributes & ta )
{
    d->attributesModel->setData( d->attributesModel->mapFromSource(index), qVariantFromValue( ta ), LineAttributesRole );
}

LineAttributes LineDiagram::lineAttributes( const QModelIndex & index ) const
{
    return qVariantValue<LineAttributes>( d->attributesModel->data( d->attributesModel->mapFromSource(index), KDChart::LineAttributesRole ) );
}

void LineDiagram::setThreeDLineAttributes( const ThreeDLineAttributes & ta )
{
    d->attributesModel->setModelData( qVariantFromValue( ta ), ThreeDLineAttributesRole );
}

void LineDiagram::setThreeDLineAttributes( int column, const ThreeDLineAttributes & ta )
{
    d->attributesModel->setHeaderData( column, Qt::Vertical, qVariantFromValue( ta ), ThreeDLineAttributesRole );
}

void LineDiagram::setThreeDLineAttributes( const QModelIndex & index, const ThreeDLineAttributes & ta )
{
    d->attributesModel->setData( d->attributesModel->mapFromSource(index),
			      qVariantFromValue( ta ), ThreeDLineAttributesRole );
}

ThreeDLineAttributes LineDiagram::threeDLineAttributes( const QModelIndex & index ) const
{
    return qVariantValue<ThreeDLineAttributes>( d->attributesModel->data( d->attributesModel->mapFromSource(index),
								       KDChart::ThreeDLineAttributesRole ) );
}

void LineDiagram::resizeEvent ( QResizeEvent* )
{
}

const QPair<QPointF, QPointF> LineDiagram::dataBoundaries () const
{
    if ( !checkInvariants( true ) ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );

    const int rowCount = d->attributesModel->rowCount(attributesModelRootIndex());
    const int colCount = d->attributesModel->columnCount(attributesModelRootIndex());
    double xMin = 0;
    double xMax = rowCount -1;
    double yMin = 0, yMax = 0;

    // calculate boundaries for  different line types Normal - Stacked - Percent - Default Normal
    switch ( type() )
        {
        case LineDiagram::Normal:
            for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                for ( int j=0; j< rowCount; ++j ) {
                    double value = valueForCell( j, i );
                    yMin = qMin( yMin, value );
                    yMax = qMax( yMax, value );
                    if ( datasetDimension() > 1 ) {
                        double xvalue = valueForCell( j, i-1);
                        xMin = qMin( xMin, xvalue );
                        xMax = qMax( xMax, xvalue );
                    }
                }
            }
            break;
        case LineDiagram::Stacked:
                for ( int j=0; j< rowCount; ++j ) {
                    // calculate sum of values per column - Find out stacked Min/Max
                    double stackedValues = 0;
                    for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                        stackedValues +=  valueForCell( j, i );
                        yMin = qMin( yMin, stackedValues );
                        yMax = qMax( yMax, stackedValues );
                    }
                }
            break;
        case LineDiagram::Percent:
            for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                for ( int j=0; j< rowCount; ++j ) {
                    // Ordinate should begin at 0 the max value being the 100% pos
                    yMax = qMax( yMax, valueForCell( j, i ) );
                }
            }
            break;
        default:
            Q_ASSERT_X ( false, "dataBoundaries()",
                         "Type item does not match a defined line chart Type." );
        }

    QPointF bottomLeft ( QPointF( xMin, yMin ) );
    QPointF topRight ( QPointF( xMax, yMax ) );

    QPointF bottomLeftThreeD, topRightThreeD;
    bool threeDBoundaries = false;
    // ThreeD boundaries
    for ( int i=0; i<colCount; ++i ) {
        QModelIndex index = model()->index( 0, i, rootIndex() );
        const ThreeDLineAttributes &tda = threeDLineAttributes( index );
        if ( tda.isEnabled() ) {
            threeDBoundaries = true;
            QPointF projLeft ( project(QPointF( xMin, yMin ), QPointF( xMin, yMin), tda.depth()/10 , index ) );
            QPointF projRight( project(QPointF( xMax, yMax ), QPointF( xMax, yMax), tda.depth()/10, index ) );
            projRight.x() > topRight.x()  ? topRightThreeD.setX( projRight.x() ):
                topRightThreeD.setX( topRight.x() );
            projRight.y() > topRight.y()  ? topRightThreeD.setY( projRight.y() ):
                topRightThreeD.setY( topRight.y() );
            projLeft.x() > bottomLeft.x() ? bottomLeftThreeD.setX( bottomLeft.x() ):
                bottomLeftThreeD.setX( projLeft.x() );
            projLeft.y() > bottomLeft.y() ? bottomLeftThreeD.setY( bottomLeft.y() ):
                bottomLeftThreeD.setY( projLeft.y() );
        }
    }
    return threeDBoundaries ? QPair<QPointF, QPointF> ( bottomLeftThreeD,  topRightThreeD ):
                              QPair<QPointF, QPointF> ( bottomLeft, topRight );
}


void LineDiagram::paintEvent ( QPaintEvent*)
{
    QPainter painter ( viewport() );
    PaintContext ctx;
    ctx.setPainter ( &painter );
    ctx.setRectangle ( QRectF ( 0, 0, width(), height() ) );
    paint ( &ctx );
}


void LineDiagram::paint( PaintContext* ctx )
{
    if ( !checkInvariants( true ) ) return;
    if ( !isBoundariesValid(dataBoundaries()) ) return;

    // draw the grid
    drawGrid( ctx );

    //calculates and stores the values
    const int rowCount = d->attributesModel->rowCount(attributesModelRootIndex());
    const int colCount = d->attributesModel->columnCount(attributesModelRootIndex());
    DataValueTextInfoList list;
    LineAttributesInfoList lineList;

    // paint different line types Normal - Stacked - Percent - Default Normal
    switch ( type() )
    {
        case LineDiagram::Normal:
            for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                QPolygonF area;
                for ( int j=0; j< rowCount; ++j ) {
                    QModelIndex index = model()->index( j, i, rootIndex() );
                    double value = valueForCell( j, i );
                    double xvalue = datasetDimension() == 1 ? j : valueForCell(j, i-1);
                    QPointF fromPoint = coordinatePlane()->translate( QPointF( xvalue, value ) );
                    area.append( fromPoint );
                    if ( j+1 < rowCount ) {
                        double nextValue = valueForCell( j+1, i);
                        double nextXValue = datasetDimension() == 1 ? j+1 : valueForCell( j+1, i-1 );
                        QPointF toPoint = coordinatePlane()->translate( QPointF( nextXValue, nextValue ) );
                        lineList.append( LineAttributesInfo( index, fromPoint, toPoint ) );
                    }
                    list.append( DataValueTextInfo( index, fromPoint, value ) );
                }
                //area can be set by column
                QModelIndex index = model()->index( 0, i, rootIndex() );
                LineAttributes laa = lineAttributes( index );
                if ( laa.displayArea() )
                    paintAreas( ctx, index, area, laa.transparency() );
            }
            break;
        case LineDiagram::Stacked:
            for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                QPolygonF area;
                for ( int j = 0; j< rowCount; ++j ) {
                    QModelIndex index = model()->index( j, i, rootIndex() );
                    double stackedValues = 0, nextValues = 0;
                    for ( int k = i; k >= datasetDimension()-1 ; k -= datasetDimension() ) {
                        stackedValues += valueForCell( j, k );
                        if ( j+1 < rowCount )
                            nextValues += valueForCell( j+1, k );
                    }
                    QPointF nextPoint = coordinatePlane()->translate( QPointF( j, stackedValues ) );
                    area.append( nextPoint );
                    QPointF toPoint = coordinatePlane()->translate( QPointF( j+1, nextValues ) );
                    if ( j+1 < rowCount )
                        lineList.append( LineAttributesInfo( index, nextPoint, toPoint ) );
                    list.append( DataValueTextInfo( index, nextPoint, valueForCell( j, i ) ) );
                }
                QModelIndex index = model()->index( 0, i, rootIndex() );
                LineAttributes laa = lineAttributes( index );
                if ( laa.displayArea() )
                    paintAreas( ctx, index, area, laa.transparency() );
            }
            break;
        case LineDiagram::Percent:
        {
            double maxValue = 0;
            double sumValues = 0;
            QVector <double > sumValuesVector;
            // search for ordinate max value or 100 %
            for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                for ( int j=0; j< rowCount; ++j )
                    maxValue = qMax( maxValue, valueForCell( j, i ) );
            }
            //calculate sum of values for each column and store
            for ( int j=0; j<rowCount ; ++j ) {
                for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                    double tmpValue = valueForCell( j, i );
                    if ( tmpValue > 0 )
                        sumValues += tmpValue;
                    if ( i == colCount-1 ) {
                        sumValuesVector <<  sumValues ;
                        sumValues = 0;
                    }
                }
            }
            // calculate stacked percent value
            for( int i = datasetDimension()-1; i < colCount; i += datasetDimension() ) {
                QPolygonF area;
                for ( int j=0; j<rowCount ; ++j ) {
                    double stackedValues = 0, nextValues = 0;
                    QPointF nextPoint;
		    QModelIndex index = model()->index( j, i, rootIndex() );
                    //calculate stacked percent value- we only take in account positives values for now.
                    for ( int k = i; k >= 0 ; --k ) {
                        double val = valueForCell( j, k );
                        if ( val > 0)
                            stackedValues += val;
                        if ( j+1 < rowCount ){
                            val = valueForCell( j+1, k);
                            if ( val > 0 )
                                nextValues += val;
                        }
                    }
                    nextPoint = coordinatePlane()->translate(
                            QPointF( j, stackedValues/sumValuesVector.at(j)* maxValue ) );
                    area.append( nextPoint );
                    if ( j+1 < rowCount ) {
                        QPointF toPoint = coordinatePlane()->translate(
                                QPointF( j+1, nextValues/sumValuesVector.at(j+1)* maxValue ) );
                        lineList.append( LineAttributesInfo( index, nextPoint, toPoint ) );
                    }
                    list.append( DataValueTextInfo( index, nextPoint, valueForCell( j, i ) ) );
                }
                QModelIndex index = model()->index( 0, i, rootIndex() );
                LineAttributes laa = lineAttributes( index );
                if ( laa.displayArea() )
                    paintAreas( ctx, index, area, laa.transparency() );
            }
            // AbstractAxis settings - see AbstractDiagram and CartesianAxis
	    setPercentMode( true );
            break;
        }
        default:
            Q_ASSERT_X ( false, "paint()",
                         "Type item does not match a defined line chart Type." );
    }
    // paint all lines and their attributes
    LineAttributesInfoListIterator itline ( lineList );
    while ( itline.hasNext() ) {
        const LineAttributesInfo& lineInfo = itline.next();
        paintLines( ctx,lineInfo.index, lineInfo.value, lineInfo.nextValue );
    }
    DataValueTextInfoListIterator it( list );
    while ( it.hasNext() ) {
        const DataValueTextInfo& info = it.next();
        paintDataValueText( ctx->painter(), info.index, info.pos, info.value );
        paintMarker( ctx->painter(), info.index, info.pos );
    }
}

void LineDiagram::paintLines( PaintContext* ctx, const QModelIndex& index,
                              const QPointF& from, const QPointF& to )
{
    ThreeDLineAttributes td = threeDLineAttributes(index);
    if ( td.isEnabled() ) {
        const double lineDepth = td.depth();
        paintThreeDLines( ctx, index, from.y(), to.y(), lineDepth );
    }    else {
        //get the brush and pen to be used from the AbstractDiagram methods
        QBrush indexBrush ( brush( index ) );
        PainterSaver painterSaver( ctx->painter() );
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( indexBrush );
        ctx->painter()->setPen( pen( index ) );
        if ( index.row() + 1 < d->attributesModel->rowCount(attributesModelRootIndex()) ) {
            ctx->painter()->drawLine( from, to );
        }
    }
}

void LineDiagram::paintAreas( PaintContext* ctx, const QModelIndex& index, const QPolygonF& area, const uint transparency )
{
    QPolygonF pol ( area );
    const QPair<QPointF, QPointF> boundaries = dataBoundaries ();
    QPointF bottomLeft = boundaries.first;
    QPointF topRight = boundaries.second;
    QColor trans( brush(index).color() );
    QPen indexPen( pen(index) );
    trans.setAlpha( transparency );
    indexPen.setColor( trans );
    pol.insert( 0,  coordinatePlane()->translate( QPointF( bottomLeft.x(), 0 ) ) );
    pol.append( coordinatePlane()->translate( QPointF( topRight.x(),0 ) ) );
    PainterSaver painterSaver( ctx->painter() );
    ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setPen( indexPen );
    ctx->painter()->setBrush( trans ) ;
    ctx->painter()->drawPolygon( pol );
}

/*!
  Projects a point in a space defined by its x, y, and z coordinates
  into a point onto a plane, given two rotation angles around the x
  resp. y axis.
*/

#define DEGTORAD(d) (d)*M_PI/180.0
const QPointF LineDiagram::project( QPointF point, QPointF maxLimits, double z, const QModelIndex& index ) const
{
    Q_UNUSED( maxLimits );

    ThreeDLineAttributes td = threeDLineAttributes( index );

    //Pending Michel FIXME - the rotation does not work as expected atm
    double xrad = DEGTORAD( td.lineXRotation() );
    double yrad = DEGTORAD( td.lineYRotation() );
    QPointF ret = QPointF(point.x()*cos( yrad ) + z * sin( yrad ) ,  point.y()*cos( xrad ) - z * sin( xrad ) );
    return ret;
}

void LineDiagram::paintThreeDLines(PaintContext* ctx, const QModelIndex& index, double from, double to, double depth  )
{
    // retrieve the boundaries
    const QPair<QPointF, QPointF> boundaries = dataBoundaries ();
    QPointF maxLimits = boundaries.second;
    QVector <QPointF > segmentPoints;
    QPointF bottomLeft = QPointF( index.row(), from);
    QPointF topLeft = project( QPointF( index.row(), from), maxLimits, depth/10, index  );
    QPointF bottomRight = QPointF( index.row()+1, to);
    QPointF topRight = project (QPointF( index.row()+1, to), maxLimits, depth/10, index  );
    segmentPoints << coordinatePlane()->translate(bottomLeft)
                  << coordinatePlane()->translate(bottomRight)
                  << coordinatePlane()->translate(topRight)
                  << coordinatePlane()->translate(topLeft);
    QPolygonF segment ( segmentPoints );
    QBrush indexBrush ( brush( index ) );
    PainterSaver painterSaver( ctx->painter() );
    ctx->painter()->setRenderHint ( QPainter::Antialiasing );
    ctx->painter()->setBrush( indexBrush );
    ctx->painter()->setPen( pen( index ) ) ;
    ctx->painter()->drawPolygon( segment );
}

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


