/* -*- Mode: C++ -*-
   KDChart1 - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart1 library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart1 licenses may use this file in
 ** accordance with the KDChart1 Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart1 Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#include <KDChart1Widget.h>
#include <KDChart1.h>
#include <KDChart1Params.h>
#include <KDChart1TableBase.h>

#include <qpainter.h>

using namespace KDChart;

class KDChart1Widget::Private
{
    friend class KDChart1Widget;
public:
    Private();
    ~Private();
private:
    KDChart1Params* params;
    KDChart1TableDataBase* data;
    KDChart1TableDataBase* defaultData;
    bool activeData;
    bool doubleBuffered;
    KDChart1DataRegion* mousePressedOnRegion;
    KDChart1DataRegionList dataRegions;
};

KDChart1Widget::Private::Private() :
    params( 0 ),
    data( 0 ),
    defaultData( 0 ),
    activeData( false ),
    mousePressedOnRegion( 0 )
{
    // this bloc left empty intentionally
}

KDChart1Widget::Private::~Private()
{
    // delete any regions that might still be registered
    qDeleteAll( dataRegions );
    dataRegions.clear();
    delete defaultData;
}


/**
  \class KDChart1Widget KDChart1Widget.h

  \brief The entry point into the charting that most people will want
  to use.

  Simply create a KChartWidget in your application and put it where
  you want in your widget hierarchy and create a KChartParams object
  that specifies how the chart should be drawn.

  \note If for some reason you are NOT using the
  KDChart1Widget class but calling the painting methods of KDChart1 directly,
  you probably will also use the KDChart1DataRegionList class:
  This class is derived from QPtrList, so all of the Qt documentation
  for this class is valid for KDChart1DataRegionList too, e.g. freeing
  of the pointers stored can either be done automatically or
  manually - so PLEASE take the time to read the reference information for this class!

  \sa KDChart1, KDChart1DataRegionList
  */


/**
  Default Constructor.

  Sets params and data pointers to zero, you should call setParams
  and setData before using this chart otherwise only a simple
  default bar chart will be shown.

  \param parent the widget parent; passed on to QWidget
  \param name the widget name; passed on to QWidget
  */

KDChart1Widget::KDChart1Widget( QWidget* parent, const char* name ) :
    KDChart::Widget( parent ),
    d( new Private() )
{
    Q_UNUSED(name);
    setDoubleBuffered( true );
    setBackgroundRole( QPalette::NoRole );

    // Let us the default chart, we used to have in KD Chart 1.x:
    justifyModelSize( 3, 1 );
    d->defaultData = new KDChart1TableData( 3, 1 );
        // 1st series
    d->defaultData->setCell( 0, 0,    12.5   );
        // 2nd series
    d->defaultData->setCell( 1, 0,     8.0   );
        // 3rd series
    d->defaultData->setCell( 2, 0,    15.0   );
    setData( d->defaultData );
}


/**
  Constructor. Stores the chart parameters.

  \param params the specification of the chart
  \param data the data to be displayed as a chart
  \param parent the widget parent; passed on to QWidget
  \param name the widget name; passed on to QWidget
  */

KDChart1Widget::KDChart1Widget( KDChart1Params* params,
        KDChart1TableDataBase* data,
        QWidget* parent, const char* name ) :
    KDChart::Widget( parent ),
    d( new Private() )
{
    Q_UNUSED(name);
    setParams( params );
    setData( data );
    setDoubleBuffered( true );
    setBackgroundRole( QPalette::NoRole );
}


/**
  Destructor.
  */
KDChart1Widget::~KDChart1Widget()
{
    delete d; d = 0;
    KDChart1AutoColor::freeInstance();
}

/*
void KDChart1Widget::paintTo( QPainter& painter,
        const QRect* rect )
{
    KDChart1::paint( &painter, params, data, &dataRegions, rect );
}


void KDChart1Widget::print( QPainter& painter,
        const QRect* rect )
{
    bool oldOpt=true;
    if( params ){
        oldOpt = params->optimizeOutputForScreen();
        params->setOptimizeOutputForScreen( false );
    }
    bool bOldBuf = doubleBuffered;
    doubleBuffered = false;
    paintTo( painter, rect );
    doubleBuffered = bOldBuf;
    if( params )
        params->setOptimizeOutputForScreen( oldOpt );
}


void KDChart1Widget::paintEvent( QPaintEvent* event )
{
    if( doubleBuffered ) {
        // if double-buffering, paint onto the pixmap and copy
        // afterwards
        buffer.fill( backgroundColor() );
        QPainter painter( &buffer );
        paintTo( painter );
        bitBlt( this, event->rect().topLeft(), &buffer, event->rect() );
    } else {
        // if not double-buffering, paint directly into the window
        QPainter painter( this );
        paintTo( painter );
    }
}
*/

/**
  \internal
  */
void KDChart1Widget::mousePressEvent( QMouseEvent* event )
{
    Q_UNUSED(event);
    /*
    if ( !d->activeData )
        return ;

    d->mousePressedOnRegion = 0;
    KDChart1DataRegion* current = 0;
    //QPtrListIterator < KDChart1DataRegion > it( dataRegions );
    for( current = d->dataRegions.last(); current; current = d->dataRegions.prev() ){
    //while ( ( current = it.current() ) ) {
        if ( current->contains( event->pos() ) ) {
            d->mousePressedOnRegion = current;
            if ( event->button() == LeftButton ){
                emit dataLeftPressed( current->row, current->col );
                emit dataLeftPressed( event->pos() );
            }else if ( event->button() == MidButton ){
                emit dataMiddlePressed( current->row, current->col );
                emit dataMiddlePressed( event->pos() );
            }else{
                emit dataRightPressed( current->row, current->col );
                emit dataRightPressed( event->pos() );
            }
            return;
        }
    }
    */
}


/**
  \internal
  */
void KDChart1Widget::mouseReleaseEvent( QMouseEvent* event )
{
    Q_UNUSED(event);
    /*
    if ( !d->activeData )
        return ;

    KDChart1DataRegion* current = 0;
    QPtrListIterator < KDChart1DataRegion > it( dataRegions );
    while ( ( current = it.current() ) ) {
        ++it;
        if ( current->contains( event->pos() ) ) {
            if ( event->button() == LeftButton ) {
                emit dataLeftReleased( current->row, current->col );
                emit dataLeftReleased( event->pos() );
                if ( mousePressedOnRegion == current ){
                    emit dataLeftClicked( current->row, current->col );
                    emit dataLeftClicked( event->pos() );
                }
            } else if ( event->button() == MidButton ) {
                emit dataMiddleReleased( current->row, current->col );
                emit dataMiddleReleased( event->pos() );
                if ( mousePressedOnRegion == current ){
                    emit dataMiddleClicked( current->row, current->col );
                    emit dataMiddleClicked( event->pos() );
                }
            } else {
                emit dataRightReleased( current->row, current->col );
                emit dataRightReleased( event->pos() );
                if ( mousePressedOnRegion == current ){
                    emit dataRightClicked( current->row, current->col );
                    emit dataRightClicked( event->pos() );
                }
            }
        }
    }
    */
}


/**
  If \a active is true, this widget reports mouse presses, releases
  and clicks on the data segments it displays. This can slow down the
  display process, so this is turned off by default.

  If active data reporting is turned on when the widget is already
  shown, data will be reported after the next repaint(). Call
  repaint() explicitly if necessary.

  Active data is currently supported for bar, pie, and line charts
  (the latter only with markers, as trying to hit the line would be
  too difficult for the user anyway).

  \param active if true, the widget reports mouse events
  \sa isActiveData()
  */
void KDChart1Widget::setActiveData( bool active )
{
    d->activeData = active;
}


/**
  Returns true if the widget is configured to report mouse
  events. The default is not to report mouse events.

  \return true if the widget is configured to report mouse events,
  false otherwise
  \sa setActiveData()
  */
bool KDChart1Widget::isActiveData() const
{
    return d->activeData;
}


/**
  If \a doublebuffered is true, the widget will double-buffer
  everything while drawing which reduces flicker a lot, but requires
  more memory as an off-screen buffer of the same size as the widget
  needs to be kept around. However, in most cases, it is worth
  spending the extra memory. Double-buffering is on by
  default. Turning double-buffering on or off does not trigger a
  repaint.

  \param doublebuffered if true, turns double-buffering on, if false,
  turns double-buffering off
  \sa isDoubleBuffered
  */
void KDChart1Widget::setDoubleBuffered( bool doublebuffered )
{
    Q_UNUSED(doublebuffered);
    d->doubleBuffered = false;
    // Double buffering is done by Qt now.
}


/**
  Returns whether the widget uses double-buffering for drawing. See
  \a setDoubleBuffered() for an explanation of double-buffering.

  \return true if double-buffering is turned on, false otherwise
  */
bool KDChart1Widget::isDoubleBuffered() const
{
    return true;
    // Double buffering is done by Qt now.
}


/**
  Set an entire new parameter set.
  (Normally you might prefer modifying the existing parameters
  rather than specifying a new set.)
  */
void KDChart1Widget::setParams( KDChart1Params* params )
{
    d->params = params;
}

/**
  Set an entire new data table.
  */
void KDChart1Widget::setData( KDChart1TableDataBase* data )
{
    d->data = data;
    QVariant::Type typeX;
    const bool hasXValues =
        data->cellsHaveSeveralCoordinates( &typeX )
            && ( QVariant::Double == typeX );

    const int nRows = data->rows();
    const int nCols = data->cols();
qDebug("nCols: %i",nCols);

    QVariant variantY, variantX;
    for( int iRow=0; iRow < nRows; ++iRow ){
        if( hasXValues ){
            QVector< QPair< double, double > > rowData;
            for( int iCol=0; iCol < nCols; ++iCol ){
                if( data->cellCoords( iRow, iCol, variantY, variantX ) ){
                    const bool foundY = QVariant::Double == variantY.type();
                    const bool foundX = QVariant::Double == variantX.type();
                    if( foundX || foundY ){
                        double valueX = 0.0;
                        double valueY = 0.0;
                        if( foundX )
                            valueY = variantY.toDouble();
                        if( foundY )
                            valueX = variantX.toDouble();
                        rowData.append( qMakePair( valueX, valueY ) );
qDebug("iCol: %i",iCol);
                    }
                }
            }
            setDataset( iRow, rowData );
        }else{
            QVector< double > rowData;
            for( int iCol=0; iCol < nCols; ++iCol )
                if( data->cellCoord( iRow, iCol, variantY ) &&
                    ( QVariant::Double == variantY.type() ) ){
                    rowData.append( variantY.toDouble() );
qDebug("iCol: %i",iCol);
                    }
            setDataset( iRow, rowData );
qDebug("iRow: %i",iRow);
        }
    }
}


/**
  Returns a pointer to the current parameter set.
  */
KDChart1Params* KDChart1Widget::params() const
{
    return d->params;
}

/**
  Returns a pointer to the current data table.
  */
KDChart1TableDataBase* KDChart1Widget::data() const
{
    return d->data;
}


const KDChart1DataRegionList* KDChart1Widget::dataRegions() const
{
     return &d->dataRegions;
}

/**
  \fn void KDChart1Widget::barsDisplayed( int barsDisplayed, int barsLeft )

  This signal is emitted when drawing of a bar chart is done.
  Use it to determine if all bars have been drawn: in case
  you specified both the bar width and the value block gap width
  it might be that KD Chart is not able to display all bars since
  they do not fit into the available horizontal space.

  The value of barsLeft indicates how many bars could not be
  drawn because the data area was not wide enough.

  \sa KDChart1Params::numBarsDisplayed, KDChart1Params::numBarsLeft
  */

/**
  \fn void KDChart1Widget::dataLeftClicked( uint row, uint col )

  This signal is emitted when a data item was clicked onto with the left mouse button.

  The values of row / col indicate the respective dataset (row) and item (col).

  \note There is another signal sent simultaneously: reporting the screen coordinates clicked onto.

  \sa dataLeftReleased
  \sa dataRightClicked, dataMiddleClicked
  */
/**
  \fn void KDChart1Widget::dataRightClicked( uint row, uint col )

  This signal is emitted when a data item was clicked onto with the right mouse button.

  The values of row / col indicate the respective dataset (row) and item (col).

  \note There is another signal sent simultaneously: reporting the screen coordinates clicked onto.

  \sa dataRightReleased
  \sa dataLeftClicked, dataMiddleClicked
  */
/**
  \fn void KDChart1Widget::dataMiddleClicked( uint row, uint col )

  This signal is emitted when a data item was clicked onto with the middle mouse button.

  The values of row / col indicate the respective dataset (row) and item (col).

  \note There is another signal sent simultaneously: reporting the screen coordinates clicked onto.

  \sa dataMiddleReleased
  \sa dataLeftClicked, dataRightClicked
  */

/**
  \fn void KDChart1Widget::dataLeftClicked( const QPoint & pnt )

  This signal is emitted when a data item was clicked onto with the left mouse button.

  The value of pnt indicates the screen coordinates in relation to the origin of the data area.

  \note There is another signal sent simultaneously: reporting which data item was clicked onto.

  \sa dataLeftReleased
  \sa dataRightClicked, dataMiddleClicked
  */
/**
  \fn void KDChart1Widget::dataRightClicked( const QPoint & pnt )

  This signal is emitted when a data item was clicked onto with the right mouse button.

  The values of row / col indicate the screen coordinates in relation to the origin of the data area.

  \note There is another signal sent simultaneously: reporting which data item was clicked onto.

  \sa dataRightReleased
  \sa dataLeftClicked, dataMiddleClicked
  */
/**
  \fn void KDChart1Widget::dataMiddleClicked( const QPoint & pnt )

  This signal is emitted when a data item was clicked onto with the middle mouse button.

  The values of row / col indicate the screen coordinates in relation to the origin of the data area.

  \note There is another signal sent simultaneously: reporting which data item was clicked onto.

  \sa dataMiddleReleased
  \sa dataLeftClicked, dataRightClicked
  */
