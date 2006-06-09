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
#include "KDChart1_Presentation.h"

#include <qpainter.h>
#include <qprinter.h>
#include <qaction.h>
#include <qkeysequence.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpalette.h>
#include <qsizepolicy.h>
#include <qmessagebox.h>
#include <qbitmap.h>
#include <qimage.h>
#include <qfile.h>

#include <QListWidget>
#include <QColorGroup>
#include <QToolButton>

#include <KDChart1.h>
#include <KDChart1Widget.h>
#include <KDChart1Enums.h>
#include <KDChart1Table.h>
#include <KDChart1CustomBox.h>
#include <KDChart1TextPiece.h>
#include <KDChart1PropertySet.h>

#include "KDChart1_Presentation_XPM.inc"

#define CHART 0


// #define QWS



//  number  param set name
static QString examples[] = { /* 0 */  "bar: labels rotated",
    /* 1 */  "bar: labels centered inside",
    /* 2 */  "bar: 3d, multiple rows",
    /* 3 */  "line: log. ordinate",
    /* 4 */  "line: 3d",

    // temporarily disabled:
    // /* 5 */  "line: 3d, rotated",

    /*  5 */  "line: reverted ordinate axis",
    /*  6 */  "bars + lines in one chart",
    /*  7 */  "point: circular markers",
    /*  8 */  "pie: simple",
    /*  9 */  "pie: simple, sagittal texts",
    /* 10 */  "pie: 3d, exploding",
    /* 11 */  "ring: simple",
    /* 12 */  "ring: relative thickness",
    /* 13 */  "ring: relative, exploding",
    /* 14 */  "ring: rel., some exploding",
    /* 15 */  "area: simple",
    /* 16 */  "area: reverse drawing view",
    /* 17 */  "area: stacked",
    /* 18 */  "area: percent",
    /* 19 */  "area: cell-specific brushes",
    /* 20 */  "high/low: simple",
    /* 21 */  "high/low: close values",
    /* 22 */  "high/low: open/close values",
    /* 23 */  "line + high/low in one chart",
    /* 24 */  "box/whisker: default",
    /* 25 */  "box/whisker: no outliers",
    /* 26 */  "box/whisker: value texts",
    /* 27 */  "box/whisker: no fence texts",
    /* 28 */  "line + box/whisker",
    /* 29 */  "polar: default view",
    /* 30 */  "polar: no markers",
    /* 31 */  "polar: no mark., no texts",
    /* 32 */  "polar: stacked view",
    /* 33 */  "polar: percentage view",
    /* 34 */  "polar: different design",
    /* 35 */  "isometric coordinate system",
    /* 36 */  "a simple grid plot",
    /* 37 */  "function plot with axes labels",
    /* 38 */  "two chart sharing one abscissa",
    /* 39 */  "horizontal lines: like Bars",
    /* 40 */  "nice full featured chart",
    "Z" };


KDChart1PresentationWidget::KDChart1PresentationWidget( int argc, char **argv )
    : QWidget( 0 ),
     _argc( argc ),
     _argv( argv ),
    _internalSizeUpdate( false ),
    _auto( false ),
#ifdef USE_SPLASH_SCREEN
    _splash( true ),
#else
    _splash( false ),
#endif
    _p(     0 ),
    _d(     0 ),
    _l(     0 ),  // for 'normal' machines
    _box(   0 ),  // for QWS
    _run(   0 ),
    _size(  0 ),
    _kdc(   0 ),
    _tId(  -1 ),
    _defaultSize0( 16 ),
_defaultSize1( 16 )
{
    // minimal data and parameter initialisation:
    resetData( 1, 1 );
    resetParams( KDChart1Params::Bar );


    // widget layout setup:

#ifdef QWS
    QVBoxLayout* l = new QVBoxLayout( this );
    _kdc = new KDChart1Widget( _p, _d, this, "KDChart1Widget");
#else
    int gap( 16 );
    QGridLayout * g = new QGridLayout( this );
    //g->setSpacing( gap );

    g->addWidget( new QLabel( "<b>Configuration:</b>", this ), 0, 0 );
    g->addWidget( new QLabel( "<b>Result:</b>", this ), 0, 1 );

    _kdc = new KDChart1Widget( _p, _d, this, "KDChart1Widget");
    g->addWidget( _kdc, 1, 1 );
#endif // QWS


#ifdef QWS
    QHBoxLayout * hLay = new QHBoxLayout( 0 );
    l->addLayout( hLay );

    _box = new QComboBox( this, "ListOfConfigurations" );
    hLay->addWidget( _box );

    hLay->addItem( new QSpacerItem( 12, 1 ) );
    _run = new QPushButton( "&Go", this );
    hLay->addWidget( _run );
    _run->setToggleButton( true );
    _run->setOn( false );

    hLay->addItem( new QSpacerItem( 12, 1 ) );
    _size = new QSpinBox( 2, 50, 1, this, "sizeBox" );
    hLay->addWidget( _size );
    _size->setValue( 10 );

    l->addWidget( _kdc );
    int i( -1 );
    while( "Z" != examples [ ++i ] )
        _box->insertItem( examples [ i ], i );
    _box->setCurrentItem( i-1 );
    connect( _box, SIGNAL(activated( const QString& )),
            this, SLOT(  setConfiguration( const QString& )) );

    _box->setFocus();
#else
    QBoxLayout * vLay = new QVBoxLayout( 0 );
    g->addLayout( vLay, 1, 0 );


    _l = new QListWidget( this );
    vLay->addWidget( _l );
    _l->setFocusPolicy( Qt::StrongFocus );

    QPalette lPal( _l->palette() );
    lPal.setColor( QPalette::Background, __chamois );
    _l->setPalette( lPal  );



    vLay->addItem( new QSpacerItem( 1, gap ) );
    QBoxLayout * hLay = new QHBoxLayout( 0 );
    vLay->addLayout( hLay );


    _run = new QToolButton( this );
    hLay->addWidget( _run );
    _run->setText( "&Go" );
    _run->setToolButtonStyle( Qt::ToolButtonTextOnly );
    //_run->setToggleButton( true );
    _run->setCheckable( true );
    _run->setChecked( false );


    hLay->addItem( new QSpacerItem( gap/3, 1 ) );

    hLay->addWidget( new QLabel( "font mag.: ", this ) );
    _size = new QSpinBox( this );
    _size->setRange( 2, 50 );
    _size->setSingleStep( 1 );
    hLay->addWidget( _size );
    _size->setValue( 10 );
    _size->setPalette( lPal  );


    g->setRowStretch( 0,0 );
    //g->setColStretch( 0,0 );
    g->setRowStretch( 1,1 );
    //g->setColStretch( 1,1 );

    int i( -1 );
    while( "Z" != examples [ ++i ] )
        _l->addItem( examples [ i ] );
    _l->setCurrentRow( 0 );

    connect( _l,    SIGNAL( currentItemChanged( QListWidgetItem *, QListWidgetItem * ) ),
            this,  SLOT(   setConfiguration( QListWidgetItem *, QListWidgetItem * ) ) );

    _l->setFocus();
#endif

    connect( _run,  SIGNAL( clicked() ),
            this,  SLOT(   runClicked() ) );
    connect( _size, SIGNAL( valueChanged( int ) ),
            this,  SLOT(   sizeEdited(   int ) ) );


    connect( _kdc, SIGNAL( dataLeftPressed( uint, uint ) ),
            this, SLOT( slotMouseLeftPressed( uint, uint ) ) );
    /*  connect( _kdc, SIGNAL( dataLeftReleased( uint, uint ) ),
        this, SLOT( slotMouseLeftReleased( uint, uint ) ) );
        connect( _kdc, SIGNAL( dataLeftClicked( uint, uint ) ),
        this, SLOT( slotMouseLeftClicked( uint, uint ) ) );     */
    connect( _kdc, SIGNAL( dataMiddlePressed( uint, uint ) ),
            this, SLOT( slotMouseMiddlePressed( uint, uint ) ) );
    /*  connect( _kdc, SIGNAL( dataMiddleReleased( uint, uint ) ),
        this, SLOT( slotMouseMiddleReleased( uint, uint ) ) );
        connect( _kdc, SIGNAL( dataMiddleClicked( uint, uint ) ),
        this, SLOT( slotMouseMiddleClicked( uint, uint ) ) );   */
    connect( _kdc, SIGNAL( dataRightPressed( uint, uint ) ),
            this, SLOT( slotMouseRightPressed( uint, uint ) ) );
    /*  connect( _kdc, SIGNAL( dataRightReleased( uint, uint ) ),
        this, SLOT( slotMouseRightReleased( uint, uint ) ) );
        connect( _kdc, SIGNAL( dataRightClicked( uint, uint ) ),
        this, SLOT( slotMouseRightClicked( uint, uint ) ) );    */

    _kdc->setActiveData( true );

    _run->animateClick();
    _auto = true;

    _tId  = startTimer( 5000 ); // emit a timer signal every 10 seconds
    QTimerEvent eve( _tId );
    timerEvent( &eve ); // call it initially to prevent empty display
}


void KDChart1PresentationWidget::timerEvent( QTimerEvent *e )
{
    if( _auto && e && e->timerId() == _tId ) {
#ifdef USE_SPLASH_SCREEN
        _splash = !_splash;
#endif
#ifdef QWS
        if ( !_splash ) {
            int i = _l->currentRow() + 1;
            if ( _box->count() == i+1 )
                i = 0;
            else
                ++i;
            _box->setCurrentItem( i );
        }
        setConfiguration( _box->currentText() );
#else
        QListWidgetItem* prevItem = 0;
        int i = _l->currentRow();
        if ( !_splash ) {
            prevItem = _l->currentItem();
            if ( static_cast < int > ( _l->count() ) == i+1 )
                i = 0;
            else
                ++i;
            _l->setCurrentRow( i );
        }
        else
            setConfiguration( _l->item( i ), prevItem );
#endif
    }
}


void KDChart1PresentationWidget::runClicked()
{
    _auto = _run->isChecked();
}


void KDChart1PresentationWidget::sizeEdited( int value )
{
    if ( _kdc && _p ) {
        bool wasAuto = _auto;
        _auto = false;
        bool wasInternalFlag = _internalSizeUpdate;
        _internalSizeUpdate = true;
        _p->setPrintDataValuesFontRelSize( 0,
                static_cast < uint > ( value / 10.0 * _defaultSize0 ) );
        _p->setPrintDataValuesFontRelSize( 1,
                static_cast < uint > ( value / 10.0 * _defaultSize1 ) );
        if( !wasInternalFlag )
            _kdc->update();
        _internalSizeUpdate = wasInternalFlag;
        _auto = wasAuto;
    }
}


void KDChart1PresentationWidget::resizeEvent( QResizeEvent * )
{
    if( _splash )
        setConfiguration( _l->currentItem()->text() );
}


void KDChart1PresentationWidget::slotMouseLeftPressed( uint row, uint col )
{
    reportMouseEvent( "left mouse button pressed", row, col );
}
/*
   void KDChart1PresentationWidget::slotMouseLeftReleased( uint row, uint col )
   {
   reportMouseEvent( "left mouse button released", row, col );
   }
   void KDChart1PresentationWidget::slotMouseLeftClicked( uint row, uint col )
   {
   reportMouseEvent( "left mouse button clicked", row, col );
   }
   */
void KDChart1PresentationWidget::slotMouseMiddlePressed( uint row, uint col )
{
    reportMouseEvent( "middle mouse button pressed", row, col );
}
/*
   void KDChart1PresentationWidget::slotMouseMiddleReleased( uint row, uint col )
   {
   reportMouseEvent( "middle mouse button released", row, col );
   }
   void KDChart1PresentationWidget::slotMouseMiddleClicked( uint row, uint col )
   {
   reportMouseEvent( "middle mouse button clicked", row, col );
   }
   */
void KDChart1PresentationWidget::slotMouseRightPressed( uint row, uint col )
{
    reportMouseEvent( "right mouse button pressed", row, col );
}
/*
   void KDChart1PresentationWidget::slotMouseRightReleased( uint row, uint col )
   {
   reportMouseEvent( "right mouse button released", row, col );
   }
   void KDChart1PresentationWidget::slotMouseRightClicked( uint row, uint col )
   {
   reportMouseEvent( "right mouse button clicked", row, col );
   }
   */


void KDChart1PresentationWidget::reportMouseEvent( QString txt, uint row, uint col )
{
    QString t( txt );
    t += "\ncell [";
    t += QString::number( row );
    t += ",   ";
    t += QString::number( col );
    t += "]";
    QVariant vValY, vValX;
    if( _d->cellCoords( row, col, vValY, vValX ) ){
        t += "\n value: ";
        t += QString::number( vValY.toDouble() );
        if( QVariant::Double == vValX.type() ){
            t += " , ";
            t += QString::number( vValX.toDouble() );
        }
    }
    QMessageBox::information( this, "Mouse event reported", t );
}


KDChart1PresentationWidget::~KDChart1PresentationWidget()
{
    _auto = false;
    delete _kdc;
    delete _p;
    delete _d;
}

void KDChart1PresentationWidget::resetParams( KDChart1Params::ChartType cType0,
        KDChart1Params::ChartType cType1,
        QPen                     pen,
        QBrush                   brush,
        const QPixmap*           backPixmap,
        KDFrame1::BackPixmapMode  backPixmapMode )
{
    if( !_internalSizeUpdate ) {
        _internalSizeUpdate = true;
        delete _p;
        _p = new KDChart1Params;
        if( _kdc )
            _kdc->setParams( _p );
        _p->setOptimizeOutputForScreen( false );
        _p->setGlobalLeading(  7,7, 7,7 );
        _p->setSimpleFrame( KDChart1Enums::AreaInnermost,
                0,0,  6,6,
                true,
                true,
                KDFrame1::FrameFlat,
                1,
                0,
                pen,
                brush,
                backPixmap,
                backPixmapMode );
        _p->setChartType( cType0 );
        _defaultSize0 = _p->dataValuesFontRelSize( 0 );
        if ( KDChart1Params::NoType != cType1 ) {
            _p->setAdditionalChartType( cType1 );
            _defaultSize1 = _p->dataValuesFontRelSize( 1 );
        } else
            _defaultSize1 = 16;
        if( _size )
            sizeEdited( _size->value() );
        _internalSizeUpdate = false;
    }
}


void KDChart1PresentationWidget::resetData( uint rows, uint cols )
{
    if( _d )
        delete _d;
    _d = new KDChart1TableData( rows, cols );
    if( _kdc )
        _kdc->setData( _d );
}


void KDChart1PresentationWidget::setConfiguration( const QString& txt)
{
    QListWidgetItem lbi( txt );
    QListWidgetItem prev( "" );
    setConfiguration( &lbi, &prev );
}

void functionA( double t, double& x, double& y )
{
    x = t;
    y = t * t / 32.0;
}

void functionB( double t, double& x, double& y )
{
    x = t;
    y = sqrt( t );
}

void functionC( double t, double& x, double& y )
{
    x = t;
    y = sin( t );
}

double getGridPlotValue( double y, double x )
{
    return sin(x) + sin(y);
}

void KDChart1PresentationWidget::setConfiguration( QListWidgetItem * item, QListWidgetItem * prevItem )
{
    Q_UNUSED(prevItem);
    bool wasAuto = _auto;
    _auto = false;
    if ( _splash ) {
        resetData( 1,1 );
        resetParams( KDChart1Params::Bar );
        _d->setCell( 0, 0, 10.0 );
        KDChart1TextPiece textPiece1( "<b></b>",
                                     QFont( "helvetica", 1, QFont::Normal, false ) );
        KDChart1CustomBox customBox1(
            textPiece1,
            -20,true,
            0,0,
            -1000,-1000,
            Qt::black,
            Qt::NoBrush,
            KDChart1Enums::AreaInnermost,
            KDChart1Enums::PosTopLeft,
            Qt::AlignTop + Qt::AlignLeft,
            0, 1 );
        _p->setSimpleFrame(
                KDChart1Enums::AreaCustomBoxesBASE
                + _p->insertCustomBox( customBox1 ),
                0,0,  6,6,
                true,
                true,
                KDFrame1::FrameFlat,
                1,
                0,
                Qt::NoPen,
                QBrush( __chamois ) );

        KDChart1TextPiece textPiece2( QString::fromUtf8("<b>Klarälvdalens&nbsp;Datakonsult&nbsp;AB</b>"),
                                     QFont( "helvetica", 1, QFont::Normal, true ) );
        KDChart1CustomBox customBox2(
            textPiece2,
            -40,true,
            -50,-60,
            0,0,
            Qt::darkBlue,
            Qt::NoBrush,
            KDChart1Enums::AreaInnermost,
            KDChart1Enums::PosTopLeft,
            Qt::AlignTop + Qt::AlignLeft,
            0, 1 );
        uint id = _p->insertCustomBox( customBox2 );
        KDChart1TextPiece textPiece3( "<b>Platform-Independent&nbsp;Software</b>",
                                     QFont( "helvetica", 1, QFont::Normal, true ) );
        KDChart1CustomBox customBox3(
            textPiece3,
            -40,true,
            -50,-80,
            0,0,
            Qt::darkRed,
            Qt::NoBrush,
            KDChart1Enums::AreaCustomBoxesBASE+id,
            KDChart1Enums::PosBottomLeft,
            Qt::AlignTop + Qt::AlignLeft,
            0, 1 );
        id = _p->insertCustomBox( customBox3 );
        KDChart1TextPiece textPiece4( "<b>Qt&nbsp;Components</b>",
                                     QFont( "helvetica", 1, QFont::Normal, true ) );
        KDChart1CustomBox customBox4(
            textPiece4,
            -35,true,
            0,-180,
            0,0,
            Qt::darkBlue,
            Qt::NoBrush,
            KDChart1Enums::AreaCustomBoxesBASE+id,
            KDChart1Enums::PosBottomLeft,
            Qt::AlignTop + Qt::AlignLeft,
            0, 1 );
        id = _p->insertCustomBox( customBox4 );
        KDChart1TextPiece textPiece5( "<b>Qt&nbsp;Courses&nbsp;and&nbsp;Mentoring</b>",
                          QFont( "helvetica", 1, QFont::Normal, true ) );
        KDChart1CustomBox customBox5(
            textPiece5,
            -35,true,
            0,-80,
            0,0,
            Qt::darkBlue,
            Qt::NoBrush,
            KDChart1Enums::AreaCustomBoxesBASE+id,
            KDChart1Enums::PosBottomLeft,
            Qt::AlignTop + Qt::AlignLeft,
            0, 1 );
        id = _p->insertCustomBox( customBox5 );
        KDChart1TextPiece textPiece6( "<b>Custom&nbsp;Software&nbsp;Development</b>",
                                     QFont( "helvetica", 1, QFont::Normal, true ) );
        KDChart1CustomBox customBox6(
            textPiece6,
            -35,true,
            0,-80,
            0,0,
            Qt::darkBlue,
            Qt::NoBrush,
            KDChart1Enums::AreaCustomBoxesBASE+id,
            KDChart1Enums::PosBottomLeft,
            Qt::AlignTop + Qt::AlignLeft,
            0, 1 );
        id = _p->insertCustomBox( customBox6 );

        _kdc->update();
        _auto = wasAuto;
        return;
    }
    int selected( -1 );
    int i( -1 );

    while( "Z" != examples [ ++i ] )
        if( examples [ i ] == item->text() ) {
            selected = i;
            break;
        }
    if( 0 <= selected ) {
        switch ( selected ) {
            case  0: setConfiguration_0();  break;
            case  1: setConfiguration_1();  break;
            case  2: setConfiguration_2();  break;
            case  3: setConfiguration_3();  break;
            case  4: setConfiguration_4();  break;
            case  5: setConfiguration_5();  break;
            case  6: setConfiguration_6();  break;
            case  7: setConfiguration_7();  break;
            case  8: setConfiguration_8();  break;
            case  9: setConfiguration_9();  break;
            case 10: setConfiguration_10(); break;
            case 11: setConfiguration_11(); break;
            case 12: setConfiguration_12(); break;
            case 13: setConfiguration_13(); break;
            case 14: setConfiguration_14(); break;
            case 15: setConfiguration_15(); break;
            case 16: setConfiguration_16(); break;
            case 17: setConfiguration_17(); break;
            case 18: setConfiguration_18(); break;
            case 19: setConfiguration_19(); break;
            case 20: setConfiguration_20(); break;
            case 21: setConfiguration_21(); break;
            case 22: setConfiguration_22(); break;
            case 23: setConfiguration_23(); break;
            case 24:
            case 25:
            case 26:
            case 27: setConfiguration_24_27( selected ); break;
            case 28: setConfiguration_28(); break;
            case 29: setConfiguration_29(); break;
            case 30: setConfiguration_30(); break;
            case 31: setConfiguration_31(); break;
            case 32: setConfiguration_32(); break;
            case 33: setConfiguration_33(); break;
            case 34: setConfiguration_34(); break;
            case 35: setConfiguration_35(); break;
            case 36: setConfiguration_36(); break;
            case 37: setConfiguration_37(); break;
            case 38: setConfiguration_38(); break;
            case 39: setConfiguration_39(); break;
            case 40: setConfiguration_40(); break;
        }
        _kdc->update();
    }
    _auto = wasAuto;
}


void KDChart1PresentationWidget::setConfiguration_0()
{
    // "bar: labels rotated"
    resetData( 5, 5 );
    resetParams( KDChart1Params::Bar );
    _p->setThreeDBars( false );

    _p->setHeader1Text( "Bar chart");
    _p->setHeader2Text( "- normal view with default text rotation -" );
    _p->setFooterText(  "[ text font/size/color/position/rotation may be defined freely ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

/*
   QStringList xAxisLabel;
   QStringList xAxisShortLabel;

   xAxisLabel << "veryLongItemName1"<<"veryLongItemName2"<<"veryLongItemName3"<<"veryLongItemName4" << "veryLongItemName5";



/ *
    //xAxisShortLabel << "short1" << "short2" << "short3" << "short4" << "short5";
    _p->setAxisLabelStringParams( KDChart1AxisParams::AxisPosBottom,
             &xAxisLabel, &xAxisShortLabel );

    _p->setGlobalLeadingLeft( 50 );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisLabelsDontAutoRotate( true );
    pa.setAxisLabelsRotation( 350 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );
* /
   _p->setAxisLabelStringParams( KDChart1AxisParams::AxisPosBottom,
            &xAxisLabel, &xAxisLabel );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisLabelsRotation( 270 );
    pa.setAxisAreaMin( 110 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );
*/
    _d->clearAllCells();
    _d->setCell( 0, 0,    37.5 );
    _d->setCell( 0, 1,     5.0 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,   -25.0 );
    _d->setCell( 0, 4,    20.0 );
    _d->setCell( 1, 0,    -9.0 );
    _d->setCell( 1, 2,   18.0 );
    _d->setCell( 1, 3,   -10.0 );

    _d->setCell( 2, 0,    25.5 );
    _d->setCell( 2, 1,    10.0 );
    _d->setCell( 2, 2,    20.0 );
    _d->setCell( 2, 3,  -  8.0 );
    _d->setCell( 2, 4,    44.0 );

    _d->setCell( 3, 0,    33.33 );
    _d->setCell( 3, 1,    15.0 );
    _d->setCell( 3, 2,  - 10.0 );
    _d->setCell( 3, 3,     7.5 );
    _d->setCell( 3, 4,    25.0 );

    _d->setCell( 4, 0,  -39.25 );
    _d->setCell( 4, 1,  36 );
    _d->setCell( 4, 2,  10 );
    _d->setCell( 4, 3,  -50 );
    _d->setCell( 4, 4,  0 );

    _p->setLegendPosition( KDChart1Params::LegendTopRightTop );

    /*
    QColor dddataValuesColor( Qt::darkBlue );
    QFont dataValuesFont( "courier", 1, QFont::Bold );
    _p->setThreeDBars( TRUE );
    _p->setAxisShowGrid( 0, FALSE );
    uint size=16;
    int rotation=0;
    _p->setPrintDataValues( true,
    KDCHART1_ALL_CHARTS,
    0,
    KDCHART1_DATA_VALUE_AUTO_DIGITS,
    &dataValuesFont,
    size,
    &dddataValuesColor,
    KDChart1Enums::PosBottomCenter,
    Qt::AlignTop    + Qt::AlignHCenter,
    0,
    20,
    rotation,
    KDChart1Enums::PosTopCenter,
    Qt::AlignBottom + Qt::AlignHCenter,
    0,
    -  25,
    rotation,
    KDChart1Enums::LayoutPolicyRotate );
    */
}


void KDChart1PresentationWidget::setConfiguration_1()
{
    // "bar: labels centered inside"
    resetData( 5, 5 );
    resetParams( KDChart1Params::Bar );
    _p->setThreeDBars( false );

    _p->setHeader1Text( "Bar chart");

    _p->setHeader2Text( "- normal view with user-defined text settings -" );

    _p->setFooterText(  "[ text font/size/color/position/rotation may be defined freely ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    int rotation = 270;
    QFont dataValuesFont( "courier", 1, QFont::Bold );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 15.0 ) : 15,
            KDCHART1_DATA_VALUE_AUTO_COLOR,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            rotation,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            rotation,
            KDChart1Enums::LayoutPolicyRotate );

    _d->clearAllCells();
    _d->setCell( 0, 0,    37.5 );
    _d->setCell( 0, 1,    12.0 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,   -25.0 );
    _d->setCell( 0, 4,    20.0 );
    _d->setCell( 1, 0,   -15.0 );
    _d->setCell( 1, 2,   18.0 );
    _d->setCell( 1, 3,   -14.0 );

    _d->setCell( 2, 0,    25.5 );
    _d->setCell( 2, 1,    10.0 );
    _d->setCell( 2, 2,    20.0 );
    _d->setCell( 2, 3,  - 13.0 );
    _d->setCell( 2, 4,    44.0 );

    _d->setCell( 3, 0,    33.33 );
    _d->setCell( 3, 1,    15.0 );
    _d->setCell( 3, 2,  - 10.0 );
    _d->setCell( 3, 3,     8.5 );
    _d->setCell( 3, 4,    25.0 );

    _d->setCell( 4, 0,  -39.25 );
    _d->setCell( 4, 1,  36 );
    _d->setCell( 4, 2,  10.5 );
    _d->setCell( 4, 3,  -50 );
    _d->setCell( 4, 4,  0 );

    _p->setLegendPosition( KDChart1Params::LegendTopLeftTop );
    _p->setLegendTitleText( "" );
}


void KDChart1PresentationWidget::setConfiguration_2()
{
    // "bar: 3d, multiple rows"

    resetParams( KDChart1Params::Bar );

    _p->setBarChartSubType( KDChart1Params::BarMultiRows );
    // this is set automatcally: _p->setThreeDBars( true );

    // use square bars
    _p->setThreeDBarDepth( 1.0 );

    // do not draw the bars' outline
    _p->setOutlineDataLineStyle( Qt::NoPen );


    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    pa.setAxisValueLeaveOut( KDCHART1_AXIS_LABELS_AUTO_LEAVEOUT );
    // pa.setAxisLabelsFontMinSize( 15 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );


    _p->setHeader1Text( "Bar chart");

    _p->setHeader2Text( "-  Three-dimensional view with bars in multiple rows  -" );

    _p->setFooterText(  "" );//[ text font/size/color/position/rotation may be defined freely ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    QFont  dataValuesFont( "helvetica", 1, QFont::Bold );
    //QColor dataValuesColor( Qt::darkBlue );

    /*
        int rotation = 0;
    _p->setPrintDataValues( true,
    KDCHART1_ALL_CHARTS,
    0,
    KDCHART1_DATA_VALUE_AUTO_DIGITS,
    &dataValuesFont,
    _size ? static_cast < uint > ( _size->value() / 10.0 * 13.0 ) : 13,
    &dataValuesColor,
    KDChart1Enums::PosBottomCenter,
    Qt::AlignTop    + Qt::AlignHCenter,
    0,
    20,
    rotation,
    KDChart1Enums::PosTopCenter,
    Qt::AlignBottom + Qt::AlignHCenter,
    0,
    -  25,
    rotation,
    KDChart1Enums::LayoutPolicyRotate );
    */

    _d->clearAllCells();
    /*
    _d->setCell( 0, 0,    37.5 );
    _d->setCell( 0, 1,     5.0 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,    25.0 );
    _d->setCell( 0, 4,    20.0 );

    _d->setCell( 1, 0,     9.0 );
    _d->setCell( 1, 2,   18.0 );
    _d->setCell( 1, 3,   - 9.75 );

    _d->setCell( 2, 0,    25.5 );
    _d->setCell( 2, 1,    10.0 );
    _d->setCell( 2, 2,    20.0 );
    _d->setCell( 2, 3,     8.0 );
    _d->setCell( 2, 4,    45.0 );

    _d->setCell( 3, 0,    33.33 );
    _d->setCell( 3, 1,    15.0 );
    _d->setCell( 3, 2,    10.0 );
    _d->setCell( 3, 3,     7.5 );
    _d->setCell( 3, 4,    25.0 );

    _d->setCell( 4, 0,   39.25 );
    _d->setCell( 4, 1,  36 );
    _d->setCell( 4, 2,  10 );
    _d->setCell( 4, 3,   50 );
    _d->setCell( 4, 4,  0 );
    */
/*
    for(int i=0; i<nRows; ++i) for(int j=0; j<10; ++j)
        _d->setCell( i, j,  6 );

    _d->setCell( 0, 0,  6 );
    _d->setCell( 0, 1,  5 );
    _d->setCell( 0, 2,  4 );
    _d->setCell( 0, 3,  5 );
    _d->setCell( 0, 4,  6 );
    _d->setCell( 1, 0,  5 );
    _d->setCell( 1, 1,  4 );
    _d->setCell( 1, 2,  3 );
    _d->setCell( 1, 3,  4 );
    _d->setCell( 1, 4,  6 );
*/
/*
    _d->setCell( 2, 0,  4 );
    _d->setCell( 2, 1,  3 );
    _d->setCell( 2, 2,  2 );
    _d->setCell( 2, 3,  3 );
    _d->setCell( 2, 4,  4 );

    _d->setCell( 2, 5,  5 );

    _d->setCell( 3, 0,  5 );
    _d->setCell( 3, 1,  4 );
    _d->setCell( 3, 2,  3 );
    _d->setCell( 3, 3,  4 );
    _d->setCell( 3, 4,  5 );
    _d->setCell( 4, 0,  6 );
    _d->setCell( 4, 1,  5 );
    _d->setCell( 4, 2,  4 );
    _d->setCell( 4, 3,  5 );
    _d->setCell( 4, 4,  6 );

    _d->setCell( 5, 2,  5 );

    _d->setCell( 4, 7,  7 );

    _d->setCell( 5, 5,  6 );
    _d->setCell( 5, 6,  7 );
    _d->setCell( 5, 7,  8 );
    _d->setCell( 5, 8,  7 );
    _d->setCell( 5, 9,  6 );
    _d->setCell( 6, 5,  7 );
    _d->setCell( 6, 6,  8 );
    _d->setCell( 6, 7,  9 );
    _d->setCell( 6, 8,  8 );
    _d->setCell( 6, 9,  7 );

    _d->setCell( 7, 4,  7 );

    _d->setCell( 7, 5,  8 );
    _d->setCell( 7, 6,  9 );
    _d->setCell( 7, 7, 10 );
    _d->setCell( 7, 8,  9 );
    _d->setCell( 7, 9,  8 );
    _d->setCell( 8, 5,  7 );
    _d->setCell( 8, 6,  8 );
    _d->setCell( 8, 7,  9 );
    _d->setCell( 8, 8,  8 );
    _d->setCell( 8, 9,  7 );
    _d->setCell( 9, 5,  6 );
    _d->setCell( 9, 6,  7 );
    _d->setCell( 9, 7,  8 );
    _d->setCell( 9, 8,  7 );
    _d->setCell( 9, 9,  6 );
*/



    // we define a simple topographic gradient
    static const int NUM_TOPOCOLORS = 20;
    static const QColor TOPOCOLORS[ NUM_TOPOCOLORS ] = {
        QColor(    0,   0,0xb2 ),
        QColor(    0,   0,0xe5 ),
        QColor(    0,0x33,0xff ),
        QColor(    0,0x66,0xff ),
        QColor(    0,0x99,0xff ),
        QColor(    0,0xcc,0xff ),
        QColor(    0,0xff,0xff ),
        QColor( 0x33,0xff,0xff ),
        QColor( 0x66,0xff,0xcc ),
        QColor( 0x99,0xff,0x99 ),
        QColor( 0xcc,0xff,0x66 ),
        QColor( 0xff,0xff,0x33 ),
        QColor( 0xff,0xff,   0 ),
        QColor( 0xff,0xcc,   0 ),
        QColor( 0xff,0x99,   0 ),
        QColor( 0xff,0x66,   0 ),
        QColor( 0xff,0x33,   0 ),
        QColor( 0xe5,   0,   0 ),
        QColor( 0xb2,   0,   0 ),
        QColor( 0x7f,   0,   0 ),
    };
    int propSetId[ NUM_TOPOCOLORS ];
    KDChart1PropertySet propSet;
    for( int i = 0;  i < NUM_TOPOCOLORS;  ++i ){
        propSet.setBarColor( KDChart1PropertySet::OwnID, TOPOCOLORS[ i ] );
        propSetId[ i ] = _p->registerProperties( propSet );
    }




    double minValue = DBL_MAX;
    double maxValue = DBL_MIN;

    const double stepT = 0.1;

    const double minTX =  1.4;
    const double maxTX = 11.01;
    const unsigned int numCellsX = static_cast < unsigned int > ((maxTX-minTX) / stepT) + 1;

    const double minTZ = 2.9;
    const double maxTZ = 8.49;
    const unsigned int numCellsZ = static_cast < unsigned int > ((maxTZ-minTZ) / stepT) + 1;

    unsigned int iRow;  // counter for datasets
    unsigned int iCol;  // counter for items (within their resp. datasets)


    //const int nRows=2;
    resetData( numCellsZ, numCellsX );


    for( iRow = 0;  iRow < numCellsZ;  ++iRow )
        for( iCol = 0; iCol < numCellsX; ++iCol )
            _d->setCell( iRow, iCol,  0.0 );
    iRow = 0;
    for( double tZ = minTZ;  tZ <= maxTZ;  tZ += stepT ){
        iCol = 0;
        for(double tX = minTX;  tX <= maxTX;  tX += stepT ){
            double yValue = 0.4 + (sin( tZ ) + sin( tX )) / 4.0 * (iCol+iRow)/10;
            _d->setCell( iRow, iCol,  yValue, tX );
            minValue = qMin(minValue, yValue);
            maxValue = qMax(maxValue, yValue);
            ++iCol;
        }
        ++iRow;
    }

    double rangeD20 = (maxValue - minValue) / (NUM_TOPOCOLORS-1);
    for( iRow = 0;  iRow < numCellsZ;  ++iRow ){
        for( iCol = 0; iCol < numCellsX; ++iCol ){
            QVariant valueY;
            if( _d->cellCoord( iRow, iCol, valueY ) &&
                QVariant::Double == valueY.type() ){
                const int id = static_cast < int > ((valueY.toDouble() - minValue) / rangeD20);
                _d->setProp( iRow, iCol, propSetId[ id ] );
            }
        }
    }



    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_3()
{
    // "line: log. ordinate"
    resetData( 4, 5 );

    resetParams( KDChart1Params::Line );
    _p->setLineChartSubType( KDChart1Params::LineNormal );
    _p->setLineWidth( 1 );
    _p->setLineMarker( true );
    _p->setLineMarkerSize( QSize( 9, 9 ) );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLogarithmic );
    //pa.setAxisValueStart(  0.0 );
    //pa.setAxisValueEnd( 1000.0 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    _p->setHeader1Text( "Line chart");

    _p->setHeader2Text( "- logarithmic ordinate axis -" );

    _p->setFooterText(  "[ marker size/color may be defined freely ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,     0.39065 );
    _d->setCell( 0, 1,     0.78125 );
    _d->setCell( 0, 2,     1.5625  );
    _d->setCell( 0, 3,     3.125   );
    _d->setCell( 0, 4,     6.25    );

    _d->setCell( 1, 0,     6.25    );
    _d->setCell( 1, 1,    12.5  );
    _d->setCell( 1, 2,    25.0  );
    _d->setCell( 1, 3,    50.0  );
    _d->setCell( 1, 4,   100.0  );

    _d->setCell( 2, 0,   100.0  );
    _d->setCell( 2, 1,   200.0  );
    _d->setCell( 2, 2,   400.0  );
    _d->setCell( 2, 3,   800.0  );
    _d->setCell( 2, 4,  1600.0  );

    _d->setCell( 3, 0,  1600.0  );
    _d->setCell( 3, 1,  3200.0  );
    _d->setCell( 3, 2,  6400.0  );
    _d->setCell( 3, 3, 12800.0  );
    _d->setCell( 3, 4, 25600.0  );

    _p->setLegendPosition( KDChart1Params::NoLegend );
}

/*
void KDChart1PresentationWidget::setConfiguration_4()
{
    // "line: real life example"
    const int maxDatasets = 8;
    const int maxValues = 3000;
    resetData( maxDatasets+1, maxValues );
    resetParams( KDChart1Params::Line );
    _p->setLineChartSubType( KDChart1Params::LineNormal );

    QFont  dataValuesFont(   "times", 1, QFont::Bold );
    QColor dataValuesColor( Qt::darkBlue );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            18,
            &dataValuesColor,
            KDChart1Enums::PosBottomRight,
            Qt::AlignBottom + Qt::AlignRight,
            0,
            65,
            318,
            KDChart1Enums::PosTopLeft,
            Qt::AlignTop    + Qt::AlignLeft,
            0,
            -  75,
            300,
            KDChart1Enums::LayoutPolicyRotate );

    const QColor gridColor( Qt::darkGray.dark(175) );
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    pa.setAxisGridColor( gridColor );
    pa.setAxisGridSubColor( gridColor );
    pa.setAxisGridSubStyle( Qt::SolidLine );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisGridColor( gridColor );
    pa.setAxisGridSubColor( gridColor );
    pa.setAxisGridSubStyle( Qt::SolidLine );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );

    QColor myBlue(10,95,137);
    _p->setSimpleFrame( KDChart1Enums::AreaInnermost,
            0,0,  6,6,
            true,
            true,
            KDFrame1::FrameFlat,
            1,
            0,
            QPen( QColor() ),
            QBrush( myBlue ) );
    _p->setSimpleFrame( KDChart1Enums::AreaData,
            0,0,  0,0,
            true,
            true,
            KDFrame1::FrameFlat,
            1,
            0,
            Qt::NoPen,
            QBrush( Qt::darkGray.dark(250) ) );
    const QBrush legendBrush( Qt::darkGray );
    _p->setSimpleFrame( KDChart1Enums::AreaLegend,
            0,0,  0,0,
            true,
            true,
            KDFrame1::FrameFlat,
            1,
            0,
            Qt::NoPen,
            legendBrush );

#ifdef QWS
    _p->setLegendPosition( KDChart1Params::NoLegend );
#else
    _p->setLegendPosition( KDChart1Params::LegendTopRightRight );
    _p->setLegendSource( KDChart1Params::LegendManual );
    _p->setLegendText( 0, QString::fromUtf8("Schöna") );
    _p->setLegendText( 1, "Dresden" );
    _p->setLegendText( 2, "Torgau" );
    _p->setLegendText( 3, "Wittenberg" );
    _p->setLegendText( 4, "Dessau" );
    _p->setLegendText( 5, "Aken" );
    _p->setLegendText( 6, "Barby" );
    _p->setLegendText( 7, "Magdeburg" );
#endif
    _p->setDataRainbowColors();

    _p->setPrintDataValues( false );
    _p->setLineColor( Qt::darkGray );

    _p->setLineMarker ( true );

#ifdef QWS
    _p->setHeader1Text( "German Elbe in 2002" );
#else
    _p->setHeader1Text( "German upper Elbe gauges in summer 2002" );
#endif
    _p->setHeader2Text( "" );
    _p->setFooterText( "" );


    // prepare the chart widget for text lines output
    _d->setCell( 0, 0,  130 );
    _d->setCell( 0, 1, 1300 );
    _d->setCell( 0, 2,  280 );
    _d->setUsedCols( 3 );
    const bool bShowProgress = (1 >= _argc) ||
               QString("dontShowProgress").upper() != QString(_argv[1]).upper();
    if( bShowProgress )
        _kdc->repaint( false );
    const QFont oldKDCFont( _kdc->font() );
    _kdc->setFont(QFont( "courier", 12, QFont::Bold ));
#if COMPAT_QT_VERSION > 0x029999
    const QColor oldKDCColor( _kdc->paletteForegroundColor() );
    _kdc->setPaletteForegroundColor(Qt::cyan);
#endif
    QDateTime firstDt( QDate(2002, 8,  1), QTime( 0,  0    ));
    QDateTime lastDt(  QDate(2002, 8, 30), QTime(23, 59, 59));

    int trueUsedValues = 0;

    for(int r=0; r<maxDatasets; ++r){
        _p->setLineMarkerStyle( r, KDChart1Params::LineMarker4Pixels );
        QString gauge(_p->legendText(r));

        QString s = QString("data/gauge%1.dat").arg( 100+r );
        if( bShowProgress ){
            _kdc->drawText( 5,
                            14,
                            gauge+": reading gauge data from file "+s );
            _kdc->repaint( false );
        }

        QFile f(s);
        if ( f.open(IO_ReadOnly) ){
            QTextStream t( &f );
            QString s, year, month, day, hour, min, value;
            int lastMInt = 0;
            int c = 0;
            bool bGoOn = true;
            while ( !t.eof() && bGoOn ){
                value = -9999.0;
                s = t.readLine().simplified();
                //qDebug(s);
                int x0=0;
                int x=s.indexOf(' ');
                if( -1 < x ){
                    year = s.mid(x0,x-x0);
                    x0=x+1;
                    x=s.indexOf(' ',x0);
                    if( -1 < x ){
                        month = s.mid(x0,x-x0);
                        x0=x+1;
                        x=s.indexOf(' ',x0);
                        if( -1 < x ){
                            day = s.mid(x0,x-x0);
                            x0=x+1;
                            x=s.indexOf(' ',x0);
                            if( -1 < x ){
                                hour = s.mid(x0,x-x0);
                                x0=x+1;
                                x=s.indexOf(' ',x0);
                                if( -1 < x ){
                                    min = s.mid(x0,x-x0);
                                    value = s.mid(x+1,20);
                                }
                            }
                        }
                    }
                }
                if( !(value.isEmpty() || "-9999.0" == value) ){
                    int valInt = value.toInt();
                    if( 0 < valInt && 1500 > valInt){
                        int mInt = month.toInt();
                        if( mInt != lastMInt ){
                            //qDebug("gauge %i/4:   year "+year+"   month "+month+
                            //    "   day "+day+"   hour "+hour+"   min "+min+
                            //    "   value "+value,
                            //    r+1);
                            lastMInt = mInt;
                        }
                        QDateTime dt( QDate(year.toInt(),
                                    mInt,
                                    day.toInt()),
                                QTime(hour.toInt(),
                                    min.toInt()) );
                        if( dt >= firstDt && dt <= lastDt ){
                            _d->setCell( r, c,
                                     value.toInt(), dt );
                            ++c;
                        }
                        bGoOn = dt < lastDt;
                    }
                }
            }
            f.close();

            // This increases calculation speed dramatically
            // if only a small part of the cells are shown.
            trueUsedValues = qMax(trueUsedValues, c);
            _d->setUsedCols( trueUsedValues );

            if( bShowProgress ){
                _kdc->drawText( 5,
                                14,
                                gauge+": "+QString::number(c)+" data imported" );
                _kdc->repaint( false );
            }
        }else{
            _kdc->drawText( 5,
                    14,
                    "Error: cannot open file "+s );
            _kdc->repaint( false );
        }
    }

    // attach an extra horizontal line:
    const QColor myColor( _p->dataColor( 2 ) ); // Torgau line color
    KDChart1PropertySet horiLinePropsA;
    horiLinePropsA.setName(
            "horizontal line at 2.20m level");
    horiLinePropsA.setLineStyle(KDChart1PropertySet::OwnID, Qt::NoPen);
    horiLinePropsA.setShowMarker(
            KDChart1PropertySet::OwnID,
            false );
    horiLinePropsA.setExtraLinesAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignLeft | Qt::AlignRight );
    horiLinePropsA.setExtraLinesWidth(
            KDChart1PropertySet::OwnID,
            -2 );
    horiLinePropsA.setExtraLinesColor(
            KDChart1PropertySet::OwnID,
            myColor );
    horiLinePropsA.setExtraLinesStyle(
            KDChart1PropertySet::OwnID,
            Qt::DotLine );
    horiLinePropsA.setExtraMarkersAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignLeft | AlignRight );
    horiLinePropsA.setExtraMarkersSize(
            KDChart1PropertySet::OwnID,
            QSize(-10, -10) );
    horiLinePropsA.setExtraMarkersColor(
            KDChart1PropertySet::OwnID,
            myColor );
    horiLinePropsA.setExtraMarkersStyle(
            KDChart1PropertySet::OwnID,
            KDChart1Params::LineMarkerCircle );
    int idHoriLinePropsA
        = _p->registerProperties( horiLinePropsA );
    _d->setCell( maxDatasets, 0,

                220.0,
                QDateTime(QDate(2002, 8, 15), QTime( 12, 0 )) ) );
    _d->setProp( maxDatasets, 0,    idHoriLinePropsA );
    // attach another extra horizontal line:
    KDChart1PropertySet horiLinePropsB;
    horiLinePropsB.setName(
            "horizontal line at 5.80m level");
    horiLinePropsB.setLineStyle(KDChart1PropertySet::OwnID, Qt::NoPen);
    horiLinePropsB.setShowMarker(
            KDChart1PropertySet::OwnID,
            false );
    horiLinePropsB.setExtraLinesAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignLeft | Qt::AlignRight );
    horiLinePropsB.setExtraLinesInFront(
            KDChart1PropertySet::OwnID,
            true );
    horiLinePropsB.setExtraLinesWidth(
            KDChart1PropertySet::OwnID,
            -2 );
    horiLinePropsB.setExtraLinesColor(
            KDChart1PropertySet::OwnID,
            myColor );
    horiLinePropsB.setExtraLinesStyle(
            KDChart1PropertySet::OwnID,
            Qt::SolidLine );
    horiLinePropsB.setExtraMarkersAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignLeft | AlignRight );
    horiLinePropsB.setExtraMarkersSize(
            KDChart1PropertySet::OwnID,
            QSize(-10, -10) );
    horiLinePropsB.setExtraMarkersColor(
            KDChart1PropertySet::OwnID,
            myColor );
    horiLinePropsB.setExtraMarkersStyle(
            KDChart1PropertySet::OwnID,
            KDChart1Params::LineMarkerCircle );
    int idHoriLinePropsB
        = _p->registerProperties( horiLinePropsB );
    _d->setCell( maxDatasets, 1,

                580.0,
                QDateTime(QDate(2002, 8, 15), QTime( 13, 0 )) );
    _d->setProp( maxDatasets, 1,    idHoriLinePropsB );
    // adjust the respective dataset's colour
    _p->setDataColor( maxDatasets, myColor );
    // adjust the legend text
    _p->setLegendText( maxDatasets, "" );

#ifndef QWS
    _p->insertCustomBox(
            KDChart1CustomBox(
                KDChart1TextPiece( "<qt><p><b>Torgau</b><br>alarm level: ___<br>normal level: ......</p></qt>",
                    QFont( "helvetica", 1, QFont::Normal, false ) ),
                -22,true,
                0,-200,
                -700,-200,
                myColor,
                Qt::NoBrush,
                KDChart1Enums::AreaLegend,
                KDChart1Enums::PosBottomLeft,
                Qt::AlignTop + Qt::AlignLeft ) );
#endif

    // restore old widget settings
    _kdc->setFont(oldKDCFont);
#if COMPAT_QT_VERSION > 0x029999
    _kdc->setPaletteForegroundColor(oldKDCColor);
#endif
}
*/

void KDChart1PresentationWidget::setConfiguration_4()
{
    // "line: 3d"
    resetData( 5, 5 );
    resetParams( KDChart1Params::Line );
    _p->setThreeDLines( true );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    _p->setHeader1Text( "Line chart");

    _p->setHeader2Text( "- Three Dimensional view with user-defined text settings -" );

    _p->setFooterText(  "[ text font/size/color/position/rotation may be defined freely ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    int rotation = 0;
    QFont  dataValuesFont( "helvetica", 1, QFont::Bold );
    QColor dataValuesColor( Qt::darkBlue );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 13.0 ) : 13,
            &dataValuesColor,
            KDChart1Enums::PosBottomCenter,
            Qt::AlignTop    + Qt::AlignHCenter,
            0,
            20,
            rotation,
            KDChart1Enums::PosTopCenter,
            Qt::AlignBottom + Qt::AlignHCenter,
            0,
            -  25,
            rotation,
            KDChart1Enums::LayoutPolicyRotate );

    _d->clearAllCells();
    _d->setCell( 0, 0,    37.5 );
    _d->setCell( 0, 1,     5.0 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,    25.0 );
    _d->setCell( 0, 4,    20.0 );

    _d->setCell( 1, 0,     9.0 );
    _d->setCell( 1, 2,   18.0 );
    _d->setCell( 1, 3,   - 9.75 );

    _d->setCell( 2, 0,    25.5 );
    _d->setCell( 2, 1,    10.0 );
    _d->setCell( 2, 2,    20.0 );
    _d->setCell( 2, 3,     8.0 );
    _d->setCell( 2, 4,    45.0 );

    _d->setCell( 3, 0,    33.33 );
    _d->setCell( 3, 1,    15.0 );
    _d->setCell( 3, 2,    10.0 );
    _d->setCell( 3, 3,     7.5 );
    _d->setCell( 3, 4,    25.0 );

    _d->setCell( 4, 0,   39.25 );
    _d->setCell( 4, 1,  36 );
    _d->setCell( 4, 2,  10 );
    _d->setCell( 4, 3,   50 );
    _d->setCell( 4, 4,  0 );

    _p->setLegendPosition( KDChart1Params::LegendBottomLeftBottom );
}


// temporarily disabled:
/*
void KDChart1PresentationWidget::setConfiguration_5()
{
    // "line: 3d, rotated"
    resetData( 5, 5 );
    resetParams( KDChart1Params::Line );
    _p->setThreeDLines( true );
    _p->setThreeDLineXRotation( 30 );
    _p->setThreeDLineYRotation( 15 );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    _p->setHeader1Text( "Line chart");

    _p->setHeader2Text( "- Three Dimensional view with user-defined text settings,\n rotated by 30 degrees in X orientation and 15 degrees in Y orientation -" );

    _p->setFooterText(  "[ text font/size/color/position/rotation may be defined freely ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
    QFont( "times", 24, QFont::Bold, true ),
    true,
    40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
    QFont( "helvetica", 24, QFont::Bold, false ),
    true,
    18 );

    int rotation = 0;
    QFont  dataValuesFont( "helvetica", 1, QFont::Bold );
    QColor dataValuesColor( Qt::darkBlue );
    _p->setPrintDataValues( true,
    KDCHART1_ALL_CHARTS,
    0,
    KDCHART1_DATA_VALUE_AUTO_DIGITS,
    &dataValuesFont,
    _size ? static_cast < uint > ( _size->value() / 10.0 * 13.0 ) : 13,
    &dataValuesColor,
    KDChart1Enums::PosBottomCenter,
    Qt::AlignTop    + Qt::AlignHCenter,
    0,
    20,
    rotation,
    KDChart1Enums::PosTopCenter,
    Qt::AlignBottom + Qt::AlignHCenter,
    0,
    -  25,
    rotation,
    KDChart1Enums::LayoutPolicyRotate );

    _d->clearAllCells();
    _d->setCell( 0, 0,    37.5 );
    _d->setCell( 0, 1,     5.0 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,    25.0 );
    _d->setCell( 0, 4,    20.0 );

    _d->setCell( 1, 0,     9.0 );
    _d->setCell( 1, 2,   18.0 );
    _d->setCell( 1, 3,   - 9.75 );

    _d->setCell( 2, 0,    25.5 );
    _d->setCell( 2, 1,    10.0 );
    _d->setCell( 2, 2,    20.0 );
    _d->setCell( 2, 3,     8.0 );
    _d->setCell( 2, 4,    45.0 );

    _d->setCell( 3, 0,    33.33 );
    _d->setCell( 3, 1,    15.0 );
    _d->setCell( 3, 2,    10.0 );
    _d->setCell( 3, 3,     7.5 );
    _d->setCell( 3, 4,    25.0 );

    _d->setCell( 4, 0,   39.25 );
    _d->setCell( 4, 1,  36 );
    _d->setCell( 4, 2,  10 );
    _d->setCell( 4, 3,   50 );
    _d->setCell( 4, 4,  0 );

    _p->setLegendPosition( KDChart1Params::LegendBottomLeftBottom );
}
*/


void KDChart1PresentationWidget::setConfiguration_5()
{
    // "line: reverted ordinate axis"
    resetData( 5, 5 );
    resetParams( KDChart1Params::Line );
    _p->setThreeDLines( false );
    _p->setLineWidth( 3 );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    pa.setAxisValuesDecreasing( true );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    _p->setHeader1Text( "Line chart");

    _p->setHeader2Text( "- Reverted Ordinate Axis -" );

    _p->setFooterText(  "[ charts with specific X values may also have reverted abscissa axis ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    int rotation = 0;
    QFont  dataValuesFont( "helvetica", 1, QFont::Bold );
    QColor dataValuesColor( Qt::darkBlue );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 13.0 ) : 13,
            &dataValuesColor,
            KDChart1Enums::PosBottomCenter,
            Qt::AlignTop    + Qt::AlignHCenter,
            0,
            20,
            rotation,
            KDChart1Enums::PosTopCenter,
            Qt::AlignBottom + Qt::AlignHCenter,
            0,
            -  25,
            rotation,
            KDChart1Enums::LayoutPolicyRotate );

    _d->clearAllCells();
    _d->setCell( 0, 0,    37.5 );
    _d->setCell( 0, 1,     5.0 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,    25.0 );
    _d->setCell( 0, 4,    20.0 );

    _d->setCell( 1, 0,     9.0 );
    _d->setCell( 1, 2,   18.0 );
    _d->setCell( 1, 3,   - 9.75 );

    _d->setCell( 2, 0,    25.5 );
    _d->setCell( 2, 1,    10.0 );
    _d->setCell( 2, 2,    20.0 );
    _d->setCell( 2, 3,     8.0 );
    _d->setCell( 2, 4,    45.0 );

    _d->setCell( 3, 0,    33.33 );
    _d->setCell( 3, 1,    15.0 );
    _d->setCell( 3, 2,    10.0 );
    _d->setCell( 3, 3,     7.5 );
    _d->setCell( 3, 4,    25.0 );

    _d->setCell( 4, 0,   39.25 );
    _d->setCell( 4, 1,  36 );
    _d->setCell( 4, 2,  10 );
    _d->setCell( 4, 3,   50 );
    _d->setCell( 4, 4,  0 );

    _p->setLegendPosition( KDChart1Params::LegendBottomLeftBottom );
}


void KDChart1PresentationWidget::setConfiguration_6()
{
    // "bars and lines in one chart"
    resetData( 6, 7 );
    resetParams( KDChart1Params::Bar,
            KDChart1Params::Line );
    _p->setBarChartSubType( KDChart1Params::BarNormal );
    _p->setLineChartSubType( KDChart1Params::LineNormal );

    /*
    left axis #1:
    */
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisUseAvailableSpace( 0, -469 );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( 0 ); // thin grid lines
    pa.setAxisLineColor( Qt::blue );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    /**/
    //pa.setAxisValueStart(  0.0 );
    //pa.setAxisValueEnd(   10.0 );
    //pa.setAxisValueDelta(  1.0 );
    /**/
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    /*
    right axis #1:
    */
    pa = _p->axisParams( KDChart1AxisParams::AxisPosRight );
    pa.setAxisUseAvailableSpace( 0, -469 );
    pa.setAxisGridStyle( Qt::DotLine );
    pa.setAxisGridLineWidth( 0 ); // thin grid lines
    pa.setAxisLineColor( Qt::black );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    /**/
    //pa.setAxisValueStart(-10.0 );
    //pa.setAxisValueEnd(    4.0 );
    //pa.setAxisValueDelta(  1.0 );
    /**/
    _p->setAxisParams( KDChart1AxisParams::AxisPosRight, pa );

    /*
    left axis #2:
    */
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft2 );
    pa.setAxisUseAvailableSpace( -530, -1000 );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( 0 );
    pa.setAxisLineColor( Qt::red );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( true );
    /**/
    //pa.setAxisValueStart(-10.0 );
    //pa.setAxisValueEnd(    4.0 );
    //pa.setAxisValueDelta(  1.0 );
    /**/
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft2, pa );
    /**/
    _p->setDataColor( 0, QColor(  40, 60, 80 ));
    _p->setDataColor( 1, QColor( 110,140,168));
    _p->setDataColor( 2, QColor( 180,218,255));
    _p->setDataColor( 3, Qt::white);
    _p->setDataColor( 4, QColor( 160, 80, 80));
    _p->setDataColor( 5, QColor( 255, 80, 80));
    /* */


    static QStringList abscissaNames;
    abscissaNames << "Sunday" << "Monday" << "Tuesday" << "Wednesday"
        << "Thursday" << "Friday" << "Saturday";
    static QStringList abscissaShortNames;
    abscissaShortNames << "Sun" << "Mon" << "Tue" << "Wed"
        << "Thu" << "Fri" << "Sat";

    _p->setAxisLabelStringParams( KDChart1AxisParams::AxisPosBottom,
            &abscissaNames,
            &abscissaShortNames,
            "Monday",
            "Friday" );
    /*
    _p->setAxisLabelTextParams( KDChart1AxisParams::AxisPosBottom,
    false,
     "Monday" ),
     "Friday" ),
    1.0,
    KDCHART1_AXIS_LABELS_AUTO_DIGITS,
    KDCHART1_AXIS_IGNORE_EMPTY_INNER_SPAN,
    KDChart1AxisParams::LabelsFromDataRowNo, 0,
    &abscissaNames,
    &abscissaShortNames );
    */

    // modify the legend texts to show the world what we have done :-)
    _p->setLegendSource( KDChart1Params::LegendManual );
    _p->setLegendText( 0, "France" );
    _p->setLegendText( 1, "Germany" );
    _p->setLegendText( 2, "Luxembourg" );
    _p->setLegendText( 3, "Fr + Ge + Lu" );

    _p->setLegendText( 4, "Adam Riese" );
    _p->setLegendText( 5, "Ottokar Winzig" );

    _p->setLegendTitleText( "Countries<br>&amp; People" );




    _p->setChartSourceMode( KDChart1Params::DataEntry,     0, 3,   0 );
    _p->setChartSourceMode( KDChart1Params::DataEntry,     4, 5,   1 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosLeft,  0, 2,   0 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosRight, 3, 3,   0 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosLeft2, 4, 5,   1 );
    _p->setHeader1Text( "Combination of two different charts" );

    _p->setHeader2Text( "- same abscissa axis for both charts -" );

    _p->setFooterText(  "Use similar colors for the ordinate lines and grid lines and their respective data.");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );

    QFont dataValuesFont( "helvetica", 1, QFont::Bold );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 13.0 ) : 13,
            0,
            KDChart1Enums::PosBottomCenter,
            Qt::AlignTop | Qt::AlignHCenter,
            0,
            0,
            0,
            KDChart1Enums::PosTopCenter,
            Qt::AlignBottom | Qt::AlignHCenter,
            0,
            0,
            0,
            KDChart1Enums::LayoutPolicyShrinkFontSize );

    QColor textColor0( Qt::darkBlue );
    QColor textColor1( Qt::darkRed );
    _p->setPrintDataValuesColor( 0, &textColor0 );
    _p->setPrintDataValuesColor( 1, &textColor1 );


    _d->clearAllCells();
    _d->setCell( 0, 0,  2.0 );
    _d->setCell( 0, 1,  4.5 );
    _d->setCell( 0, 2,  4.0 );
    _d->setCell( 0, 3,  3.0 );
    _d->setCell( 0, 4,  2.0 );
    _d->setCell( 0, 5,  3.05 );
    _d->setCell( 0, 6,  5.25 );
    //--
    _d->setCell( 1, 0,  4.0 );
    _d->setCell( 1, 1,  2.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  3.0 );
    _d->setCell( 1, 5,  5.1 );
    _d->setCell( 1, 6,  2.32 );
    //_______
    _d->setCell( 2, 0,  0.6 );
    _d->setCell( 2, 1,  0.125 );
    _d->setCell( 2, 2,  0.15 );
    _d->setCell( 2, 3,  0.3 );
    _d->setCell( 2, 4,  0.25 );
    _d->setCell( 2, 5,  0.45 );
    _d->setCell( 2, 6,  1.25 );
    //------
    _d->setCell( 3, 0,  6.6 );
    _d->setCell( 3, 1,  6.625 );
    _d->setCell( 3, 2,  7.15 );
    _d->setCell( 3, 3,  5.3 );
    _d->setCell( 3, 4,  5.25 );
    _d->setCell( 3, 5,  7.05 );
    _d->setCell( 3, 6,  4.25 );
    //-------
    _d->setCell( 4, 0,  2.0 );
    _d->setCell( 4, 1,   0.1 );
    _d->setCell( 4, 2,  3.0 );
    _d->setCell( 4, 3,  1.4 );
    _d->setCell( 4, 4,  2.0 );
    _d->setCell( 4, 5,  3.5 );
    _d->setCell( 4, 6,  1.55 );
    //-----------
    _d->setCell( 5, 0,  -0.9 );
    _d->setCell( 5, 1,  -1.2 );
    _d->setCell( 5, 2,  -0.3 );
    _d->setCell( 5, 3,  0.0 );
    _d->setCell( 5, 4,  1.2 );
    _d->setCell( 5, 5,  3.0 );
    _d->setCell( 5, 6,  1.95 );
    //--------------

    _p->setLegendPosition( KDChart1Params::LegendTopLeftLeft );
}


void KDChart1PresentationWidget::setConfiguration_7()
{
    // "point: circular markers",
    const uint nDatasets = 9;
    resetData( nDatasets, 3 );
    resetParams( KDChart1Params::Line );
    _p->setLineStyle( Qt::NoPen );
    _p->setLineMarker( true );
    _p->setLineMarkerSize( QSize( 18, 18 ) );
    for( uint i=0; i<nDatasets; ++i )
      _p->setLineMarkerStyle( i, KDChart1Params::LineMarkerRing );

    // make yellow visible on our background
    _p->setDataColor( 5, QColor(0xB0,0x80,0x20) );

    _p->setHeader1Text( "Point chart");

    _p->setHeader2Text( "- take a Line chart: leave out connecting lines, add markers -" );

    _p->setFooterText(  "[ markers have user definable sizes/colors/shapes ]");
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter2,
        "[ setLegendOrientation( Qt::Horizontal ) makes the legend grow in horizontal direction ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "times", 24, QFont::Bold, false ),
            true,
            27 );

    //
    //left axis:
    //
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    pa.setAxisLineColor( Qt::blue );
    pa.setAxisGridColor( __chamois.dark(125) );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    //
    //bottom axis:
    //
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisLineColor( Qt::blue );
    //pa.setAxisGridColor( __chamois.dark(125) );
    pa.setAxisShowGrid( false );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );

    // most of the frequently used axis settings may also
    // be specilied by KDChart1Params convenience function like this one:
    _p->setAxisLabelsFont( KDChart1AxisParams::AxisPosLeft,
                           QFont( "helvetica", 1, QFont::Normal ),
                           -12,
                           Qt::black );
    _p->setAxisLabelsFont( KDChart1AxisParams::AxisPosBottom,
                           QFont( "helvetica", 1, QFont::Normal ),
                           -12,
                           Qt::black );


    int rotation = 0;
    QFont  dataValuesFont( "helvetica", 1, QFont::Bold );
    QColor dataValuesColor( Qt::darkBlue );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 13.0 ) : 13,
            &dataValuesColor,
            KDChart1Enums::PosBottomCenter,
            Qt::AlignTop    + Qt::AlignHCenter,
            30,  // x shift
            10,
            rotation,
            KDChart1Enums::PosTopCenter,
            Qt::AlignBottom + Qt::AlignHCenter,
            30,  // x shift
            - 10,
            rotation,
            KDChart1Enums::LayoutPolicyRotate );
/*
    KDChart1PropertySet myNewNormalProps;
    // retrieve the property set specified for normal data
    _p->properties( KDCHART1_PROPSET_NORMAL_DATA,
            myNewNormalProps );
    myNewNormalProps.setLineStyle(
            KDChart1PropertySet::OwnID,
            Qt::NoPen );
    myNewNormalProps.setShowMarker(
            KDChart1PropertySet::OwnID,
            true );
    myNewNormalProps.setExtraLinesAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignBottom );
    myNewNormalProps.setExtraLinesStyle(
            KDChart1PropertySet::OwnID,
            Qt::DotLine );
    myNewNormalProps.setExtraMarkersAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignTop );
    myNewNormalProps.setExtraMarkersSize(
            KDChart1PropertySet::OwnID,
            QSize(-67, -67) );
    myNewNormalProps.setExtraMarkersColor(
            KDChart1PropertySet::OwnID,
            Qt::black );
    myNewNormalProps.setExtraMarkersStyle(
            KDChart1PropertySet::OwnID,
            KDChart1Params::LineMarkerCircle );
    // declare our new property set to be used for normal data
    _p->setProperties( KDCHART1_PROPSET_NORMAL_DATA,
            myNewNormalProps );
*/
    _d->clearAllCells();
    _d->setCell( 0, 0,    17.5,  QDateTime(QDate(2004,12,1), QTime(15,58,45,600) ) );
    _d->setCell( 1, 0,     5.0,  QDateTime(QDate(2004,12,1), QTime(16, 1,10,100) ) );
    _d->setCell( 2, 0,    10.0,  QDateTime(QDate(2004,12,1), QTime(15,59,20,300) ) );
    _d->setCell( 3, 0,    25.0,  QDateTime(QDate(2004,12,1), QTime(16, 6,30,700) ) );
    _d->setCell( 4, 0,    20.0,  QDateTime(QDate(2004,12,1), QTime(16, 2,35,800) ) );
    _d->setCell( 5, 0,    21.0,  QDateTime(QDate(2004,12,1), QTime(16, 7,32,900) ) );
    _d->setCell( 0, 1,    -1.75, QDateTime(QDate(2004,12,1), QTime(16, 5, 3,200) ) );
    _d->setCell( 1, 1,    14.0,  QDateTime(QDate(2004,12,1), QTime(16, 4,23,100) ) );
    _d->setCell( 2, 1,     9.75, QDateTime(QDate(2004,12,1), QTime(16, 1,33,400) ) );
    _d->setCell( 4, 1,    17.0,  QDateTime(QDate(2004,12,1), QTime(16, 9,37,700) ) );
    _d->setCell( 5, 1,     7.0,  QDateTime(QDate(2004,12,1), QTime(16,10,40,900) ) );
    _d->setCell( 0, 2,    25.5,  QDateTime(QDate(2004,12,1), QTime(16, 2, 5,300) ) );
    _d->setCell( 1, 2,    10.0,  QDateTime(QDate(2004,12,1), QTime(16, 1,15,200) ) );
    _d->setCell( 2, 2,    20.0,  QDateTime(QDate(2004,12,1), QTime(16, 6,25,400) ) );
    _d->setCell( 3, 2,     8.0,  QDateTime(QDate(2004,12,1), QTime(16, 8,35,300) ) );
    _d->setCell( 4, 2,    15.0,  QDateTime(QDate(2004,12,1), QTime(16, 9,45,300) ) );
    _d->setCell( 5, 2,    16.0,  QDateTime(QDate(2004,12,1), QTime(16, 1,42,900) ) );

    _d->setCell( 6, 0,    10.0,  QDateTime(QDate(2004,12,1), QTime(16, 4,35,800) ) );
    _d->setCell( 7, 0,    13.0,  QDateTime(QDate(2004,12,1), QTime(16, 2,32,900) ) );
    _d->setCell( 8, 0,    -0.50, QDateTime(QDate(2004,12,1), QTime(16, 9, 3,200) ) );
    _d->setCell( 6, 1,    11.0,  QDateTime(QDate(2004,12,1), QTime(16, 0,23,100) ) );
    _d->setCell( 7, 1,     9.25, QDateTime(QDate(2004,12,1), QTime(16, 3,33,400) ) );
    _d->setCell( 8, 1,    12.0,  QDateTime(QDate(2004,12,1), QTime(16, 2,37,700) ) );
    _d->setCell( 6, 2,    13.0,  QDateTime(QDate(2004,12,1), QTime(16, 7,40,900) ) );
    _d->setCell( 7, 2,    21.0,  QDateTime(QDate(2004,12,1), QTime(16, 6, 5,300) ) );
    _d->setCell( 8, 2,    12.0,  QDateTime(QDate(2004,12,1), QTime(15,59,15,200) ) );

    _p->setLegendPosition( KDChart1Params::LegendBottom );
    _p->setLegendOrientation( Qt::Horizontal );
    //_p->setLegendTitleText( QString() );
}


void KDChart1PresentationWidget::setConfiguration_8()
{
    // "pie: simple"
    resetData( 1, 5 );
    resetParams( KDChart1Params::Pie );

    _p->setHeader1Text( "Pie chart");

    _p->setHeader2Text( "- simple mode showing default text settings -" );

    _p->setFooterText(  "[ orientation of text may be tangential, sagittal, or fixed angle ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
                            QFont( "times", 24, QFont::Bold, true ),
                            true,
                            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
                            QFont( "helvetica", 24, QFont::Bold, false ),
                            true,
                            18 );

    _p->setPieStart( 20 );
    _p->setExplodeFactor( 0.0 );
    _p->setThreeDPies( false );

    _d->clearAllCells();
    _d->setCell( 0, 0,     5.0 );
    _d->setCell( 0, 1,    37.5 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,    25.0 );
    _d->setCell( 0, 4,    20.0 );

    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_9()
{
    // "pie: simple, sagittal texts"
    resetData( 1, 5 );
    resetParams( KDChart1Params::Pie );

    _p->setHeader1Text( "Pie chart");

    _p->setHeader2Text( "- simple mode with user-defined text settings -" );

    _p->setFooterText(  "[ orientation of text may be tangential, sagittal, or fixed angle ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
                            QFont( "times", 24, QFont::Bold, true ),
                            true,
                            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
                            QFont( "helvetica", 24, QFont::Bold, false ),
                            true,
                            18 );

    _p->setPieStart( 20 );
    _p->setExplodeFactor( 0.0 );
    _p->setThreeDPies( false );

    _d->clearAllCells();
    _d->setCell( 0, 0,    10.0 );
    _d->setCell( 0, 1,    37.5 );
    _d->setCell( 0, 2,     5.0 );
    _d->setCell( 0, 3,    25.0 );
    _d->setCell( 0, 4,    20.0 );

    int rotation = KDCHART1_SAGITTAL_ROTATION;
    QFont  dataValuesFont( "helvetica", 1, QFont::Bold );
    QColor dataValuesColor( Qt::red );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 26.0 ) : 26,
            &dataValuesColor,
            KDChart1Enums::PosTopCenter,
            Qt::AlignLeft | Qt::AlignVCenter,
            0,
            0,
            rotation,
            KDChart1Enums::PosTopCenter,
            Qt::AlignLeft | Qt::AlignVCenter,
            0,
            -25,
            rotation,
            KDChart1Enums::LayoutPolicyRotate );

    _p->setLegendPosition( KDChart1Params::LegendBottomRightRight );
}


void KDChart1PresentationWidget::setConfiguration_10()
{
    // "pie: 3d, exploding"
    resetData( 1, 5 );
    resetParams( KDChart1Params::Pie );

    _p->setHeader1Text( "Pie chart");

    _p->setHeader2Text( "- some segments exploding in 3-dimensional mode -" );

    _p->setFooterText(  "[ orientation of text may be tangential, sagittal, or fixed angle ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _p->setPieStart( 20 );
    _p->setExplode( true );
    _p->setExplodeFactor( 0.2 );
    _p->setThreeDPies( true );
    _p->setThreeDPieHeight( 30 );

    QList<int> explodeList;
    explodeList.clear();
    //explodeList << 0;
    explodeList << 2;
    _p->setExplodeValues( explodeList );


    _d->clearAllCells();
    _d->setCell( 0, 0,    37.5 );
    _d->setCell( 0, 1,     5.0 );
    _d->setCell( 0, 2,    10.0 );
    _d->setCell( 0, 3,    25.0 );
    _d->setCell( 0, 4,    20.0 );
    /*
        int rotation = KDCHART1_TANGENTIAL_ROTATION;
        QFont  dataValuesFont( "times", 1, QFont::Bold );
        _p->setPrintDataValues( true,
        KDCHART1_ALL_CHARTS,
        0,
        KDCHART1_DATA_VALUE_AUTO_DIGITS,
        &dataValuesFont,
        _size ? static_cast < uint > ( _size->value() / 10.0 * 24.0 ) : 24,
        KDCHART1_DATA_VALUE_AUTO_COLOR,
        KDChart1Enums::PosTopCenter,
        Qt::AlignTop + Qt::AlignHCenter,
        0,
        0,
        rotation,
        KDChart1Enums::PosTopCenter,
        Qt::AlignTop + Qt::AlignHCenter,
        0,
        50,
        rotation,
        KDChart1Enums::LayoutPolicyRotate );
        */
    _p->setLegendPosition( KDChart1Params::LegendBottomRightRight );
}


void KDChart1PresentationWidget::setConfiguration_11()
{
    // "ring: simple"
    resetData( 2, 5 );
    resetParams( KDChart1Params::Ring );

    _p->setHeader1Text( "Ring chart");

    _p->setHeader2Text( "- simple mode -" );

    _p->setFooterText(  "[ orientation of text may be tangential, sagittal, or fixed angle ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  4.0 );
    _d->setCell( 1, 1,  2.0 );
    _d->setCell( 1, 2, 10.0 );
    _d->setCell( 1, 3,  8.0 );
    _d->setCell( 1, 4,  6.0 );

    _p->setRingStart( 0 );
    _p->setExplode( false );
    _p->setExplodeFactor( 0.1 );
    _p->setRelativeRingThickness( false );
    /*
        int rotation = KDCHART1_TANGENTIAL_ROTATION;
        QFont  dataValuesFont( "times", 1, QFont::Bold );
        _p->setPrintDataValues( true,
        KDCHART1_ALL_CHARTS,
        0,
        KDCHART1_DATA_VALUE_AUTO_DIGITS,
        &dataValuesFont,
        _size ? static_cast < uint > ( _size->value() / 10.0 * 24.0 ) : 24,
        KDCHART1_DATA_VALUE_AUTO_COLOR,
        KDChart1Enums::PosTopCenter,
        Qt::AlignTop + Qt::AlignHCenter,
        0,
        0,
        rotation,
        KDChart1Enums::PosCenter,
        Qt::AlignCenter,
        0,
        0,
        rotation,
        KDChart1Enums::LayoutPolicyRotate );
        */
    _p->setLegendPosition( KDChart1Params::LegendBottomLeftLeft );
}


void KDChart1PresentationWidget::setConfiguration_12()
{
    // "ring: relative thickness"
    resetData( 2, 5 );
    resetParams( KDChart1Params::Ring );

    _p->setHeader1Text( "Ring chart");

    _p->setHeader2Text( "- relative thickness mode -" );

    _p->setFooterText(  "[ orientation of text may be tangential, sagittal, or fixed angle ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  4.0 );
    _d->setCell( 1, 1,  2.0 );
    _d->setCell( 1, 2, 10.0 );
    _d->setCell( 1, 3,  8.0 );
    _d->setCell( 1, 4,  6.0 );

    _p->setRingStart( 0 );
    _p->setExplode( false );
    _p->setExplodeFactor( 0.1 );
    _p->setRelativeRingThickness( true );
    /*
        int rotation = 0;
        QFont  dataValuesFont( "courier", 1, QFont::Normal );
        _p->setPrintDataValues( true,
        KDCHART1_ALL_CHARTS,
        0,
        KDCHART1_DATA_VALUE_AUTO_DIGITS,
        &dataValuesFont,
        _size ? static_cast < uint > ( _size->value() / 10.0 * 18.0 ) : 18,
        KDCHART1_DATA_VALUE_AUTO_COLOR,
        KDChart1Enums::PosTopCenter,
        Qt::AlignTop + Qt::AlignHCenter,
        0,
        0,
        rotation,
        KDChart1Enums::PosCenter,
        Qt::AlignCenter,
        0,
        0,
        rotation,
        KDChart1Enums::LayoutPolicyRotate );
        */
    _p->setLegendPosition( KDChart1Params::LegendBottomLeftLeft );
}


void KDChart1PresentationWidget::setConfiguration_13()
{
    // "ring: relative, exploding"
    resetData( 2, 10 );
    resetParams( KDChart1Params::Ring );

    _p->setHeader1Text( "Ring chart");

    _p->setHeader2Text( "- all segments exploding in relative thickness mode -" );

    _p->setFooterText(  "[ orientation of text may be tangential, sagittal, or fixed angle ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  6.0 );
    _d->setCell( 0, 4,  3.0 );
    _d->setCell( 0, 5,  3.0 );
    _d->setCell( 0, 6,  2.0 );
    _d->setCell( 0, 7,  3.0 );
    _d->setCell( 0, 8,  1.0 );
    _d->setCell( 0, 9,  4.0 );

    _d->setCell( 1, 0,  4.0 );
    _d->setCell( 1, 1,  2.0 );
    _d->setCell( 1, 2, 10.0 );
    _d->setCell( 1, 3,  8.0 );
    _d->setCell( 1, 4,  6.0 );
    _d->setCell( 1, 5,  6.0 );
    _d->setCell( 1, 6,  4.0 );
    _d->setCell( 1, 7,  6.0 );
    _d->setCell( 1, 8,  2.0 );
    _d->setCell( 1, 9,  8.0 );

    _p->setRingStart( 0 );
    _p->setExplode( true );
    _p->setExplodeFactor( 0.1 );
    _p->setRelativeRingThickness( true );

    int rotation = KDCHART1_TANGENTIAL_ROTATION;
    QFont dataValuesFont( "times", 1, QFont::Bold );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 24.0 ) : 24,
            0,
            KDChart1Enums::PosTopCenter,
            Qt::AlignTop + Qt::AlignHCenter,
            0,
            0,
            rotation,
            KDChart1Enums::PosTopCenter,
            Qt::AlignBottom + Qt::AlignHCenter,
            0,
            0,
            rotation,
            KDChart1Enums::LayoutPolicyRotate );

    _p->setLegendPosition( KDChart1Params::LegendBottomLeftLeft );
}


void KDChart1PresentationWidget::setConfiguration_14()
{
    // "ring: rel., some exploding"
    resetData( 2, 6 );
    resetParams( KDChart1Params::Ring );

    _p->setHeader1Text( "Ring chart");

    _p->setHeader2Text( "- some segments exploding in relative thickness mode -" );

    _p->setFooterText(  "[ orientation of text may be tangential, sagittal, or fixed angle ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );
    _d->setCell( 0, 5,  3.0 );

    _d->setCell( 1, 0,  4.0 );
    _d->setCell( 1, 1,  2.0 );
    _d->setCell( 1, 2, 10.0 );
    _d->setCell( 1, 3,  8.0 );
    _d->setCell( 1, 4,  6.0 );
    _d->setCell( 1, 5,  6.0 );

    _p->setRingStart( 0 );
    _p->setExplode( true );
    _p->setExplodeFactor( 0.1 );
    QList<int> explodeList;
    explodeList.clear();
    explodeList << 0;
    explodeList << 2;
    _p->setExplodeValues( explodeList );
    _p->setRelativeRingThickness( true );
    /*
        int rotation = KDCHART1_TANGENTIAL_ROTATION;
        QFont  dataValuesFont( "times", 1, QFont::Bold );
        _p->setPrintDataValues( true,
        KDCHART1_ALL_CHARTS,
        0,
        KDCHART1_DATA_VALUE_AUTO_DIGITS,
        &dataValuesFont,
        _size ? static_cast < uint > ( _size->value() / 10.0 * 24.0 ) : 24,
        KDCHART1_DATA_VALUE_AUTO_COLOR,
        KDChart1Enums::PosTopCenter,
        Qt::AlignTop + Qt::AlignHCenter,
        0,
        0,
        rotation,
        KDChart1Enums::PosCenter,
        Qt::AlignCenter,
        0,
        0,
        rotation,
        KDChart1Enums::LayoutPolicyRotate );
        */
    _p->setLegendPosition( KDChart1Params::LegendBottomLeftLeft );
}


void KDChart1PresentationWidget::setConfiguration_15()
{
    // "area: simple"
    resetData( 2, 5 );
    resetParams( KDChart1Params::Area );

    _p->setAreaChartSubType( KDChart1Params::AreaNormal );
    _p->setAreaLocation( KDChart1Params::AreaBelow );
    _p->setHeader1Text( "Area chart");

    _p->setHeader2Text( "- traditional view -" );

    _p->setFooterText(  "Problem: One of the areas is partially hidden by the other one.");
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,    3.0 );
    _d->setCell( 0, 1,    4.0 );
    _d->setCell( 0, 2,    2.0 );
    _d->setCell( 0, 3,    1.0 );
    _d->setCell( 0, 4,    0.0 );

    _d->setCell( 1, 0,    3.0 );
    _d->setCell( 1, 1,    2.0 );
    _d->setCell( 1, 2,    3.0 );
    _d->setCell( 1, 3,    0.0 );
    _d->setCell( 1, 4,    1.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_16()
{
    // "area: reverse drawing view"
    resetData( 2, 5 );
    resetParams( KDChart1Params::Area );

    _p->setAreaChartSubType( KDChart1Params::AreaNormal );
    _p->setAreaLocation( KDChart1Params::AreaAbove );
    _p->setHeader1Text( "Area chart");

    _p->setHeader2Text( "- reverse drawing view -" );

    _p->setFooterText(  "Area parts normally hidden by the other area are visible now!");
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,    3.0 );
    _d->setCell( 0, 1,    4.0 );
    _d->setCell( 0, 2,    2.0 );
    _d->setCell( 0, 3,    1.0 );
    _d->setCell( 0, 4,    0.0 );

    _d->setCell( 1, 0,    3.0 );
    _d->setCell( 1, 1,    2.0 );
    _d->setCell( 1, 2,    3.0 );
    _d->setCell( 1, 3,    0.0 );
    _d->setCell( 1, 4,    1.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_17()
{
    // "area: stacked"
    resetData( 2, 5 );
    resetParams( KDChart1Params::Area );

    _p->setAreaChartSubType( KDChart1Params::AreaStacked );
    _p->setAreaLocation( KDChart1Params::AreaBelow );
    _p->setHeader1Text( "Area chart");

    _p->setHeader2Text( "- traditional view in stacked mode -" );

    _p->setFooterText(  "[ stacked mode also available for bar and line charts ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,    3.0 );
    _d->setCell( 0, 1,    4.0 );
    _d->setCell( 0, 2,    2.0 );
    _d->setCell( 0, 3,    1.0 );
    _d->setCell( 0, 4,    0.0 );

    _d->setCell( 1, 0,    3.0 );
    _d->setCell( 1, 1,    2.0 );
    _d->setCell( 1, 2,    3.0 );
    _d->setCell( 1, 3,    0.0 );
    _d->setCell( 1, 4,    1.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_18()
{
    // "area: percent"
    resetData( 6, 5 );
    resetParams( KDChart1Params::Area );

    _p->setAreaChartSubType( KDChart1Params::AreaPercent );
    _p->setAreaLocation( KDChart1Params::AreaBelow );
    _p->setHeader1Text( "Area chart");

    _p->setHeader2Text( "- traditional view in percentage mode - subdued colors -" );

    _p->setFooterText(  "[ percentage mode also available for bar and line charts ]");


    // activate subdued color set
    _p->setDataSubduedColors( true );  // TRUE == colours sorted according to their values

    // disable using a special background for the data value texts
    // by re-setting the parameters to their default value
    _p->setDataValuesColors();

    // move the data values to the right side of their respective points
    _p->setDataValuesPlacing( KDChart1Enums::PosCenter,
                              Qt::AlignTop | Qt::AlignLeft,
                              6,2,
                              0 );

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,    3.0 );
    _d->setCell( 0, 1,    4.0 );
    _d->setCell( 0, 2,    2.0 );
    _d->setCell( 0, 3,    1.0 );
    _d->setCell( 0, 4,    0.0 );

    _d->setCell( 1, 0,    3.0 );
    _d->setCell( 1, 1,    2.0 );
    _d->setCell( 1, 2,    3.0 );
    _d->setCell( 1, 3,    0.0 );
    _d->setCell( 1, 4,    1.0 );

    _d->setCell( 2, 0,    4.0 );
    _d->setCell( 2, 1,    3.0 );
    _d->setCell( 2, 2,    1.5 );
    _d->setCell( 2, 3,    1.5 );
    _d->setCell( 2, 4,    1.0 );

    _d->setCell( 3, 0,    5.0 );
    _d->setCell( 3, 1,    9.0 );
    _d->setCell( 3, 2,    1.0 );
    _d->setCell( 3, 3,    7.5 );
    _d->setCell( 3, 4,    3.0 );

    _d->setCell( 4, 0,    2.0 );
    _d->setCell( 4, 1,    3.0 );
    _d->setCell( 4, 2,    8.0 );
    _d->setCell( 4, 3,    1.0 );
    _d->setCell( 4, 4,    1.0 );

    _d->setCell( 5, 0,    5.0 );
    _d->setCell( 5, 1,    5.5 );
    _d->setCell( 5, 2,    2.0 );
    _d->setCell( 5, 3,    4.0 );
    _d->setCell( 5, 4,    2.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_19()
{
    // "area: cell specific brushes"
    const int nValues = 15;
    resetData( 1, 2*nValues );
    resetParams( KDChart1Params::Area );

    _p->setPrintDataValues( false );

    // hide the top border line of the areas
    _p->setLineStyle( Qt::NoPen );


    _p->setAreaChartSubType( KDChart1Params::AreaNormal );
    _p->setHeader1Text( "Area chart");

    _p->setHeader2Text( "- using cell-specific brushes -" );

    _p->setFooterText(  "[ Note that cell-specific brushes as of yet are supported for non-stacked view only ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );



    KDChart1PropertySet propsDarkRed(
            "using a dark red colored brush",
            KDCHART1_PROPSET_NORMAL_DATA );

    propsDarkRed.setAreaBrush( KDChart1PropertySet::OwnID, QBrush( Qt::darkRed ) );
    int idPropsDarkRed = _p->registerProperties( propsDarkRed );

    KDChart1PropertySet propsLightRed(
            "using a light red colored brush",
            KDCHART1_PROPSET_NORMAL_DATA );

    propsLightRed.setAreaBrush( KDChart1PropertySet::OwnID, QBrush( QColor( Qt::red ).light() ) );
    int idPropsLightRed = _p->registerProperties( propsLightRed );

    KDChart1PropertySet propDense3Pattern(
            "using a pattern brush",
            KDCHART1_PROPSET_NORMAL_DATA );

    propDense3Pattern.setAreaBrush( KDChart1PropertySet::OwnID, QBrush( Qt::darkRed, Qt::Dense3Pattern ) );
    int idPropDense3Pattern = _p->registerProperties( propDense3Pattern );



    _d->setCell( 0, 0,    3.0,  0.0 );
    _d->setCell( 0, 2,    4.2,  1.0 );
    _d->setCell( 0, 4,    2.1,  2.0 );
    _d->setCell( 0, 6,    1.8,  3.0 );
    _d->setCell( 0, 8,    0.9,  4.0 );
    _d->setCell( 0,10,    3.2,  5.0 );
    _d->setCell( 0,12,    2.5,  6.0 );
    _d->setCell( 0,14,    3.0,  7.0 );
    _d->setCell( 0,16,    0.2,  8.0 );
    _d->setCell( 0,18,    1.4,  9.0 );
    _d->setCell( 0,20,    3.4, 10.0 );
    _d->setCell( 0,22,    4.0, 11.0 );
    _d->setCell( 0,24,    2.1, 12.0 );
    _d->setCell( 0,26,    1.6, 13.0 );
    _d->setCell( 0,28,    0.5, 14.0 );
    QVariant vValY, vValX;
    int propID;
    for( int iCell = 0;  iCell < nValues;  ++iCell ){
        if( _d->cellContent( 0, 2*iCell, vValY, vValX, propID ) ){
            const double dValY = vValY.toDouble();
            const double dValX = vValX.toDouble();
            // duplicate the Cell
            _d->setCell( 0, 2*iCell+1, dValY, dValX+1.0 );
            if( 0.5 > dValY ){
                _d->setProp(0, 2*iCell,   idPropDense3Pattern );
                _d->setProp(0, 2*iCell+1, idPropDense3Pattern );
            }else if( 1.5 > dValY ){
                _d->setProp(0, 2*iCell,   idPropsDarkRed );
                _d->setProp(0, 2*iCell+1, idPropsDarkRed );
            }else if( 3.0 < dValY ){
                _d->setProp(0, 2*iCell,   idPropsLightRed );
                _d->setProp(0, 2*iCell+1, idPropsLightRed );
            }
        }
    }

    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_20()
{
    // "high/low: simple"
    resetData( 3, 4 );
    resetParams( KDChart1Params::HiLo );
    _p->setHiLoChartSubType( KDChart1Params::HiLoSimple );
    _p->setLineWidth( 3 );
    QFont dataValuesFont( "helvetica", 1, QFont::Normal );
    QColor dataColor( Qt::darkBlue );
    uint size =
        _size
        ? static_cast < uint > ( _size->value() / 10.0 * 28.0 )
        : 26;
    _p->setHiLoChartPrintHighValues( true,
            &dataValuesFont,
            size,
            &dataColor );
    _p->setHiLoChartPrintLowValues( true,
            &dataValuesFont,
            size,
            &dataColor );

    _p->setHeader1Text( "HiLo chart" );

    _p->setHeader2Text( "- simple mode showing MIN and MAX values -" );

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  1.1 );
    _d->setCell( 0, 3,  1.9 );
    _d->setCell( 1, 0,  2.0 );
    _d->setCell( 1, 1,  3.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.5 );
    _d->setCell( 2, 0,  1.2 );
    _d->setCell( 2, 1,  2.9 );
    _d->setCell( 2, 2,  1.2 );
    _d->setCell( 2, 3,  1.2 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_21()
{
    // "high/low: close values"
    resetData( 3, 4 );
    resetParams( KDChart1Params::HiLo );
    _p->setHiLoChartSubType( KDChart1Params::HiLoClose );
    _p->setLineWidth( 3 );
    QFont dataValuesFont( "helvetica", 1, QFont::Normal );
    QColor dataColor( Qt::darkBlue );
    uint size =
        _size
        ? static_cast < uint > ( _size->value() / 10.0 * 28.0 )
        : 26;
    _p->setHiLoChartPrintCloseValues( true,
            &dataValuesFont,
            size,
            &dataColor );
    _p->setHiLoChartPrintHighValues( true,
            &dataValuesFont,
            size,
            &dataColor );
    _p->setHiLoChartPrintLowValues( true,
            &dataValuesFont,
            size,
            &dataColor );

    _p->setHeader1Text( "HiLo chart" );

    _p->setHeader2Text( "- close mode showing MAX, MIN, LAST values -" );

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  1.1 );
    _d->setCell( 0, 3,  1.9 );
    _d->setCell( 1, 0,  2.0 );
    _d->setCell( 1, 1,  3.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.5 );
    _d->setCell( 2, 0,  1.2 );
    _d->setCell( 2, 1,  2.9 );
    _d->setCell( 2, 2,  1.2 );
    _d->setCell( 2, 3,  1.2 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_22()
{
    // "high/low: open/close values"
    resetData( 3, 4 );
    resetParams( KDChart1Params::HiLo );
    _p->setHiLoChartSubType( KDChart1Params::HiLoOpenClose );
    _p->setLineWidth( 3 );
    QFont dataValuesFont( "helvetica", 1, QFont::Normal );
    QColor dataO( Qt::darkGreen );
    QColor dataC( Qt::darkRed );
    QColor dataH( Qt::blue  );
    QColor dataL( Qt::black );
    _p->setHiLoChartPrintOpenValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 28.0 ) : 28,
            &dataO );
    _p->setHiLoChartPrintCloseValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 38.0 ) : 38,
            &dataC );
    _p->setHiLoChartPrintHighValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 34.0 ) : 34,
            &dataH );
    _p->setHiLoChartPrintLowValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 30.0 ) : 30,
            &dataL );
    /*
        static QStringList abscissaNamesAndDates;
        abscissaNamesAndDates << "Monday\n01.01.2002" << "Tuesday\n02.01.2002" << "Wednesday\n03.01.2002"
        << "Thursday<br>04.01.2002" << "Friday" << "Saturday";
        _p->setAxisLabelTextParams( KDChart1AxisParams::AxisPosBottom,
        false,
         "Monday" ),
         "Friday" ),
        1.0,
        KDCHART1_AXIS_LABELS_AUTO_DIGITS,
        KDCHART1_AXIS_IGNORE_EMPTY_INNER_SPAN,
        KDChart1AxisParams::LabelsFromDataRowNo, 0,
        &abscissaNamesAndDates );
        */

    _p->setHeader1Text( "HiLo chart" );

    _p->setHeader2Text( "- open/close mode showing MAX, MIN, FIRST, LAST values -" );

    _p->setFooterText(  "[ text settings may be defined individually for each of the 4 text positions ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  1.1 );
    _d->setCell( 0, 3,  1.9 );
    _d->setCell( 1, 0,  2.0 );
    _d->setCell( 1, 1,  3.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.5 );
    _d->setCell( 2, 0,  1.2 );
    _d->setCell( 2, 1,  2.9 );
    _d->setCell( 2, 2,  1.2 );
    _d->setCell( 2, 3,  1.2 );

    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_23()
{
    // "line and high/low in one chart"
    resetData( 3, 16 );
    resetParams( KDChart1Params::Line,
            KDChart1Params::HiLo );
    _p->setLineChartSubType(  KDChart1Params::LineNormal );
    _p->setHiLoChartSubType( KDChart1Params::HiLoOpenClose );
    _p->setLineWidth( 3 );

    /*
        left axis #1:
        */
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisUseAvailableSpace( 0, -499 );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( KDCHART1_AXIS_GRID_AUTO_LINEWIDTH );
    pa.setAxisLineColor( Qt::black );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    /**/
    //pa.setAxisValueStart(  0.0 );
    //pa.setAxisValueEnd(   10.0 );
    //pa.setAxisValueDelta(  1.0 );
    /**/
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    /*
        left axis #2:
        */
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft2 );
    pa.setAxisUseAvailableSpace( -750, -1000 );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( KDCHART1_AXIS_GRID_AUTO_LINEWIDTH );
    pa.setAxisLineColor( Qt::black );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( true );
    pa.setAxisValueStart( 1.5 );
    pa.setAxisValueEnd(   3.0 );
    pa.setAxisValueDelta( 0.5 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft2, pa );

    // most of the frequently used axis settings may also
    // be specilied by KDChart1Params convenience function like this one:
    _p->setAxisLabelsFont( KDChart1AxisParams::AxisPosBottom,
                           QFont( "helvetica", 1, QFont::Normal ),
                           -12,
                           Qt::black );

    _p->setDataColor( 0, QColor( 150, 50, 50 ));
    _p->setDataColor( 1, QColor(  50,150,100));
    _p->setDataColor( 2, QColor(  50,100,150));

    // modify the legend texts to show the world what we have done :-)
    _p->setLegendSource( KDChart1Params::LegendManual );
    _p->setLegendText( 0, "some measurements" );
    _p->setLegendText( 1, "high/low 31.10.2002" );
    _p->setLegendText( 2, "high/low 01.11.2002" );

    _p->setChartSourceMode( KDChart1Params::DataEntry,     0, 0,   0 );
    _p->setChartSourceMode( KDChart1Params::DataEntry,     1, 2,   1 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosLeft,  0, 0,   0 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosLeft2, 1, 2,   1 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosRight,
            KDCHART1_NO_DATASET );
    _p->setHeader1Text( "Combination of two different charts" );

    _p->setHeader2Text( "- same abscissa axis for both charts -" );

    _p->setFooterText(  "Vertical space occupied by each chart may be specified just as you like.");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );

    _p->setLineMarker( true );
    _p->setLineMarkerSize( QSize( 24, 24 ) );

    QFont dataValuesFont( "helvetica", 10, QFont::Bold );

    QColor textColor0( Qt::yellow );
    QColor textColor1( Qt::black );
    QColor textColor1close( Qt::darkRed );
    _p->setPrintDataValues( true,
            0,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            0, // We use the fixed size of the font.
            &textColor0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            2,
            1,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            2,
            1,
            0,
            KDChart1Enums::LayoutPolicyShrinkFontSize );

    _p->setHiLoChartPrintOpenValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 28.0 ) : 28,
            &textColor1 );
    _p->setHiLoChartPrintCloseValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 36.0 ) : 36,
            &textColor1close );
    _p->setHiLoChartPrintHighValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 28.0 ) : 28,
            &textColor1 );
    _p->setHiLoChartPrintLowValues( true,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 28.0 ) : 28,
            &textColor1 );

    _d->clearAllCells();
    _d->setCell( 0, 0,  2.7, QDateTime(QDate(2002,10,31),QTime( 1,30,0)) );
    _d->setCell( 0, 1,  2.75,QDateTime(QDate(2002,10,31),QTime( 4,30,0)) );
    _d->setCell( 0, 2,  2.5, QDateTime(QDate(2002,10,31),QTime( 7,30,0)) );
    _d->setCell( 0, 3,  2.2, QDateTime(QDate(2002,10,31),QTime(10,30,0)) );
    _d->setCell( 0, 4,  1.9, QDateTime(QDate(2002,10,31),QTime(13,30,0)) );
    _d->setCell( 0, 5,  1.8, QDateTime(QDate(2002,10,31),QTime(16,30,0)) );
    _d->setCell( 0, 6,  1.8, QDateTime(QDate(2002,10,31),QTime(19,30,0)) );
    _d->setCell( 0, 7,  1.85,QDateTime(QDate(2002,10,31),QTime(22,30,0)) );
    _d->setCell( 0, 8,  1.9, QDateTime(QDate(2002,11, 1),QTime( 1,30,0)) );
    _d->setCell( 0, 9,  2.0, QDateTime(QDate(2002,11, 1),QTime( 4,30,0)) );
    _d->setCell( 0,10,  2.1, QDateTime(QDate(2002,11, 1),QTime( 7,30,0)) );
    _d->setCell( 0,11,  2.1, QDateTime(QDate(2002,11, 1),QTime(10,30,0)) );
    _d->setCell( 0,12,  2.2, QDateTime(QDate(2002,11, 1),QTime(13,30,0)) );
    _d->setCell( 0,13,  2.25,QDateTime(QDate(2002,11, 1),QTime(16,30,0)) );
    _d->setCell( 0,14,  2.15,QDateTime(QDate(2002,11, 1),QTime(19,30,0)) );
    _d->setCell( 0,15,  2.2, QDateTime(QDate(2002,11, 1),QTime(22,30,0)) );
    //--
    _d->setCell( 1, 0,  2.75 );
    _d->setCell( 1, 1,  1.8 );
    _d->setCell( 1, 2,  2.7 );
    _d->setCell( 1, 3,  1.85 );
    //--
    _d->setCell( 2, 0,  2.25 );
    _d->setCell( 2, 1,  1.9 );
    _d->setCell( 2, 2,  1.9 );
    _d->setCell( 2, 3,  2.2 );

    _p->setLegendPosition( KDChart1Params::LegendTop );
}


void KDChart1PresentationWidget::setConfiguration_24_27( int selected )
{
    // "box+whisker: without or with text"
    resetData( 3, 25 );
    resetParams( KDChart1Params::BoxWhisker );
    _p->setBWChartSubType( KDChart1Params::BWSimple );
    _p->setLineWidth( 1 );
    QFont dataValuesFont( "helvetica", 1, QFont::Normal );
    QColor dataColor( Qt::darkBlue );
    uint size =
        _size
        ? static_cast < uint > ( _size->value() / 10.0 * 20.0 )
        : 26;

    _p->setHeader1Text( "Box and Whisker chart" );

    switch( selected ){
        case 24:
            _p->setHeader2Text( "- default: showing the outliers but no texts -" );
            _p->setBWChartPrintStatistics( KDChart1Params::BWStatValALL,
                    false );
            // reset to default:
            _p->setBWChartOutValMarkerSize( -25 );
            _p->setBWChartBrush( Qt::white );
            break;
        case 25:
            _p->setHeader2Text( "- showing no outliers and no value texts -" );
            _p->setBWChartPrintStatistics( KDChart1Params::BWStatValALL,
                    false );
            _p->setBWChartOutValMarkerSize( 0 );
            // reset to default:
            _p->setBWChartBrush( Qt::white );
            break;
        case 26:
            _p->setHeader2Text( "- showing the outliers and all value texts - default colors -" );
            _p->setBWChartPrintStatistics( KDChart1Params::BWStatValALL,
                    true );
            // reset to default:
            _p->setBWChartOutValMarkerSize( -25 );
            _p->setBWChartBrush( Qt::white );
            break;
        case 27:
            {
                _p->setHeader2Text( "- showing the outliers and some value texts (no fences) - custom colors -" );
                QBrush noBrush( Qt::NoBrush );
                QBrush brush( __chamois );
                QBrush bYellow( Qt::yellow );
                QColor red( Qt::red );
                _p->setBWChartBrush( brush );
                _p->setBWChartPrintStatistics( KDChart1Params::BWStatValALL,
                        true,
                        &dataValuesFont,
                        size,
                        &dataColor,
                        &brush );
                _p->setBWChartPrintStatistics( KDChart1Params::Quartile1,
                        true,
                        &dataValuesFont,
                        size * 2,
                        &dataColor,
                        &noBrush );
                _p->setBWChartPrintStatistics( KDChart1Params::Quartile3,
                        true,
                        &dataValuesFont,
                        size * 2,
                        &dataColor,
                        &noBrush );
                _p->setBWChartPrintStatistics( KDChart1Params::MaxValue,
                        true,
                        &dataValuesFont,
                        size * 2 / 3,
                        &red,
                        &bYellow );
                _p->setBWChartPrintStatistics( KDChart1Params::MeanValue,
                        true,
                        &dataValuesFont,
                        size / 2,
                        &red );
                _p->setBWChartPrintStatistics( KDChart1Params::MinValue,
                        true,
                        &dataValuesFont,
                        size * 2 / 3,
                        &red,
                        &bYellow );
                _p->setBWChartPrintStatistics( KDChart1Params::UpperOuterFence,
                        false );
                _p->setBWChartPrintStatistics( KDChart1Params::LowerOuterFence,
                        false );
                _p->setBWChartPrintStatistics( KDChart1Params::UpperInnerFence,
                        false );
                _p->setBWChartPrintStatistics( KDChart1Params::LowerInnerFence,
                        false );
                // reset to default:
                _p->setBWChartOutValMarkerSize( -25 );
            }
            break;
    }

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    // 1st series:
    _d->setCell( 0, 0,  51 );
    _d->setCell( 0, 1,  27 );
    _d->setCell( 0, 2,  34 );
    _d->setCell( 0, 3,  52 );
    _d->setCell( 0, 4,  54 );
    _d->setCell( 0, 5,  55 );
    _d->setCell( 0, 6,  51 );
    _d->setCell( 0, 7,  57 );
    _d->setCell( 0, 8,  58 );
    _d->setCell( 0, 9,  52 );
    _d->setCell( 0,10,  50 );
    _d->setCell( 0,11,  57 );
    _d->setCell( 0,12,  21 );
    _d->setCell( 0,13,  93 );
    _d->setCell( 0,14,  55 );
    // 2nd series:
    _d->setCell( 1, 0,  91 );
    _d->setCell( 1, 1,  27 );
    _d->setCell( 1, 2,  11 );
    _d->setCell( 1, 3,  52 );
    _d->setCell( 1, 4,  54 );
    _d->setCell( 1, 5,  55 );
    _d->setCell( 1, 6,  61 );
    _d->setCell( 1, 7,  68 );
    _d->setCell( 1, 8,  78 );
    _d->setCell( 1, 9,  82 );
    _d->setCell( 1,10, 105 );
    _d->setCell( 1,11,  87 );
    _d->setCell( 1,12,  38 );
    _d->setCell( 1,13,  93 );
    _d->setCell( 1,14,  85 );
    // 3rd series:
    _d->setCell( 2, 0,  61 );
    _d->setCell( 2, 1,  47 );
    _d->setCell( 2, 2,  44 );
    _d->setCell( 2, 3,   5 );
    _d->setCell( 2, 4,  54 );
    _d->setCell( 2, 5,  55 );
    _d->setCell( 2, 6,  61 );
    _d->setCell( 2, 7,  58 );
    _d->setCell( 2, 8,  58 );
    _d->setCell( 2, 9,  12 );
    _d->setCell( 2,10,  12 );
    _d->setCell( 2,11,  57 );
    _d->setCell( 2,12,  58 );
    _d->setCell( 2,13,  53 );
    _d->setCell( 2,14,  55 );
    _d->setCell( 2,15,  53 );
    _d->setCell( 2,16,  52 );
    _d->setCell( 2,17,  53 );
    _d->setCell( 2,18,  52 );
    _d->setCell( 2,19,  53 );
    _d->setCell( 2,20,  52 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_28()
{
    // "line and box+whisker"
    resetData( 6, 15 );
    resetParams( KDChart1Params::Line,
            KDChart1Params::BoxWhisker );
    _p->setLineChartSubType(  KDChart1Params::LineNormal );
    _p->setHiLoChartSubType( KDChart1Params::HiLoOpenClose );
    _p->setBWChartSubType( KDChart1Params::BWSimple );
    _p->setLineWidth( 1 );

    QFont dataValuesFont( "helvetica", 1, QFont::Normal );
    QColor dataColor( Qt::darkBlue );
    uint size =
        _size
        ? static_cast < uint > ( _size->value() / 10.0 * 20.0 )
        : 26;

    _p->setHeader1Text( "Combination of two different charts" );

    _p->setHeader2Text( "- same abscissa axis for both charts -" );

    QBrush noBrush( Qt::NoBrush );
    QBrush brush( __chamois );
    QBrush bYellow( Qt::yellow );
    QColor red( Qt::red );
    _p->setBWChartBrush( brush );
    _p->setBWChartPrintStatistics( KDChart1Params::BWStatValALL,
            true,
            &dataValuesFont,
            size,
            &dataColor,
            &brush );
    _p->setBWChartPrintStatistics( KDChart1Params::Quartile1,
            true,
            &dataValuesFont,
            size * 2,
            &dataColor,
            &noBrush );
    _p->setBWChartPrintStatistics( KDChart1Params::Quartile3,
            true,
            &dataValuesFont,
            size * 2,
            &dataColor,
            &noBrush );
    _p->setBWChartPrintStatistics( KDChart1Params::MaxValue,
            true,
            &dataValuesFont,
            size * 2 / 3,
            &red,
            &bYellow );
    _p->setBWChartPrintStatistics( KDChart1Params::MeanValue,
            true,
            &dataValuesFont,
            size / 2,
            &red );
    _p->setBWChartPrintStatistics( KDChart1Params::MinValue,
            true,
            &dataValuesFont,
            size * 2 / 3,
            &red,
            &bYellow );
    _p->setBWChartPrintStatistics( KDChart1Params::UpperOuterFence,
            false );
    _p->setBWChartPrintStatistics( KDChart1Params::LowerOuterFence,
            false );
    _p->setBWChartPrintStatistics( KDChart1Params::UpperInnerFence,
            false );
    _p->setBWChartPrintStatistics( KDChart1Params::LowerInnerFence,
            false );
    // reset to default:
    _p->setBWChartOutValMarkerSize( -25 );


    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );

    const QColor __chamois_dark(__chamois.dark(125));
    /*
        left axis #1:
        */
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisUseAvailableSpace( 0, -359 );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( KDCHART1_AXIS_GRID_AUTO_LINEWIDTH );
    pa.setAxisLineColor( Qt::black );
    pa.setAxisGridColor( __chamois_dark );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( true );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    /*
        left axis #2:
        */
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft2 );
    pa.setAxisUseAvailableSpace( -450, -1000 );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( KDCHART1_AXIS_GRID_AUTO_LINEWIDTH );
    pa.setAxisLineColor( Qt::black );
    pa.setAxisGridColor( __chamois_dark );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( true );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft2, pa );

    /*
        bottom axis:
        */
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisLineColor( Qt::black );
    pa.setAxisGridColor( __chamois_dark );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );

    _p->setChartSourceMode( KDChart1Params::DataEntry,     0, 2,   0 );
    _p->setChartSourceMode( KDChart1Params::DataEntry,     3, 5,   1 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosLeft,  0, 2,   0 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosLeft2, 3, 5,   1 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosRight,
            KDCHART1_NO_DATASET );

    _p->setDataColor( 3, _p->dataColor(0).dark(125));
    _p->setDataColor( 4, _p->dataColor(1).dark(125));
    _p->setDataColor( 5, _p->dataColor(2).dark(125));

    for(int i=0; i<2; ++i){
        // 1st series:
        _d->setCell( i*3+0, 0,  51 );
        _d->setCell( i*3+0, 1,  27 );
        _d->setCell( i*3+0, 2,  34 );
        _d->setCell( i*3+0, 3,  52 );
        _d->setCell( i*3+0, 4,  54 );
        _d->setCell( i*3+0, 5,  55 );
        _d->setCell( i*3+0, 6,  51 );
        _d->setCell( i*3+0, 7,  57 );
        _d->setCell( i*3+0, 8,  58 );
        _d->setCell( i*3+0, 9,  52 );
        _d->setCell( i*3+0,10,  50 );
        _d->setCell( i*3+0,11,  57 );
        _d->setCell( i*3+0,12,  21 );
        _d->setCell( i*3+0,13,  93 );
        _d->setCell( i*3+0,14,  55 );
        // 2nd series:
        _d->setCell( i*3+1, 0,  91 );
        _d->setCell( i*3+1, 1,  27 );
        _d->setCell( i*3+1, 2,  11 );
        _d->setCell( i*3+1, 3,  52 );
        _d->setCell( i*3+1, 4,  54 );
        _d->setCell( i*3+1, 5,  55 );
        _d->setCell( i*3+1, 6,  61 );
        _d->setCell( i*3+1, 7,  68 );
        _d->setCell( i*3+1, 8,  78 );
        _d->setCell( i*3+1, 9,  82 );
        _d->setCell( i*3+1,10, 105 );
        _d->setCell( i*3+1,11,  87 );
        _d->setCell( i*3+1,12,  38 );
        _d->setCell( i*3+1,13,  93 );
        _d->setCell( i*3+1,14,  85 );
        // 3rd series:
        _d->setCell( i*3+2, 0,  61 );
        _d->setCell( i*3+2, 1,  47 );
        _d->setCell( i*3+2, 2,  44 );
        _d->setCell( i*3+2, 3,   5 );
        _d->setCell( i*3+2, 4,  54 );
        _d->setCell( i*3+2, 5,  55 );
        _d->setCell( i*3+2, 6,  61 );
        _d->setCell( i*3+2, 7,  58 );
        _d->setCell( i*3+2, 8,  58 );
        _d->setCell( i*3+2, 9,  12 );
        _d->setCell( i*3+2,10,  12 );
        _d->setCell( i*3+2,11,  57 );
        _d->setCell( i*3+2,12,  58 );
        _d->setCell( i*3+2,13,  53 );
        _d->setCell( i*3+2,14,  55 );
    }
    /*
        _p->setLegendText( 0, "Series 1: values" );
        _p->setLegendText( 1, "Series 2: values" );
        _p->setLegendText( 2, "Series 3: values" );
        _p->setLegendText( 3, "Series 1: B + W" );
        _p->setLegendText( 4, "Series 2: B + W" );
        _p->setLegendText( 5, "Series 3: B + W" );
        */
    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_29()
{
    // "polar: simple"
    resetData( 3, 5 );
    resetParams( KDChart1Params::Polar );

    // reset the Data Labels texts as well: they should be centered, but have
    // been changed in the previous samples - so we set them back.
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            0, // use the default font
            _size ? static_cast < uint > ( _size->value() / 10.0 * 15.0 ) : 15,
            KDCHART1_DATA_VALUE_AUTO_COLOR,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0, // no rotation
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0, // no rotation
            KDChart1Enums::LayoutPolicyShrinkFontSize );

    _p->setPolarChartSubType( KDChart1Params::PolarNormal );
    int msiz = _size ? static_cast < int > ( _size->value() / 10.0 * -40.0 ) : -40;
    _p->setPolarMarkerSize( QSize( msiz,msiz ) );

    _p->setHeader1Text( "Polar chart" );

    _p->setHeader2Text( "- normal view -" );

    _p->setFooterText(  "[ polar charts are showing both markers and texts by default ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  5.0 );
    _d->setCell( 1, 1,  4.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  1.0 );

    _d->setCell( 2, 0,  3.0 );
    _d->setCell( 2, 1,  4.0 );
    _d->setCell( 2, 2,  5.0 );
    _d->setCell( 2, 3,  6.0 );
    _d->setCell( 2, 4,  7.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_30()
{
    // "polar: no markers"
    resetData( 3, 5 );
    resetParams( KDChart1Params::Polar );
    _p->setPolarChartSubType( KDChart1Params::PolarNormal );
    _p->setPolarMarker( false );
    _p->setPolarLineWidth( -3 );


    QFont dataValuesFont( "helvetica", 1, QFont::Bold );
    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            &dataValuesFont,
            _size ? static_cast < uint > ( _size->value() / 10.0 * 30.0 ) : 30,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::LayoutPolicyShrinkFontSize );
    QColor textColor( Qt::darkGreen );//255,198,43 );
    _p->setPrintDataValuesColor( 0, &textColor );

    _p->setHeader1Text( "Polar chart" );

    _p->setHeader2Text( "- normal view but no markers, different value font -" );

    _p->setFooterText(  "[ try resizing the window to see fonts and line widths adapting accordingly ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  5.0 );
    _d->setCell( 1, 1,  4.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  1.0 );

    _d->setCell( 2, 0,  3.0 );
    _d->setCell( 2, 1,  4.0 );
    _d->setCell( 2, 2,  5.0 );
    _d->setCell( 2, 3,  6.0 );
    _d->setCell( 2, 4,  7.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_31()
{
    // "polar: no mark., no texts"
    resetData( 3, 5 );
    resetParams( KDChart1Params::Polar );
    _p->setPolarChartSubType( KDChart1Params::PolarNormal );
    _p->setPolarMarker( false );
    _p->setPolarLineWidth( -6 );



    _p->setPrintDataValues( false );

    _p->setHeader1Text( "Polar chart" );

    _p->setHeader2Text( "- normal view but no markers, no value texts and with double line width -" );

    _p->setFooterText(  "[ try resizing the window to see fonts and line widths adapting accordingly ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  5.0 );
    _d->setCell( 1, 1,  4.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  1.0 );

    _d->setCell( 2, 0,  3.0 );
    _d->setCell( 2, 1,  4.0 );
    _d->setCell( 2, 2,  5.0 );
    _d->setCell( 2, 3,  6.0 );
    _d->setCell( 2, 4,  7.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_32()
{
    // "polar: stacked view"
    resetData( 3, 5 );
    resetParams( KDChart1Params::Polar );
    _p->setPolarChartSubType( KDChart1Params::PolarStacked );
    int msiz = _size ? static_cast < int > ( _size->value() / 10.0 * -40.0 ) : -40;
    _p->setPolarMarkerSize( QSize( msiz,msiz ) );

    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            0, // use the default font
            _size ? static_cast < uint > ( _size->value() / 10.0 * 30.0 ) : 30,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::LayoutPolicyShrinkFontSize );

    _p->setHeader1Text( "Polar chart" );

    _p->setHeader2Text( "- stacked view -" );

    _p->setFooterText(  "[ polar charts are showing both markers and texts by default ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  5.0 );
    _d->setCell( 1, 1,  4.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  1.0 );

    _d->setCell( 2, 0,  3.0 );
    _d->setCell( 2, 1,  4.0 );
    _d->setCell( 2, 2,  5.0 );
    _d->setCell( 2, 3,  6.0 );
    _d->setCell( 2, 4,  7.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_33()
{
    // "polar: percentage view"
    resetData( 3, 5 );
    resetParams( KDChart1Params::Polar );
    _p->setPolarChartSubType( KDChart1Params::PolarPercent );
    int msiz = _size ? static_cast < int > ( _size->value() / 10.0 * -40.0 ) : -40;
    _p->setPolarMarkerSize( QSize( msiz,msiz ) );

    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            0, // use the default font
            _size ? static_cast < uint > ( _size->value() / 10.0 * 30.0 ) : 30,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::LayoutPolicyShrinkFontSize );

    _p->setHeader1Text( "Polar chart" );

    _p->setHeader2Text( "- percentage view -" );

    _p->setFooterText(  "[ polar charts are showing both markers and texts by default ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  5.0 );
    _d->setCell( 1, 1,  4.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  1.0 );

    _d->setCell( 2, 0,  3.0 );
    _d->setCell( 2, 1,  4.0 );
    _d->setCell( 2, 2,  5.0 );
    _d->setCell( 2, 3,  6.0 );
    _d->setCell( 2, 4,  7.0 );

    _p->setLegendPosition( KDChart1Params::LegendRight );
}


void KDChart1PresentationWidget::setConfiguration_34()
{
    // "polar: different design"
    resetData( 3, 5 );
    resetParams( KDChart1Params::Polar );
    _p->setPolarChartSubType( KDChart1Params::PolarNormal );
    _p->setPolarMarker( false );

    _p->setPolarRotateCircularLabels( true );
    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosTopLeft,      true,  true  );
    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosTopCenter,    false, false );
    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosTopRight,     true,  true  );

    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosCenterLeft,   false, false );
    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosCenterRight,  false, false );

    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosBottomLeft,   true,  true  );
    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosBottomCenter, false, false );
    _p->setPolarDelimsAndLabelsAtPos( KDChart1Enums::PosBottomRight,  true,  true  );

    _p->setPrintDataValues( true,
            KDCHART1_ALL_CHARTS,
            0,
            KDCHART1_DATA_VALUE_AUTO_DIGITS,
            0, // use the default font
            _size ? static_cast < uint > ( _size->value() / 10.0 * 30.0 ) : 30,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::PosCenter,
            Qt::AlignCenter,
            0,
            0,
            0,
            KDChart1Enums::LayoutPolicyShrinkFontSize );

    QColor textColor( 255,198,43 );
    _p->setPrintDataValuesColor( 0, &textColor );


    _p->setHeader1Text( "Polar chart" );

    _p->setHeader2Text( "- normal view, user-defined color settings -" );

    _p->setFooterText("");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterColor( KDChart1Params::HdFtPosHeader, Qt::darkRed );
    _p->setHeaderFooterColor( KDChart1Params::HdFtPosHeader2,Qt::darkRed );

    _d->setCell( 0, 0,  1.0 );
    _d->setCell( 0, 1,  2.0 );
    _d->setCell( 0, 2,  3.0 );
    _d->setCell( 0, 3,  4.0 );
    _d->setCell( 0, 4,  5.0 );

    _d->setCell( 1, 0,  5.0 );
    _d->setCell( 1, 1,  4.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  1.0 );

    _d->setCell( 2, 0,  3.0 );
    _d->setCell( 2, 1,  4.0 );
    _d->setCell( 2, 2,  5.0 );
    _d->setCell( 2, 3,  6.0 );
    _d->setCell( 2, 4,  7.0 );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosCircular );
    pa.setAxisGridLineWidth( 0 ); // thin grid lines
    pa.setAxisLineColor( Qt::darkGray );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    pa.setAxisLabelsColor( Qt::gray );
    _p->setAxisParams( KDChart1AxisParams::AxisPosCircular, pa );


    pa = _p->axisParams( KDChart1AxisParams::AxisPosSagittal );
    pa.setAxisGridLineWidth( 0 ); // thin grid lines
    pa.setAxisLineColor( Qt::darkGray );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    pa.setAxisLabelsColor( Qt::gray );
    _p->setAxisParams( KDChart1AxisParams::AxisPosSagittal, pa );

    _p->setLegendPosition( KDChart1Params::LegendBottom );

    _p->setSimpleFrame( KDChart1Enums::AreaInnermost,
            0,0,  0,0,
            true,
            true,
            KDFrame1::FrameFlat,
            1,
            0,
            QPen( Qt::red ),
            QBrush( Qt::gray ) );

    _p->setSimpleFrame( KDChart1Enums::AreaData,
            0,0,  0,0,
            true,
            true,
            KDFrame1::FrameFlat,
            1,
            0,
            QPen( Qt::red ),
            QBrush( Qt::black ) );

    _p->setSimpleFrame( KDChart1Enums::AreaLegend,
            0,0,  0,0,
            true,
            true,
            KDFrame1::FrameFlat,
            1,
            0,
            QPen( Qt::red ),
            QBrush( Qt::black ) );
    _p->setLegendTitleTextColor( Qt::white );
    _p->setLegendTextColor( Qt::gray );
}


void KDChart1PresentationWidget::setConfiguration_35()
{
    // "isometric coordinate system"
    resetData( 1, 26 );
    resetParams( KDChart1Params::Line );
    _p->setLineChartSubType( KDChart1Params::LineNormal );
    _p->setLineWidth( 1 );

    //
    //left axis:
    //
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisLineColor( Qt::blue );
    pa.setAxisGridColor( __chamois.dark(125) );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( true );
    pa.setIsometricReferenceAxis( KDChart1AxisParams::AxisPosBottom );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    //
    //bottom axis:
    //
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisLineColor( Qt::blue );
    pa.setAxisGridColor( __chamois.dark(125) );
    pa.setAxisShowGrid( true );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );

    //
    // _p->insertCustomBox(
    //     KDChart1CustomBox(
    //         KDChart1TextPiece( "%",
    //                           QFont( "times", 1, QFont::Normal, false ) ),
    //         -32,true,
    //         0,0,
    //         -165,-275,
    //         Qt::darkBlue,
    //         Qt::NoBrush,
    //         KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosLeft,
    //         KDChart1Enums::PosTopLeft,
    //         Qt::AlignTop + Qt::AlignLeft ) );
    //
    _p->setHeader1Text( "Line chart on isometric cartesian coordinate system");

    _p->setHeader2Text( "- using several cell-specific properties -" );

    _p->setFooterText(  "[ a Qt::NoPen property causes the gap between the figures ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _p->setPrintDataValues( false );

    // specify some nice property sets
    KDChart1PropertySet propSet;
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 1 );
    const int w1 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 2 );
    const int w2 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 3 );
    const int w3 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 4 );
    const int w4 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 5 );
    const int w5 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 6 );
    const int w6 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 7 );
    const int w7 = _p->registerProperties( propSet );
    int iDummy = 0; // initialization is not necessary, only done
    // here to avoid annoying compiler warning
    QColor cDummy;
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 3 );
    propSet.setLineStyle( KDChart1PropertySet::OwnID, Qt::DashDotLine );
    const int e1 = _p->registerProperties( propSet );
    propSet.setLineStyle( KDChart1PropertySet::OwnID, Qt::DashLine );
    const int e2 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::OwnID, 6 );
    propSet.setLineColor( KDChart1PropertySet::OwnID, Qt::blue );
    propSet.setLineStyle( KDChart1PropertySet::OwnID, Qt::DotLine );
    const int e3 = _p->registerProperties( propSet );
    propSet.setLineWidth( KDChart1PropertySet::UndefinedID, iDummy );
    propSet.setLineColor( KDChart1PropertySet::UndefinedID, cDummy );
    propSet.setLineStyle( KDChart1PropertySet::UndefinedID, Qt::SolidLine );
    propSet.setShowMarker( KDChart1PropertySet::OwnID, true );
    const int e4 = _p->registerProperties( propSet );

    // some coordinates using int values
    _d->setCell( 0,   0,    3  ,   3 );
    _d->setProp( 0,   0,    w1 );
    _d->setCell( 0,   1,    1  ,   4  );
    _d->setProp( 0,   1,    w2 );
    _d->setCell( 0,   2,   -1  ,   4  );
    _d->setProp( 0,   2,    w3 );
    _d->setCell( 0,   3,   -3  ,   3  );
    _d->setProp( 0,   3,    w4 );
    _d->setCell( 0,   4,   -4  ,   1  );
    _d->setProp( 0,   4,    w5 );
    _d->setCell( 0,   5,   -4  ,  -1  );
    _d->setProp( 0,   5,    w6 );
    _d->setCell( 0,   6,   -3  ,  -3  );
    _d->setProp( 0,   6,    w7 );
    _d->setCell( 0,   7,   -1  ,  -4  );
    _d->setProp( 0,   7,    w6 );
    _d->setCell( 0,   8,    1  ,  -4  );
    _d->setProp( 0,   8,    w5 );
    _d->setCell( 0,   9,    3  ,  -3  );
    _d->setProp( 0,   9,    w4 );
    _d->setCell( 0,  10,    4  ,  -1  );
    _d->setProp( 0,  10,    w3 );
    _d->setCell( 0,  11,    4  ,   1  );
    _d->setProp( 0,  11,    w2 );
    _d->setCell( 0,  12,    3  ,   3  );
    _d->setProp( 0,  12,    KDCHART1_PROPSET_TRANSPARENT_DATA );
    //
    //_d->setCell( 0,   0,    8  ,   8  );
    //_d->setCell( 0,   1,    6  ,   9  );
    //_d->setCell( 0,   2,    4  ,   9  );
    //_d->setCell( 0,   3,    2  ,   8  );
    //_d->setCell( 0,   4,    1  ,   6  );
    //_d->setCell( 0,   5,    1  ,   4  );
    //_d->setCell( 0,   6,    2  ,   2  );
    //_d->setCell( 0,   7,    4  ,   1  );
    //_d->setCell( 0,   8,    6  ,   1  );
    //_d->setCell( 0,   9,    8  ,   2  );
    //_d->setCell( 0,  10,    9  ,   4  );
    //_d->setCell( 0,  11,    9  ,   6  );
    //_d->setCell( 0,  12,    8  ,   8  );
    //_d->setProp( 0,  12,    KDCHART1_PROPSET_TRANSPARENT_DATA );
    //

    // some coordinates using int or double values
    _d->setCell( 0,  13,    6  ,  9  );
    _d->setCell( 0,  14,    6  , 10  );
    _d->setProp( 0,  14,    e1 );
    _d->setCell( 0,  15,    6.5, 11  );
    _d->setCell( 0,  16,    7.5, 11.5 );
    _d->setCell( 0,  17,    8.5, 11.5 );
    _d->setProp( 0,  17,    e2 );
    _d->setCell( 0,  18,    9.5, 11  );
    _d->setCell( 0,  19,   10  , 10  );
    _d->setCell( 0,  20,   10  ,  9  );
    _d->setProp( 0,  20,    e3 );
    _d->setCell( 0,  21,    9.5,  8  );
    _d->setCell( 0,  22,    8.5,  7.5 );
    _d->setCell( 0,  23,    7.5,  7.5 );
    _d->setProp( 0,  23,    e4 );
    _d->setCell( 0,  24,    6.5,  8  );
    _d->setCell( 0,  25,    6  ,  9  );

    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_36()
{
    // "a simple grid plot"
    const uint nRows = 100;
    const uint nCols = 140;
    resetData( nRows, nCols );
    resetParams( KDChart1Params::Line );

    // hide the lines and the markers:
    // we will use cell-specific properties instead
    _p->setLineStyle( Qt::NoPen );
    _p->setLineMarker( false );

    _p->setHeader1Text( "Grid Plot using cell-specific properties");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "times", 24, QFont::Bold, false ),
            true,
            27 );

    //
    //left axis:
    //
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    pa.setAxisLineColor( Qt::darkGray );
    pa.setAxisGridColor( __chamois.dark(125) );

    pa.setAxisValueStart(  0.0 );
    pa.setAxisValueEnd(  100.0 );
    pa.setAxisValueDelta( 20.0 );

    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    //pa.setIsometricReferenceAxis( KDChart1AxisParams::AxisPosBottom );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    //
    //bottom axis:
    //
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisValues( true );
    pa.setAxisLineColor( Qt::darkGray );
    //pa.setAxisGridColor( __chamois.dark(125) );

    pa.setAxisValueStart(  0.0 );
    pa.setAxisValueEnd(  140.0 );
    pa.setAxisValueDelta( 20.0 );

    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( false );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );

    // most of the frequently used axis settings may also
    // be specilied by KDChart1Params convenience function like this one:
    _p->setAxisLabelsFont( KDChart1AxisParams::AxisPosLeft,
                           QFont( "helvetica", 1, QFont::Normal ),
                           -12,
                           Qt::black );
    _p->setAxisLabelsFont( KDChart1AxisParams::AxisPosBottom,
                           QFont( "helvetica", 1, QFont::Normal ),
                           -12,
                           Qt::black );


    _p->setPrintDataValues( false );


    // specify the colors to be used
    typedef QList<QColor> ColorList;
    ColorList colors;
    colors.append( QColor( Qt::white ) );
    colors.append( QColor( 0xe0,0x80,0 ) );
    colors.append( QColor( 0xff,0xff,0 ) );
    colors.append( QColor( 0x80,0xe0,0 ) );
    colors.append( QColor(    0,0x80,0 ) );
    colors.append( QColor(    0,0xff,0 ) );
    colors.append( QColor( 0x40,0xa0,0xc0 ) );
    colors.append( QColor( 0x40,0x40,0xc0 ) );
    colors.append( QColor( 0x60,   0,0xc0 ) );
    colors.append( QColor( 0x20,   0,0x40 ) );


    _d->clearAllCells();

    // make all the cells be shown at their respective positions:
    for( uint y=0; y<nRows; ++y )
        for( uint x=0; x<nCols; ++x )
            _d->setCell( y, x,   y, x );
    /*
    // calculate the geometry of the chart: used for calculating the size of the data fields
    bool bOk;
    {
        QPainter painter( this ); // a temporary QPainter is needed
        QRect drawRect;
        bOk = KDChart1::painterToDrawRect( &painter, drawRect ) &&
              KDChart1::setupGeometry( &painter, _p, _d, drawRect );
    }
    if( bOk ){
        // calculate the size of the data fields:
        const QSize fieldSize( _p->dataArea().width() / nCols + 2, _p->dataArea().height() / nRows + 2);

        // define and register the property sets for our colours
        typedef QList<int> PropIDsList;
        PropIDsList propIDs;
        KDChart1PropertySet props0(   "color 0", KDCHART1_PROPSET_NORMAL_DATA );
        props0.setShowMarker(  KDChart1PropertySet::OwnID, true );
        props0.setMarkerAlign( KDChart1PropertySet::OwnID, Qt::AlignBottom | Qt::AlignLeft );
        props0.setMarkerSize(  KDChart1PropertySet::OwnID, fieldSize );
        props0.setMarkerStyle( KDChart1PropertySet::OwnID, KDChart1Params::LineMarkerSquare );
        props0.setMarkerColor( KDChart1PropertySet::OwnID, colors[0] );
        const int idProps0 = _p->registerProperties( props0 );
        propIDs.append( idProps0 );
        int i=1;
        ColorList::iterator it = colors.begin();
        ++it;
        for ( ; it != colors.end(); ++it, ++i ){
            KDChart1PropertySet props( QString("color %1").arg(i), idProps0 );
            props.setMarkerColor( KDChart1PropertySet::OwnID, (*it) );
            propIDs.append( _p->registerProperties( props ) );
        }

        // specify the color value for all of the cells:
        for( uint y=0; y<nRows; ++y ){
            for( uint x=0; x<nCols; ++x ){
                const double val = getGridPlotValue( y/25.0, x/25.0 );
                const double stepWidth =  0.25;
                const double stepStart = -1.0;

                i=0;
                double step = stepStart + stepWidth;
                int idProps = -1;
                for ( it = colors.begin(); it != colors.end(); ++it, ++i, step+=stepWidth ){
                    if( val < step ){
                        idProps = propIDs[ i ];
                        break;
                    }
                }
                if( idProps == -1 )
                    idProps = propIDs.last();

                _d->setProp( y,x, idProps );
            }
        }
    }
    */

    _p->setLegendPosition( KDChart1Params::NoLegend );
    //_p->setLegendPosition( KDChart1Params::LegendBottom );
    //_p->setLegendOrientation( Qt::Horizontal );
}


void KDChart1PresentationWidget::setConfiguration_37()
{
    // "function plot: axes labels"

    const double minT  =   0.01;
    const double maxT  =  15.99;
    const double stepT =   0.005;
    resetData( 3, static_cast < unsigned int > ((maxT-minT) / stepT + 1 ) );

    resetParams( KDChart1Params::Line/*,
                                    KDChart1Params::NoType,
                                    QPen( Qt::blue ),
                                    QBrush( Qt::white )*/ );
    _p->setLineChartSubType( KDChart1Params::LineNormal );

    _p->setLineMarker( true );
    _p->setLineMarkerStyle( 0, KDChart1Params::LineMarker4Pixels );
    _p->setLineMarkerStyle( 1, KDChart1Params::LineMarker4Pixels );
    _p->setLineMarkerStyle( 2, KDChart1Params::LineMarker4Pixels );
    _p->setLineWidth( 2);//-3 );

    _p->setDataColor( 0, QColor( 139, 139, 139 ));
    _p->setDataColor( 1, QColor(  62, 132, 132 ));
    _p->setDataColor( 2, QColor( 183, 105, 160 ));

    const QColor gridRed(  239, 165, 162);
    const QColor gridGreen(142, 205, 133);
    //
    //left axis:
    //
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisLineWidth( 1 );
    pa.setAxisLineColor( Qt::blue );
    /*
    pa.setAxisValueStart(-1.0 );
    pa.setAxisValueEnd(   9.0 );
    //                pa.setAxisValueDelta( 0.2 );
    */
    pa.setAxisShowGrid(      true );
    pa.setAxisGridLineWidth( 1 );
    pa.setAxisGridColor(     gridRed );

    pa.setAxisShowSubDelimiters( true );
    pa.setAxisGridSubLineWidth(  1 );
    pa.setAxisGridSubColor(      gridGreen );
    pa.setAxisGridSubStyle(      Qt::SolidLine );

    pa.setIsometricReferenceAxis( KDChart1AxisParams::AxisPosBottom );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );


    //
    //bottom axis:
    //
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisLineWidth( 1 );
    pa.setAxisLineColor( Qt::blue );

    pa.setAxisValueStart( 0.0 );
    pa.setAxisValueEnd(  16.0 );
    pa.setAxisValueDelta( 0.5 );

    pa.setAxisShowGrid(      true );
    pa.setAxisGridLineWidth( 1 );
    pa.setAxisGridColor(     gridRed );

    pa.setAxisShowSubDelimiters( true );
    pa.setAxisGridSubLineWidth(  1 );
    pa.setAxisGridSubColor(      gridGreen );
    pa.setAxisGridSubStyle(      Qt::SolidLine );

    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );


    KDChart1TextPiece textPiece1( "<qt><nobr><b>left</b> Y: <small>small</small> <big>big</big></nobr></qt>",
                                 QFont( "times", 6, QFont::Normal, false ) );
    KDChart1CustomBox customBox1( -90,
                                 textPiece1,
                                 -18,true,
                                 0,-40,
                                 -200,-200,
                                 Qt::darkRed,
                                 Qt::NoBrush,
                                 KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosLeft,
                                 KDChart1Enums::PosBottomLeft,
                                 Qt::AlignTop + Qt::AlignLeft,
                                 0,0,0,
                                 Qt::AlignBottom + Qt::AlignRight,
                                 false );
    _p->insertCustomBox( customBox1 );
    KDChart1TextPiece textPiece2( "<qt><nobr><center><b>center</b> Y: <small>small</small> <big>big</big></center></nobr></qt>",
                                 QFont( "helvetica", 6, QFont::Normal, false ) );
    KDChart1CustomBox customBox2( -90,
                                 textPiece2,
                                 -18,true,
                                 0,-40,
                                 -200,-200,
                                 Qt::darkGreen,
                                 Qt::NoBrush,
                                 KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosLeft,
                                 KDChart1Enums::PosCenterLeft,
                                 Qt::AlignTop + Qt::AlignHCenter,
                                 0,0,0,
                                 Qt::AlignBottom + Qt::AlignRight,
                                 false );
    _p->insertCustomBox( customBox2 );
    KDChart1TextPiece textPiece3( "<qt><p align=right><nobr><b>right</b> Y: <small>small</small> <big>big</big></nobr></p></qt>",
                                 QFont( "times", 6, QFont::Normal, false ) );
    KDChart1CustomBox customBox3( -90,
                                 textPiece3,
                                 -18,true,
                                 0,-40,
                                 -200,-200,
                                 Qt::darkBlue,
                                 Qt::NoBrush,
                                 KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosLeft,
                                 KDChart1Enums::PosTopLeft,
                                 Qt::AlignTop + Qt::AlignRight,
                                 0,0,0 ,
                                 Qt::AlignBottom + Qt::AlignRight,
                                 false );
    _p->insertCustomBox( customBox3 );

    KDChart1TextPiece textPiece4( "<qt><nobr><b>left</b> X: <small>small</small> <big>big</big></nobr></qt>",
                                 QFont( "times", 6, QFont::Normal, false ) );
    KDChart1CustomBox customBox4(
        textPiece4,
        -18,true,
        0,0,
        -200,-50,
        Qt::darkRed,
        Qt::NoBrush,
        KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosBottom,
        KDChart1Enums::PosBottomLeft,
        Qt::AlignBottom + Qt::AlignLeft,
        0,0,0,
        Qt::AlignTop + Qt::AlignLeft );
    _p->insertCustomBox( customBox4 );

    KDChart1TextPiece textPiece5( "<qt><nobr><center><b>center</b> X: <small>small</small> <big>big</big></nobr></center></qt>",
                                 QFont( "helvetica", 6, QFont::Normal, false ) );
    KDChart1CustomBox customBox5(
        textPiece5,
        -18,true,
        0,0,
        -200,-50,
        Qt::darkGreen,
        Qt::NoBrush,
        KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosBottom,
        KDChart1Enums::PosBottomCenter,
        Qt::AlignBottom + Qt::AlignHCenter,
        0,0,0,
        Qt::AlignTop + Qt::AlignHCenter );
    _p->insertCustomBox( customBox5 );

    KDChart1TextPiece textPiece6( "<qt><p align=right><nobr><b>right</b> X: <small>small</small> <big>big</big><nobr></p></qt>",
                                 QFont( "times", 6, QFont::Normal, false ) );
    KDChart1CustomBox customBox6(
        textPiece6,
        -18,true,
        0,0,
        -200,-50,
        Qt::darkBlue,
        Qt::NoBrush,
        KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosBottom,
        KDChart1Enums::PosBottomRight,
        Qt::AlignBottom + Qt::AlignRight,
        0,0,0,
        Qt::AlignTop + Qt::AlignRight );
    _p->insertCustomBox( customBox6 );


    //
    // KDChart1TextPiece textPiece7( "%", QFont( "times", 1, QFont::Normal, false ) );
    // KDChart1CustomBox customBox7(
    //         textPiece7,
    //         -32,true,
    //         0,0,
    //         -165,-275,
    //         Qt::darkBlue,
    //         Qt::NoBrush,
    //         KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosLeft,
    //         KDChart1Enums::PosTopLeft,
    //         Qt::AlignTop + Qt::AlignLeft );
    // _p->insertCustomBox( customBox7 );
    //
    _p->setHeader1Text( "Line plot on isometric cartesian coordinate system");

    _p->setHeader2Text( "");//- data generated based on a user-defined function -" );

    _p->setFooterText(  "");//"[ a Qt::NoPen property causes the gap between the figures ]");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            40 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "helvetica", 24, QFont::Bold, false ),
            true,
            18 );

    _p->setPrintDataValues( false );

    // calculate the points
    int i=0;
    double x,y;
    for(double t = minT;  t <= maxT;  t += stepT, ++i){
        functionA( t, x,y );
        _d->setCell( 0, i,   y,x );
        functionB( t, x,y );
        _d->setCell( 1, i,   y,x );
        functionC( t, x,y );
        _d->setCell( 2, i,   y,x );
    }

    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_38()
{
    // "two chart sharing one abscissa"
    QColor myBack( 218,255,240 );
    resetData( 5, 5 );

    const QPixmap pixUSD( (const char **)xpmUSD );
    const QPixmap pixKlaralven( (const char **)xpmKlaralven );


    QPixmap pixBrushUSD( pixUSD );
    pixBrushUSD.fill( myBack );
    {
        const QRect r( QPoint(0,0), pixBrushUSD.size() );
        QPainter painter( &pixBrushUSD );
        painter.drawPixmap( r, pixUSD, r );
    }
    resetParams( KDChart1Params::Bar,
                KDChart1Params::Line,
                QPen( Qt::blue ),
                QBrush( myBack ) );//, pixBrushUSD ) );

    const QColor myBackDark115( myBack.dark(115) );

    _p->setSimpleFrame( KDChart1Enums::AreaOutermost,
                        0,0,  0,0,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( Qt::blue ),
                        QBrush( myBackDark115 ) );

    _p->setSimpleFrame( KDChart1Enums::AreaInnermost,
                        0,0, 0,0,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( Qt::blue ),
                        QBrush( myBack, pixBrushUSD ) );

    _p->setSimpleFrame( KDChart1Enums::AreaData,
                        0,0, -1,-1,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( Qt::black ),
                        // a:
                        //QBrush(__chamois ) );
                        // b:
                        QBrush( __chamois ),
                        &pixUSD,
                        KDFrame1::PixStretched ); // enum: PixCentered, PixScaled, PixStretched

    // coloring of all-headers area
    QColor myHead(218,240,255 );
    _p->setSimpleFrame( KDChart1Enums::AreaHeaders,
                        0,0, 0,0,//-15,3,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( Qt::darkBlue ),
                        QBrush( myHead ) );
    _p->setHeaderFooterColor( KDChart1Params::HdFtPosHeader,
                            Qt::darkBlue );
    _p->setHeaderFooterColor( KDChart1Params::HdFtPosHeader2,
                            myHead.dark() );

    // coloring of all-footers area
    _p->setSimpleFrame( KDChart1Enums::AreaFooters,
                        0,0,  0,0,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( Qt::darkBlue ),
                        QBrush( myHead ) );
    // show each little footer area
    for( int iHdFt  = KDChart1Enums::AreaHdFtBASE + KDChart1Params::HdFtPosFootersSTART;
        iHdFt <= KDChart1Enums::AreaHdFtBASE + KDChart1Params::HdFtPosFootersEND;
        ++iHdFt )
        _p->setSimpleFrame( iHdFt,
                            0,0,  0,0,
                            true,
                            true,
                            KDFrame1::FrameFlat,
                            1,
                            0,
                            QPen( Qt::red ),
                            QBrush( myHead.dark(115) ) );

    // special coloring and frame of main footer area
    _p->setSimpleFrame( KDChart1Enums::AreaHdFtBASE + KDChart1Params::HdFtPosFooter,
                        0,0,  0,0,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( Qt::darkBlue ),
                        QBrush( myHead.dark(130) ) );
    _p->setHeaderFooterColor( KDChart1Params::HdFtPosFooter,
                            myHead );

    _p->setSimpleFrame( KDChart1Enums::AreaLegend,
                        0,0,  0,0,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( QColor( 0,255,240 ) ),
                        QBrush( __chamois ) );

    _p->setBarChartSubType( KDChart1Params::BarNormal );
    _p->setThreeDBars( true );
    _p->setLineMarker( true );
    _p->setLineMarkerSize( QSize( 9, 9 ) );
    _p->setLineMarkerStyle( 4, KDChart1Params::LineMarkerDiamond );

    KDChart1TextPiece textPiece1( "<qt>Mouse clicks on the BARS or MARKERS will be reported!</qt>",
                                 QFont( "times", 1, QFont::Normal, false ) );
    KDChart1CustomBox customBox1(
        textPiece1,
        -18,true,
        -100,0,
        -2000,-60,
        Qt::darkBlue,
        Qt::NoBrush,//QBrush(Qt::yellow),
        KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosBottom,
        KDChart1Enums::PosBottomLeft,
        Qt::AlignBottom + Qt::AlignLeft );

    _p->insertCustomBox( customBox1 );

#ifndef QWS
    KDChart1TextPiece textPiece2( "<b></b>",
                                 QFont( "times", 1, QFont::Normal, false ) );
    KDChart1CustomBox customBox2(
        textPiece2,
        -22,true,
        5,5,
        static_cast < int > ( pixKlaralven.width() * -1.33 ),
        static_cast < int > ( pixKlaralven.height()* -1.33 ),
        Qt::black,
        Qt::NoBrush,
        KDChart1Enums::AreaLegend,
        KDChart1Enums::PosBottomLeft,
        Qt::AlignTop + Qt::AlignLeft );
    _p->setSimpleFrame( KDChart1Enums::AreaCustomBoxesBASE
                        + _p->insertCustomBox( customBox2 ),
                        0,0,  0,0,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( Qt::red ),
                        QBrush(),
                        &pixKlaralven,
                        KDFrame1::PixStretched );
#endif

    KDChart1TextPiece textPiece3( "<b>MAX</b>",
                                 QFont( "helvetica", 1, QFont::Normal, false ) );
    KDChart1CustomBox customBox3(
        textPiece3,
        -20,true,
        2,0,
        -350,-50,
        Qt::blue,
        Qt::NoBrush,
        KDChart1Enums::AreaChartDataRegion,
        KDChart1Enums::PosTopRight,
        Qt::AlignVCenter + Qt::AlignLeft,
        0, 1 );
    _p->insertCustomBox( customBox3 );

    KDChart1TextPiece textPiece4( "<qt><p align=right><b>MIN</b></p></qt>",
                                 QFont( "helvetica", 1, QFont::Normal, false ) );
    KDChart1CustomBox customBox4(
        textPiece4,
        -20,true,
        2,0,
        -350,-50,
        Qt::blue,
        Qt::NoBrush,
        KDChart1Enums::AreaChartDataRegion,
        KDChart1Enums::PosBottomRight,
        Qt::AlignTop + Qt::AlignRight,
        2, 1 );
    _p->insertCustomBox( customBox4 );

    KDChart1TextPiece textPiece5( "<qt><nobr>some text...</nobr></qt>",
                                 QFont( "times", 1, QFont::Normal, false ) );
    KDChart1CustomBox customBox5(
        textPiece5,
        -22,true,
        0,0,
        -150,-50,
        Qt::darkRed,
        Qt::NoBrush,
        KDChart1Enums::AreaChartDataRegion,
        KDChart1Enums::PosBottomRight,
        Qt::AlignTop + Qt::AlignLeft,
        4, 3 );

    _p->setSimpleFrame( KDChart1Enums::AreaCustomBoxesBASE
                        + _p->insertCustomBox( customBox5 ),
                        0,0,  0,0,
                        true,
                        true,
                        KDFrame1::FrameFlat,
                        1,
                        0,
                        QPen( myHead.dark() ),
                        QBrush( myHead ) );


    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( 0 ); // thin grid lines
    pa.setAxisLineColor( Qt::blue );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( true );
    pa.setAxisUseAvailableSpace( 0, -499 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    pa = _p->axisParams( KDChart1AxisParams::AxisPosRight );
    pa.setAxisGridStyle( Qt::SolidLine );
    pa.setAxisGridLineWidth( 0 ); // thin grid lines
    pa.setAxisLineColor( Qt::red );
    pa.setAxisGridColor( pa.axisLineColor() );
    pa.setAxisShowGrid( true );
    pa.setAxisShowSubDelimiters( true );
    pa.setAxisUseAvailableSpace( -600, -1000 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosRight, pa );

    _p->setDataColor( 0, QColor(  40, 60, 80 ));
    _p->setDataColor( 1, QColor( 110,140,168));
    _p->setDataColor( 2, QColor( 180,218,255));
    _p->setDataColor( 3, QColor( 160, 80, 80));
    _p->setDataColor( 4, QColor( 255, 80, 80));

    static QStringList abscissaNames;
    abscissaNames << "Sunday" << "Monday" << "Tuesday" << "Wednesday"
        << "Thursday" << "Friday" << "Saturday";
    static QStringList abscissaShortNames;
    abscissaShortNames << "Sun" << "Mon" << "Tue" << "Wed"
        << "Thu" << "Fri" << "Sat";
    _p->setAxisLabelTextParams( KDChart1AxisParams::AxisPosBottom,
                                false,
                                "Monday",
                                "Friday",
                                1.0,
                                KDCHART1_AXIS_LABELS_AUTO_DIGITS,
                                KDCHART1_AXIS_IGNORE_EMPTY_INNER_SPAN,
                                KDChart1AxisParams::LabelsFromDataRowNo, 0,
                                &abscissaNames,
                                &abscissaShortNames );

    // modify the legend texts to show the world what we have done :-)
    _p->setLegendSource( KDChart1Params::LegendManual );
    _p->setLegendText( 0, "France" );
    _p->setLegendText( 1, "Germany" );
    _p->setLegendText( 2, "Luxembourg" );

    _p->setLegendText( 3, "Adam Riese" );
    _p->setLegendText( 4, "Ottokar Winzig" );

    _p->setChartSourceMode( KDChart1Params::DataEntry,     0, 2,   0 );
    _p->setChartSourceMode( KDChart1Params::DataEntry,     3, 4,   1 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosLeft,  0, 2,   0 );
    _p->setAxisDatasets( KDChart1AxisParams::AxisPosRight, 3, 4,   1 );

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
                             QFont( "helvetica", 1, QFont::Normal, false ),
                             true,
                             36 );

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
                             QFont( "helvetica", 1, QFont::Bold, false ),
                             true,
                             14 );

    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeader0, "header #0" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeader0L,"left header #0" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeader0R,"right header #0" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeader,  "Combination of two different charts" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeaderL, "left" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeaderR, "right" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeader2, "- same abscissa axis for both charts -" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeader2L,"left #2" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosHeader2R,"right #2" );

    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter0, "footer #0" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter0L,"left footer #0" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter0R,"right footer #0" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter,  "Use similar colors for the ordinate lines and grid lines and their respective data.");
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooterL, "left" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooterR, "right" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter2, "footer #2" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter2L,"left #2" );
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter2R,"right #2" );

    QColor textColor0( Qt::darkBlue );
    QColor textColor1( Qt::darkRed );
    _p->setPrintDataValuesColor( 0, &textColor0 );
    _p->setPrintDataValuesColor( 1, &textColor1 );

    _d->clearAllCells();
    _d->setCell( 0, 0,  2.0 );
    _d->setCell( 0, 1,  4.5 );
    _d->setCell( 0, 2,  4.0 );
    _d->setCell( 0, 3,  3.0 );
    _d->setCell( 0, 4,  2.0 );
    _d->setCell( 1, 0,  4.0 );
    _d->setCell( 1, 1,  2.0 );
    _d->setCell( 1, 2,  3.0 );
    _d->setCell( 1, 3,  2.0 );
    _d->setCell( 1, 4,  3.0 );

    _d->setCell( 2, 0,  0.6 );
    _d->setCell( 2, 1,  0.125 );
    _d->setCell( 2, 2,  0.15 );
    _d->setCell( 2, 3,  0.3 );
    _d->setCell( 2, 4,  0.25 );

    _d->setCell( 3, 0,  2.0 );
    _d->setCell( 3, 1,   0.1 );
    _d->setCell( 3, 2,  3.0 );
    _d->setCell( 3, 3,  1.4 );
    _d->setCell( 3, 4,  2.0 );
    _d->setCell( 4, 0,  -0.9 );
    _d->setCell( 4, 1,  -1.2 );
    _d->setCell( 4, 2,  -0.3 );
    _d->setCell( 4, 3,  0.0 );
    _d->setCell( 4, 4,  1.2 );

    // have special properties for some of the cells
    KDChart1PropertySet highlightedBarProps;
    highlightedBarProps.setName("highlighted bar");
    highlightedBarProps.setBarColor( KDChart1PropertySet::OwnID,
                                    QColor(0xff,0xa0,0) );
    int idHighlightedBarProps = _p->registerProperties( highlightedBarProps );
    KDChart1PropertySet dashLineProps;
    dashLineProps.setName("dash line");
    dashLineProps.setLineStyle( KDChart1PropertySet::OwnID,
                                Qt::DashLine );
    int idDashLineProps = _p->registerProperties( dashLineProps );
    KDChart1PropertySet crossedLinesProps;
    crossedLinesProps.setName("thick yellow cross instead of normal marker");
    crossedLinesProps.setShowMarker( KDChart1PropertySet::OwnID,
                                    false );
    crossedLinesProps.setExtraLinesAlign( KDChart1PropertySet::OwnID,
                                        Qt::AlignHCenter );
    crossedLinesProps.setExtraLinesColor( KDChart1PropertySet::OwnID,
                                        QColor(0xff,0xa0,0) );
    crossedLinesProps.setExtraLinesLength( KDChart1PropertySet::OwnID,
                                        -45 );
    crossedLinesProps.setExtraLinesWidth( KDChart1PropertySet::OwnID,
                                        -125 );
    int idCrossedLinesProps = _p->registerProperties( crossedLinesProps );

    _d->setProp( 0,1,    idHighlightedBarProps );
    _d->setProp( 3,0,    idDashLineProps );
    _d->setProp( 3,2,    idDashLineProps );
    _d->setProp( 4,0,    idDashLineProps );
    _d->setProp( 3,2,    idCrossedLinesProps );

    _p->setDataRegionFrame( 2,1,0,
                            6,6,
                            true,
                            true,
                            KDFrame1::FrameFlat,
                            0,
                            0,
                            QPen( QColor(0xff,0xa0,0), 4, Qt::SolidLine ) );

    _p->setLegendPosition( KDChart1Params::LegendTopLeftLeft );
}


void KDChart1PresentationWidget::setConfiguration_39()
{
    //  "horizontal lines: like Bars",
    //  "horizontal lines: like Gantt",
    const uint nDatasets = 3;
    const uint nItems = 5;
    resetData( nDatasets, nItems );
    resetParams( KDChart1Params::Line );
    _p->setLineStyle( Qt::NoPen );
    _p->setLineMarker( false );

    // activate subdued color set
    _p->setDataSubduedColors( false );  // FALSE == colours NOT sorted according to their values

    _p->setHeader1Text( "Make a Line chart look like a Horizontal Bar chart");

    _p->setHeader2Text( "- hide connecting lines, add horizontal lines, add custom labels  -" );

    _p->setFooterText(  "[ such lines can be drawn from either side to their point ]");
    _p->setHeaderFooterText( KDChart1Params::HdFtPosFooter2,
        "-- here using setExtraLinesAlign( KDChart1PropertySet::OwnID, Qt::AlignLeft ) --");

    _p->setHeaderFooterFont( KDChart1Params::HdFtPosHeader,
            QFont( "times", 24, QFont::Bold, true ),
            true,
            36 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter,
            QFont( "times", 24, QFont::Bold, false ),
            true,
            27 );
    _p->setHeaderFooterFont( KDChart1Params::HdFtPosFooter2,
            QFont( "times", 24, QFont::Bold, false ),
            true,
            21 );

    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosLeft );
    pa.setAxisCalcMode( KDChart1AxisParams::AxisCalcLinear );
    pa.setAxisValuesDecreasing( true );
    pa.setAxisShowSubDelimiters( false );
    pa.setAxisValueStart( 1.0 );
    pa.setAxisValueDelta( 1.0 );
    pa.setAxisValueEnd(   6.0 );
    _p->setAxisParams( KDChart1AxisParams::AxisPosLeft, pa );

    const QColor informationBoxColor( 222,248,255 );

    KDChart1TextPiece textPiece1(
        QString("<qt><table><tr><td bgcolor=%1>"
            "&nbsp;<br>"
            "<u>Note:</u><br>"
            "&nbsp;<br>"
            "Using KD Chart options for showing such horizontal bars<br>"
            "are quite limited, since KD Chart does not support the<br>"
            "Horizontal Bars chart type out-of-the-box.<br>"
            "&nbsp;<br>"
            "In this example we are using cell-specific properties<br>"
            "technic to make it look like a horizontal bar chart,<br>"
            "but things like e.g. labeling can be done in a very<br>"
            "limited way only.<br>"
            "&nbsp;<br>"
            "If you need more sophisticated horizontal bar charting,<br>"
            "please ask for <b>KD Gantt</b>.<br>"
            "That's our full-featured and highly customizable tool<br>"
            "for these kind of jobs: Most-likely it will perfectly<br>"
            "suit your requirements.<br>"
            "&nbsp;"
        "</td></tr></table></qt>").arg( informationBoxColor.name() ),
        QFont( "courier", 1) );
    KDChart1CustomBox informationBox( textPiece1,
            -14,
            true,
            10,
            10,
            0,
            0,
            Qt::black,
            Qt::NoBrush,//QBrush(Qt::white),//
				     KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosLeft/*AreaData*/,
            KDChart1Enums::PosTopLeft,
            Qt::AlignTop | Qt::AlignLeft );
    _p->insertCustomBox( informationBox );

    KDChart1PropertySet myProperties;
    myProperties.setLineStyle(
            KDChart1PropertySet::OwnID,
            Qt::NoPen );
    myProperties.setExtraLinesAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignLeft );
    myProperties.setExtraLinesStyle(
            KDChart1PropertySet::OwnID,
            Qt::SolidLine );
    myProperties.setExtraLinesWidth(
            KDChart1PropertySet::OwnID,
            -40 );
    const int myPropertiesID = _p->registerProperties( myProperties );


    _d->clearAllCells();
    _d->setCell( 0, 0,    1.2,  17.5  );
    _d->setCell( 0, 1,    2.2,  15.0  );
    _d->setCell( 0, 2,    3.2,  12.5  );
    _d->setCell( 0, 3,    4.2,  13.0  );
    _d->setCell( 0, 4,    5.2,  14.0  );

    _d->setCell( 1, 0,    1.5,  11.5  );
    _d->setCell( 1, 1,    2.5,  12.0  );
    _d->setCell( 1, 2,    3.5,  12.5  );
    _d->setCell( 1, 3,    4.5,  13.0  );
    _d->setCell( 1, 4,    5.5,  13.5  );

    _d->setCell( 2, 0,    1.8,  12.25 );
    _d->setCell( 2, 1,    2.8,  12.0  );
    _d->setCell( 2, 2,    3.8,  11.75 );
    _d->setCell( 2, 3,    4.8,  11.5  );
    _d->setCell( 2, 4,    5.8,  11.25 );


    // Note: we can not print the ata values using KD Chart's build-in default way
    //       because our data values here are the X values, while KD Chart would
    //       print the  values.
    // The solution is to manually add small KDChart1CustomBox instances, as shown
    // in the loop below.
    _p->setPrintDataValues( false );

    for( uint iD=0; iD<nDatasets; ++iD )
        for( uint iI=0; iI<nItems; ++iI ){
            // assign the property set ID: This makes the 'bar' appear.
            _d->setProp( iD, iI, myPropertiesID );

            // add a custom box: This makes the data value appear.
            KDChart1TextPiece textPiece(
                QString("<qt>%1</qt>")
                    .arg( _d->cellVal( iD, iI, 2 ).toDouble() ),
                QFont( "helvetica", 1, QFont::Normal, false ) );
            KDChart1CustomBox box(
                textPiece,
                -20,true,
                0,0,  // no y gap, no x gap
                0,0,  // no size: so the size is calculated automatically
                Qt::black,
                Qt::NoBrush,

                // Note: The 'data region' is just the points's position,
                //       but NOT the area covered by our bar - since actually
                //       KD Chart does not know about any 'bar' but it just
                //       draws a horizontally line from our point's position
                //       to the left edge of the data area.
                KDChart1Enums::AreaChartDataRegion,
                KDChart1Enums::PosCenterRight,
                Qt::AlignVCenter + Qt::AlignLeft,
                iD, iI );
            _p->insertCustomBox( box );
    }

    _p->setLegendPosition( KDChart1Params::NoLegend );
}


void KDChart1PresentationWidget::setConfiguration_40()
{
    // "nice full featured chart"
    QColor myBack( 218,255,240 );

    const int maxValues = 200;
    resetData( 2, maxValues );

    const QPixmap pixUSD( (const char **)xpmUSD );
    const QPixmap pixKlaralven( (const char **)xpmKlaralven );

    QPixmap pixBrushUSD( pixUSD );
    pixBrushUSD.fill( myBack );
    {
        const QRect r( QPoint(0,0), pixBrushUSD.size() );
        QPainter painter( &pixBrushUSD );
        painter.drawPixmap( r, pixUSD, r );
    }
    resetParams( KDChart1Params::Line,
                 KDChart1Params::NoType,
                 QPen( Qt::blue ),
                 QBrush( myBack ) );//, pixBrushUSD ) );


    _p->setChartType( KDChart1Params::Line );

    _p->setHeader1Text( "Past and predicted highwater levels on Elbe gauge Torgau" );
    _p->setHeader2Text( "[ forecast made on 2002/08/17, 18:00 ]" );

    _p->setDataColor( 0, Qt::blue );
    _p->setPrintDataValues( false );
    _p->setLineWidth( 3 );
    _p->setLegendPosition( KDChart1Params::NoLegend );

    // we remove the vertical grid sub lines
    KDChart1AxisParams pa;
    pa = _p->axisParams( KDChart1AxisParams::AxisPosBottom );
    pa.setAxisGridSubStyle( Qt::NoPen );
    _p->setAxisParams( KDChart1AxisParams::AxisPosBottom, pa );

    // specify and register the property sets
    KDChart1PropertySet propSetForFutureValue(
            "future value",
            KDCHART1_PROPSET_NORMAL_DATA );
    propSetForFutureValue.setLineStyle(
            KDChart1PropertySet::OwnID, Qt::DotLine );
    const int idProp_Future_Value
            = _p->registerProperties( propSetForFutureValue );

    const QColor colorForCriticalValue( Qt::red );
    KDChart1PropertySet propSetForCriticalValue(
            "critical value",
            KDCHART1_PROPSET_NORMAL_DATA );
    propSetForCriticalValue.setLineColor(
            KDChart1PropertySet::OwnID, colorForCriticalValue );
    // the following two specifications will *not* result in extra
    // lines being shown: they are needed for inheritance by
    // the below specified lastTrueValuePropsCritical only.
    propSetForCriticalValue.setExtraLinesColor(
            KDChart1PropertySet::OwnID, colorForCriticalValue );
    propSetForCriticalValue.setExtraMarkersColor(
            KDChart1PropertySet::OwnID, colorForCriticalValue );
    const int idProp_Critical_Value
                = _p->registerProperties( propSetForCriticalValue );

    KDChart1PropertySet propSetForFutureCriticalValues(
            "future + critical values",
            KDCHART1_PROPSET_NORMAL_DATA );
    propSetForFutureCriticalValues.setLineStyle( idProp_Future_Value,
            Qt::NoPen );  // this 2nd parameter will be ignored
    propSetForFutureCriticalValues.setLineColor( idProp_Critical_Value,
            QColor() );
    const int idProp_FutureCritical_Value
            = _p->registerProperties( propSetForFutureCriticalValues );


    // we add a custom box above the left ordinate axis
    KDChart1TextPiece textPiece1( "<qt>(<i>cm</i>)<p></qt>",
                                 QFont( "helvetica", 1 ) );
    KDChart1CustomBox customBox1(
        textPiece1,
        -800,
        false,
        0,
        0,
        -60,
        -50,
        Qt::darkBlue,
        Qt::NoBrush,
        KDChart1Enums::AreaAxisBASE + KDChart1AxisParams::AxisPosLeft,
        KDChart1Enums::PosTopRight,
        Qt::AlignBottom | Qt::AlignRight,
        0,0,0, // no cell number since not aligning to a data point
        KDCHART1_AlignAuto,
        false );
    _p->insertCustomBox( customBox1 );


    // we add three pixmaps to our chart
    QPixmap pixWave;
    if( pixWave.load("data/waveBig.png") )
        // we display a big wave pixmap scaled to
        // the width or the height of the chart's data area
        _p->setSimpleFrame( KDChart1Enums::AreaData,
                            0,0,  0,0,
                            true,
                            true,
                            KDFrame1::FrameFlat,
                            1,
                            0,
                            Qt::NoPen,
                            Qt::white,
                            &pixWave,
                            KDFrame1::PixScaled );
    if( pixWave.load("data/waveSmall.png") )
        // we show a smaller wave pixmap as background tile
        // covering the left axis and the space above and below it
        _p->setSimpleFrame(
                KDChart1Enums::AreaAxisBASE
                + KDChart1AxisParams::AxisPosLeft,
                0,0,  0,40,
                true,
                true,
                KDFrame1::FrameFlat,
                1,
                0,
                Qt::NoPen,
                QBrush( Qt::black, pixWave ) );
    if( pixWave.load("data/waveSmallFull.png") )
    {
        // we show a non-transparent version of this small wave
        // framed by a thin blue line in the upper right corner
        // of the chart's outer area
        //
        // NOTE:
        // Since a frame needs an area we first insert an empty
        // custom box with the exact pixmap size, this box is
        // then framed by our simple blue line and get the
        // centered background pixmap.

        KDChart1TextPiece textPiece2( "<qt><p></p></qt>",
                                     QFont( "times", 1 ) );
        KDChart1CustomBox customBox2(
            textPiece2,
            -22,true,
            1,1,
            pixWave.width(),
            pixWave.height(),
            Qt::black,
            Qt::NoBrush,
            KDChart1Enums::AreaOutermost,
            KDChart1Enums::PosTopRight,
            Qt::AlignTop + Qt::AlignRight );
        _p->setSimpleFrame(
            KDChart1Enums::AreaCustomBoxesBASE
            + _p->insertCustomBox( customBox2 ),
            0,0,  0,0,
            true,
            true,
            KDFrame1::FrameFlat,
            1,
            0,
            QColor(Qt::blue),
            QBrush(Qt::NoBrush),
            &pixWave,
            KDFrame1::PixCentered );
    }

    // we compose a multi-colored table explaining the meaning
    // of the vertical and horizontal extra lines
    KDChart1TextPiece textPiece3(
                "<qt><table>"
                "<tr>"
                    "<td align=left width=90%>"
                        "indicator lines:"
                    "</td>"
                    "<td>"
                        "<table cellspacing=1, cellpadding=0>"
                            "<tr>"
                                "<td bgcolor=#ff0000>"
                                    "&nbsp;"
                                "</td>"
                                "<td>"
                                    "<nobr>last true value and date</nobr>"
                                "</td>"
                            "</tr>"
                            "<tr>"
                                "<td bgcolor=#800000>"
                                    "&nbsp;"
                                "</td>"
                                "<td>"
                                    "<nobr>alarm level Torgau/Elbe</nobr>"
                                "</td>"
                            "</tr>"
                            "<tr>"
                                "<td bgcolor=#000080>"
                                    "&nbsp;"
                                "</td>"
                                "<td>"
                                    "<nobr>normal water level</nobr>"
                                "</td>"
                            "</tr>"
                        "</table>"
                    "</td>"
                "</tr>"
                "</table></qt>",
		QFont( "helvetica", 11) );
    KDChart1CustomBox customBox3( textPiece3,
            -300,
            false,
            0,
            0,
	    -320,-80,
            Qt::black,
            Qt::NoBrush,
            KDChart1Enums::AreaInnermost,
            KDChart1Enums::PosBottomRight,
            Qt::AlignBottom | Qt::AlignRight,
            0,0,0, // no cell number since not aligning to a data point
            KDCHART1_AlignAuto,
            true );
    _p->insertCustomBox( customBox3 );


    // ********************************************************************
    // set Chart Table Data
    // ********************************************************************

    // Of course a real world application would determine
    // the number of cells dynamically, here we just make
    // our table large enough for the values since we know
    // that we will never read more than 200 data entries.
    int trueUsedValues  = 0;

    double highValue = 0.0;
    int highValueIdx = 0;

    QString sfilename("data/gaugelevels.dat");
    QFile f( sfilename );
    if ( f.open(QIODevice::ReadOnly) ){
        QTextStream t( &f );
        QString s, year, month, day, hour, min, value;

        while ( !t.atEnd() && (trueUsedValues < maxValues) ){
            value = "-9999.0";
            s = t.readLine().simplified();
            //qDebug(s);
            int x0=0;
            int x=s.indexOf(' ');
            if( -1 < x ){
                year = s.mid(x0,x-x0);
                x0=x+1;
                x=s.indexOf(' ',x0);
                if( -1 < x ){
                    month = s.mid(x0,x-x0);
                    x0=x+1;
                    x=s.indexOf(' ',x0);
                    if( -1 < x ){
                        day = s.mid(x0,x-x0);
                        x0=x+1;
                        x=s.indexOf(' ',x0);
                        if( -1 < x ){
                            hour = s.mid(x0,x-x0);
                            x0=x+1;
                            x=s.indexOf(' ',x0);
                            if( -1 < x ){
                                min = s.mid(x0,x-x0);
                                value = s.mid(x+1,20);
                            }
                        }
                    }
                }
            }
            if( !(value.isEmpty() || "-9999.0" == value) ){
                int valInt = value.toInt();
                if( 0 < valInt && 1500 > valInt ){ //skip impossible values
                    QDateTime dt( QDate( year.toInt(),
                                         month.toInt(),
                                         day.toInt() ),
                                  QTime( hour.toInt(),
                                         min.toInt() ) );
                    _d->setCell( 0, trueUsedValues, valInt, dt );
                    if( highValue <= valInt ){
                        highValue    = valInt;
                        highValueIdx = trueUsedValues;
                    }
                    ++trueUsedValues;
                }
            }
        }
        f.close();

        // This increases calculation speed dramatically
        // if only a small part of the cells are shown.
        _d->setUsedCols( trueUsedValues );
    }


    // Now we define two sets of extra lines
    const QColor lastTrueValueLinesColorNormal( _p->dataColor( 0 ) );
    KDChart1PropertySet lastTrueValuePropsNormal(
            "two lines marking the last true value (if normal)",
            KDCHART1_PROPSET_NORMAL_DATA );
    // specify the extra lines and markers:
    lastTrueValuePropsNormal.setExtraLinesAlign(
            KDChart1PropertySet::OwnID, Qt::AlignLeft | Qt::AlignBottom );
    lastTrueValuePropsNormal.setExtraLinesWidth(
            KDChart1PropertySet::OwnID, -4 );
    lastTrueValuePropsNormal.setExtraLinesColor(
            KDChart1PropertySet::OwnID, lastTrueValueLinesColorNormal );
    lastTrueValuePropsNormal.setExtraLinesStyle(
            KDChart1PropertySet::OwnID, Qt::DashLine );
    lastTrueValuePropsNormal.setExtraMarkersAlign(
            KDChart1PropertySet::OwnID, Qt::AlignLeft | Qt::AlignBottom );
    lastTrueValuePropsNormal.setExtraMarkersSize(
            KDChart1PropertySet::OwnID, QSize(-33, -33) );
    lastTrueValuePropsNormal.setExtraMarkersColor(
            KDChart1PropertySet::OwnID, lastTrueValueLinesColorNormal );
    lastTrueValuePropsNormal.setExtraMarkersStyle(
            KDChart1PropertySet::OwnID, KDChart1Params::LineMarkerDiamond );
    int idProp_LastTrueValueNormal
        = _p->registerProperties( lastTrueValuePropsNormal );

    KDChart1PropertySet lastTrueValuePropsCritical(
            "two lines marking the last true value (if critical)",
            idProp_LastTrueValueNormal );
    // color settings are inherited from the idProp_Critical_Value set:
    lastTrueValuePropsCritical.setLineColor(
            idProp_Critical_Value, QColor() );
    lastTrueValuePropsCritical.setExtraLinesColor(
            idProp_Critical_Value, QColor() );
    lastTrueValuePropsCritical.setExtraMarkersColor(
            idProp_Critical_Value, QColor() );
    int idProp_LastTrueValueCritical
        = _p->registerProperties( lastTrueValuePropsCritical );


    // assign the propertys to the cells
    const double alarmLevel = 580.0;
    const QDateTime currentTime(QDate(2002, 8, 17), QTime(17,50));
    bool bLastValueDone = false;
    bool bIsFuture      = false;
    bool bIsCritical    = false;
    QVariant vValY, vValX;
    int propID;
    for( int iCell = 0;  iCell < trueUsedValues;  ++iCell ){
        if( _d->cellContent( 0, iCell, vValY, vValX, propID ) ){
            bIsFuture = vValX.toDateTime() > currentTime;
            if( bIsFuture && iCell && !bLastValueDone ){
                _d->setProp(0, iCell-1, bIsCritical
                                      ? idProp_LastTrueValueCritical
                                      : idProp_LastTrueValueNormal );
                bLastValueDone = true;
            }
            bIsCritical = vValY.toDouble() >= alarmLevel;
            if( bIsFuture || bIsCritical )
                _d->setProp(0, iCell,  bIsFuture
                                    ? (   bIsCritical
                                        ? idProp_FutureCritical_Value
                                        : idProp_Future_Value )
                                    : idProp_Critical_Value );
        }
    }


    // using the high value's cell number found above we add
    // a little black ring around the high value's data point
    KDChart1PropertySet maxWaterLevelValue(
            "a black ring marking the maximal water level",
            idProp_FutureCritical_Value );
    // specify the extra lines and markers:
    maxWaterLevelValue.setExtraLinesAlign(
            KDChart1PropertySet::OwnID, Qt::AlignLeft );
    maxWaterLevelValue.setExtraLinesInFront(
            KDChart1PropertySet::OwnID, true );
    maxWaterLevelValue.setExtraLinesStyle(
            KDChart1PropertySet::OwnID, Qt::NoPen );
    maxWaterLevelValue.setExtraMarkersAlign(
            KDChart1PropertySet::OwnID, Qt::AlignRight );
    maxWaterLevelValue.setExtraMarkersSize(
            KDChart1PropertySet::OwnID, QSize(-30, -30) );
    maxWaterLevelValue.setExtraMarkersColor(
            KDChart1PropertySet::OwnID, Qt::darkGreen );
    maxWaterLevelValue.setExtraMarkersStyle(
            KDChart1PropertySet::OwnID, KDChart1Params::LineMarkerRing );
    int idProp_MaxWaterLevelValue
        = _p->registerProperties( maxWaterLevelValue );

    if( _d->cellCoords( 0, highValueIdx, vValY, vValX ) ){
        _d->setProp(0, highValueIdx, idProp_MaxWaterLevelValue );

        // we add a explanation text to the max data value's point
        QString sDecline
            = QString( "<qt><center><nobr>Last maximal water level:%1 %2 %3 cm</nobr></center></qt>" )
                .arg( vValX.toDate().toString() )
                .arg( vValX.toTime().toString() )
                .arg( QString::number( vValY.toDouble() ) );
        KDChart1TextPiece textPiece4( sDecline,
                                     QFont( "helvetica", 1, QFont::Normal, false ) );
        //pending michel
        KDChart1CustomBox customBox4(
            textPiece4,
            -16,true,
            0,-10,
            0,0, // using two Zeroes for the size of the box activates auto size calculation
            Qt::darkGreen,
            Qt::NoBrush,
            KDChart1Enums::AreaChartDataRegion,
            KDChart1Enums::PosCenter,
            Qt::AlignBottom + Qt::AlignHCenter,
            0, highValueIdx );
        _p->insertCustomBox( customBox4 );
    }

    // Now we define and set two horizontal lines
    const QColor horizLineAColor( Qt::darkBlue );
    KDChart1PropertySet horiLinePropsA(
            "horizontal line at 2.20m level" );
    horiLinePropsA.setLineStyle(KDChart1PropertySet::OwnID, Qt::NoPen);
    horiLinePropsA.setShowMarker(
            KDChart1PropertySet::OwnID,
            false );
    horiLinePropsA.setExtraLinesAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignLeft | Qt::AlignRight );
    horiLinePropsA.setExtraLinesWidth(
            KDChart1PropertySet::OwnID,
            -8 );
    horiLinePropsA.setExtraLinesColor(
            KDChart1PropertySet::OwnID,
            horizLineAColor );
    horiLinePropsA.setExtraLinesStyle(
            KDChart1PropertySet::OwnID,
            Qt::DotLine );
    horiLinePropsA.setExtraMarkersAlign(
            KDChart1PropertySet::OwnID,
            Qt::AlignLeft | Qt::AlignRight );
    horiLinePropsA.setExtraMarkersSize(
            KDChart1PropertySet::OwnID,
            QSize(-40, -40) );
    horiLinePropsA.setExtraMarkersColor(
            KDChart1PropertySet::OwnID,
            horizLineAColor );
    horiLinePropsA.setExtraMarkersStyle(
            KDChart1PropertySet::OwnID,
            KDChart1Params::LineMarkerDiamond );
    int idHoriLinePropsA
        = _p->registerProperties( horiLinePropsA );

    const QColor horizLineBColor( Qt::darkRed );
    KDChart1PropertySet horiLinePropsB(
            "horizontal line at 5.80m level",
            idHoriLinePropsA );
    horiLinePropsB.setExtraLinesStyle(
            KDChart1PropertySet::OwnID,
            Qt::SolidLine );
    horiLinePropsB.setExtraLinesColor(
            KDChart1PropertySet::OwnID,
            horizLineBColor );
    horiLinePropsB.setExtraMarkersColor(
            KDChart1PropertySet::OwnID,
            horizLineBColor );
    int idHoriLinePropsB
        = _p->registerProperties( horiLinePropsB );

    _d->setCell( 1, 0,

                220.0,
                QDateTime(QDate(2002, 8, 15), QTime( 12, 0 )) );
    _d->setCell( 1, 1,

                580.0,
                QDateTime(QDate(2002, 8, 15), QTime( 13, 0 )) );

    _d->setProp( 1, 0,    idHoriLinePropsA );
    _d->setProp( 1, 1,    idHoriLinePropsB );


    // two additional indicator bars:

    // We define two little, dark green indicator bars
    // at the Y position (and at the X position, resp.)
    // of the high level point, that was determined previously.
    if( _d->cellCoords( 0, highValueIdx, vValY, vValX ) ){
        const QColor indicatorBarColor( Qt::darkGreen );

        // specify the horizontal bar:
        KDChart1PropertySet indicatorBarPropsHoriz;
        indicatorBarPropsHoriz.setLineStyle(
                KDChart1PropertySet::OwnID,
                Qt::NoPen);
        indicatorBarPropsHoriz.setShowMarker(
                KDChart1PropertySet::OwnID,
                true );
        indicatorBarPropsHoriz.setMarkerStyle(
                KDChart1PropertySet::OwnID,
                KDChart1Params::LineMarkerCross );
        indicatorBarPropsHoriz.setMarkerSize(
                KDChart1PropertySet::OwnID,
                QSize(10, 10) );
        indicatorBarPropsHoriz.setMarkerColor(
                KDChart1PropertySet::OwnID,
                indicatorBarColor );
        indicatorBarPropsHoriz.setExtraLinesWidth(
                KDChart1PropertySet::OwnID,
                3 );
        indicatorBarPropsHoriz.setExtraLinesColor(
                KDChart1PropertySet::OwnID,
                indicatorBarColor );
        indicatorBarPropsHoriz.setExtraLinesStyle(
                KDChart1PropertySet::OwnID,
                Qt::SolidLine );
        indicatorBarPropsHoriz.setExtraLinesAlign(
                KDChart1PropertySet::OwnID,
                Qt::AlignHCenter );
        // because these extra lines are centered, they do not touch
        // the edges of the data area, so they need an explicit size:
        indicatorBarPropsHoriz.setExtraLinesLength(
                KDChart1PropertySet::OwnID,
                -50 );  // -50 used with AlignHCenter means: 50 per Mille of the data area's width

        indicatorBarPropsHoriz.setExtraMarkersAlign(
                KDChart1PropertySet::OwnID,
                Qt::AlignLeft | Qt::AlignRight );
        indicatorBarPropsHoriz.setExtraMarkersSize(
                KDChart1PropertySet::OwnID,
                QSize(0, 10) );  // these two 'cross' type markers shall have no horizontal 'arms'
        indicatorBarPropsHoriz.setExtraMarkersColor(
                KDChart1PropertySet::OwnID,
                indicatorBarColor );
        indicatorBarPropsHoriz.setExtraMarkersStyle(
                KDChart1PropertySet::OwnID,
                KDChart1Params::LineMarkerCross );

        int idIndicatorBarPropsHoriz
            = _p->registerProperties( indicatorBarPropsHoriz );

        // specify the vertical bar, re-using the
        // specification of the horizontal one:
        KDChart1PropertySet indicatorBarPropsVert( "", idIndicatorBarPropsHoriz );
        indicatorBarPropsVert.setExtraLinesAlign(
                KDChart1PropertySet::OwnID,
                Qt::AlignVCenter );
        indicatorBarPropsVert.setExtraLinesLength(
                KDChart1PropertySet::OwnID,
                -150 );  // -150 used with AlignVCenter means: 150 per Mille of the data area's height
        indicatorBarPropsVert.setExtraMarkersAlign(
                KDChart1PropertySet::OwnID,
                Qt::AlignTop | Qt::AlignBottom );
        indicatorBarPropsVert.setExtraMarkersSize(
                KDChart1PropertySet::OwnID,
                QSize(10, 0) );  // these two 'cross' type markers shall have no vertical 'arms'

        int idIndicatorBarPropsVert
            = _p->registerProperties( indicatorBarPropsVert );

        _d->setCell(1, 2, vValY,
                          vValX.toDateTime().addDays( 4 ) );
        _d->setProp(1, 2, idIndicatorBarPropsHoriz );

        _d->setCell(1, 3, vValY.toDouble()-250.0,
                          vValX );
        _d->setProp(1, 3, idIndicatorBarPropsVert );
    }
}
