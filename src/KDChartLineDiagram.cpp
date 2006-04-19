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
   d->lineType = type;
   emit layoutChanged( this );
}

LineDiagram::LineType LineDiagram::type() const
{
   return d->lineType;
}

void LineDiagram::setLineAttributes( const LineAttributes & ta )
{
     d->attributesModel->setModelData( QVariant::fromValue( ta ), LineAttributesRole );
}

void LineDiagram::setLineAttributes( int column, const LineAttributes & ta )
{
    d->attributesModel->setHeaderData( column, Qt::Vertical, QVariant::fromValue( ta ), LineAttributesRole );
}

void LineDiagram::setLineAttributes( const QModelIndex & index, const LineAttributes & ta )
{
    model()->setData( index, QVariant::fromValue( ta ), LineAttributesRole );
}

LineAttributes LineDiagram::lineAttributes( const QModelIndex & index ) const
{
    return qVariantValue<LineAttributes>( model()->data( index, KDChart::LineAttributesRole ) );
}

void LineDiagram::setThreeDLineAttributes( const ThreeDLineAttributes & ta )
{
  d->attributesModel->setModelData( QVariant::fromValue( ta ), ThreeDLineAttributesRole );
}

void LineDiagram::setThreeDLineAttributes( int column, const ThreeDLineAttributes & ta )
{
    d->attributesModel->setHeaderData( column, Qt::Vertical, QVariant::fromValue( ta ), ThreeDLineAttributesRole );
}

void LineDiagram::setThreeDLineAttributes( const QModelIndex & index, const ThreeDLineAttributes & ta )
{
    model()->setData( index, QVariant::fromValue( ta ), ThreeDLineAttributesRole );
}

ThreeDLineAttributes LineDiagram::threeDLineAttributes( const QModelIndex & index ) const
{
    return qVariantValue<ThreeDLineAttributes>( model()->data( index, KDChart::ThreeDLineAttributesRole ) );
}

void LineDiagram::resizeEvent ( QResizeEvent* )
{
}

const QPair<QPointF, QPointF> LineDiagram::dataBoundaries () const
{
    if ( !checkInvariants() ) return QPair<QPointF, QPointF>( QPointF( 0, 0 ), QPointF( 0, 0 ) );
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    double xMin = 0;
    double xMax = rowCount -1;
    double yMin = 0, yMax = 0;
    ThreeDLineAttributes tda;

    // calculate boundaries for  different line types Normal - Stacked - Percent - Default Normal
    switch ( type() )
        {
        case LineDiagram::Normal:
            for ( int i=0; i<colCount; ++i ) {
                for ( int j=0; j< rowCount; ++j ) {
                    double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    yMin = qMin( yMin, value );
                    yMax = qMax( yMax, value );
                }
            }
            break;
        case LineDiagram::Stacked:
                for ( int j=0; j< rowCount; ++j ) {
                    // calculate sum of values per column - Find out stacked Min/Max
                    double stackedValues = 0;
                    for ( int i=0; i<colCount ; ++i ) {
                        stackedValues +=  model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                        yMin = qMin( yMin, stackedValues);
                        yMax = qMax( yMax, stackedValues);
                    }
                }
            break;
        case LineDiagram::Percent:

            for ( int i=0; i<colCount; ++i ) {
                for ( int j=0; j< rowCount; ++j ) {
                    // Ordinate should begin at 0 the max value being the 100% pos
                    double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    yMax = qMax( yMax, value );
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
        tda = threeDLineAttributes( index );
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
    if ( !checkInvariants() ) return;

    // draw the grid
    drawGrid( ctx );

    //calculates and stores the values
    const int rowCount = model()->rowCount(rootIndex());
    const int colCount = model()->columnCount(rootIndex());
    DataValueTextInfoList list;
    LineAttributesInfoList lineList;
    double maxValue = 0;
    double sumValues = 0;
    QVector <double > sumValuesVector;
    QModelIndex index;
    QPolygonF area;
    QBrush indexBrush;
    QPen indexPen;

    // paint different line types Normal - Stacked - Percent - Default Normal
    switch ( type() )
        {
        case LineDiagram::Normal:
            for ( int i = 0; i<colCount; ++i ) {
                area.clear();
                for ( int j=0; j< rowCount; ++j ) {
                    index = model()->index( j, i, rootIndex() );
                    double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    QPointF nextPoint = coordinatePlane()->translate( QPointF( j, value ) );
                    area.append( nextPoint );
                    if ( j+1 < rowCount ) {
                        double nextValue = model()->data( model()->index( j+1, i, rootIndex() ) ).toDouble();
                        lineList.append( LineAttributesInfo(  model()->index( j, i, rootIndex() ), value, nextValue ) );
                    }
                    list.append( DataValueTextInfo(  model()->index( j, i, rootIndex() ), nextPoint, value ) );
                }
                //area can be set by column
                LineAttributes laa = lineAttributes( model()->index( 0, i, rootIndex() ) );
                if ( laa.displayArea() )
                    paintAreas( ctx, index, area, laa.transparency() );
            }
            break;
        case LineDiagram::Stacked:
            for ( int i = 0; i<colCount; ++i ) {
                area.clear();
                for ( int j = 0; j< rowCount; ++j ) {
                    index = model()->index( j, i, rootIndex() );
                    double value = 0, stackedValues = 0, nextValues = 0;
                    QPointF nextPoint;
                    value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    for ( int k = i; k >= 0 ; --k ) {
                        stackedValues += model()->data( model()->index( j, k, rootIndex() ) ).toDouble();
                        if ( j+1 < rowCount )
                            nextValues +=  model()->data( model()->index( j+1, k, rootIndex() ) ).toDouble();
                    }
                    nextPoint = coordinatePlane()->translate( QPointF( j, stackedValues ) );
                    area.append( nextPoint );
                    if ( j+1 < rowCount )
                        lineList.append( LineAttributesInfo(  model()->index( j, i, rootIndex() ),
                                                              stackedValues, nextValues ) );
                    list.append( DataValueTextInfo( model()->index( j, i, rootIndex() ), nextPoint, value ) );
                }
                LineAttributes laa = lineAttributes( model()->index( 0, i, rootIndex() ) );
                if ( laa.displayArea() )
                    paintAreas( ctx, index, area, laa.transparency() );
            }
            break;
        case LineDiagram::Percent:
            // search for ordinate max value or 100 %
            for ( int i=0; i<colCount; ++i ) {
                for ( int j=0; j< rowCount; ++j ) {
                    double value = model()->data( model()->index( j, i, rootIndex() ) ).toDouble();
                    maxValue = qMax( maxValue, value );
                }
            }
            //calculate sum of values for each column and store
            for ( int j=0; j<rowCount; ++j ) {
                for ( int i=0; i<colCount; ++i ) {
                    double tmpValue = model()->data( model()->index ( j, i, rootIndex() ) ).toDouble();
                    if ( tmpValue > 0 )
                        sumValues += tmpValue;
                    if ( i == colCount-1 ) {
                        sumValuesVector <<  sumValues ;
                        sumValues = 0;
                    }
                }
            }
            // calculate stacked percent value
            for ( int i = 0; i<colCount; ++i ) {
                area.clear();
                for ( int j=0; j<rowCount ; ++j ) {
                    double value = 0, stackedValues = 0, nextValues = 0;
                    QPointF nextPoint;
                     index = model()->index( j, i, rootIndex() );
                    value = model()->data(  model()->index( j, i, rootIndex() ) ).toDouble();
                    //calculate stacked percent value- we only take in account positives values for now.
                    for ( int k = i; k >= 0 ; --k ) {
                        if ( model()->data( model()->index( j, k, rootIndex() ) ).toDouble() > 0)
                            stackedValues += model()->data( model()->index( j, k, rootIndex() ) ).toDouble();
                        if ( j+1 < rowCount ){
                            if ( model()->data( model()->index( j+1, k, rootIndex() ) ).toDouble() > 0 )
                                nextValues += model()->data( model()->index( j+1, k, rootIndex() ) ).toDouble();
                        }
                    }
                    nextPoint = coordinatePlane()->translate( QPointF( j,  stackedValues/sumValuesVector.at(j)* maxValue ) );
                    area.append( nextPoint );
                    if ( j+1 < rowCount )
                        lineList.append( LineAttributesInfo( model()->index( j, i, rootIndex() ), stackedValues/sumValuesVector.at(j)* maxValue, nextValues/sumValuesVector.at(j+1)* maxValue ) );
                    list.append( DataValueTextInfo( model()->index( j, i, rootIndex() ), nextPoint, value ) );
                }
                LineAttributes laa = lineAttributes( model()->index( 0, i, rootIndex() ) );
                if ( laa.displayArea() )
                    paintAreas( ctx, index, area, laa.transparency() );
            }
            // AbstractAxis settings - see AbstractDiagram and CartesianAxis
	    setPercentMode( true );
            break;
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

void LineDiagram::paintLines( PaintContext* ctx, const QModelIndex& index, double from, double to )
{
    ThreeDLineAttributes td = threeDLineAttributes(index);
    if ( td.isEnabled() ) {
        const int lineDepth = td.depth();
        paintThreeDLines( ctx, index, from, to, lineDepth );
    }    else {
        //get the brush and pen to be used from the AbstractDiagram methods
        QBrush indexBrush ( brush( index ) );
        PainterSaver painterSaver( ctx->painter() );
        ctx->painter()->setRenderHint ( QPainter::Antialiasing );
        ctx->painter()->setBrush( indexBrush );
        ctx->painter()->setPen( pen( index ) );
        if ( index.row() + 1 < model()->rowCount(rootIndex()) ) {
            const QPointF posfrom = coordinatePlane()->translate( QPointF( index.row(), from) );
            const QPointF posto = coordinatePlane()->translate( QPointF( index.row()+1, to ) );
            ctx->painter()->drawLine( posfrom, posto);
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

void LineDiagram::paintThreeDLines(PaintContext* ctx, const QModelIndex& index, double from, double to, const int depth  )
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
    return model()->rowCount(rootIndex());
}

const int LineDiagram::numberOfOrdinateSegments () const
{
    return model()->columnCount(rootIndex());
}

//#endif


