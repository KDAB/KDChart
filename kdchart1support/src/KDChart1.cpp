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
#if defined KDAB_EVAL
#include "../evaldialog/evaldialog.h"
#endif

/**
  \dontinclude KDChart1Painter.h
  */
#include <KDChart1.h>
#include <KDChart1Painter.h>
#include <KDChart1Params.h>
#include <KDChart1Global.h>
#include <KDChart1AxisParams.h>

#include <qglobal.h>
#include <qpainter.h>
#include <qpaintdevice.h>

#ifdef QSA
#include <qsinterpreter.h>
#include "KDChart1WrapperFactory.h"
#include "KDChart1ObjectFactory.h"
#endif

/**
  \class KDChart1 KDChart1.h

  \brief Provides a single entry-point to the charting engine for
  applications that wish to provide their own QPainter.

  It is not useful to instantiate this class as it contains
  static methods only.

  \note If for some reason you are NOT using the
  KDChart1Widget class but calling the painting methods of KDChart1 directly,
  you probably will also use the KDChart1DataRegionList class:
  This class is derived from QPtrList, so all of the Qt documentation
  for this class is valid for KDChart1DataRegionList too, e.g. freeing
  of the pointers stored can either be done automatically or
  manually - so PLEASE take the time to read the reference information for this class!

  \sa KDChart1Widget, KDChart1DataRegionList
  */

KDChart1Params* KDChart1::oldParams = 0;
KDChart1Painter* KDChart1::cpainter = 0;
KDChart1Painter* KDChart1::cpainter2 = 0;
KDChart1Params::ChartType KDChart1::cpainterType = KDChart1Params::NoType;
KDChart1Params::ChartType KDChart1::cpainterType2 = KDChart1Params::NoType;

/**
  A global function that cleans up possible KDChart1Painter objects at
  application shutdown.
  */
void cleanupPainter();


bool hasCartesianAxes( KDChart1Params::ChartType chartType )
{
    switch( chartType ){
        case KDChart1Params::NoType:     return false;
        case KDChart1Params::Bar:        return true;
        case KDChart1Params::Line:       return true;
        case KDChart1Params::Area:       return true;
        case KDChart1Params::Pie:        return false;
        case KDChart1Params::HiLo:       return true;
        case KDChart1Params::Ring:       return false;
        case KDChart1Params::Polar:      return false; // Polar axes are NO cartesian axes!
        case KDChart1Params::BoxWhisker: return true;
        default:
                                        qDebug("\n\n\n\nKDCHART1 ERROR: Type missing in KDChart1.cpp hasCartesianAxes()\n"
                                                "=============================================================\n"
                                                "=============================================================\n\n\n\n");
    }
    return false;
}


/**
  Calculates the drawing area from a given QPainter.

  Use this function to get a QRect that you may pass to
  KDChart1::setupGeometry() if you need to know the positions and
  sizes of the axis areas and/or the data area *before* drawing
  the chart.  After calling KDChart1::setupGeometry() you may use
  KDChart1Params::axisArea() and/or KDChart1Params::dataArea()
  to retrieve the desired information.

  \return True if the painter was valid and the drawing area
  could be calculated successfully, else false.
  */
bool KDChart1::painterToDrawRect( QPainter* painter, QRect& drawRect )
{
    if( painter ){
        drawRect = QRect( 0, 0, painter->device()->width(), painter->device()->height() );
        drawRect.setWidth(  drawRect.width() -2 );
        drawRect.setHeight( drawRect.height()-2 );
        return true;
    }else{
        drawRect = QRect( QPoint(0,0), QSize(0,0) );
        qDebug("ERROR: KDChart1Painter::painterToDrawRect() was called with *no* painter.");
        return false;
    }
}


/**
  Calculates the axis and data area rects of a chart with the
  specified parameters on the specified painter.

  \note Call this function if you need to know the positions and
  sizes of the axis areas and/or the data area *before* drawing
  the chart.  After calling this function you may use
  KDChart1Params::axisArea() and/or KDChart1Params::dataArea()
  to retrieve the desired information.

  To get the right drawing area from a given QPainter please
  use the static method KDChart1::painterToDrawRect().

  \param painter the painter that is eventually to be used for drawing
  \param params the parameters defining the chart
  \param data the data that should be displayed as a chart
  \param drawRect the position and size of the drawing area to be used
  */
bool KDChart1::setupGeometry( QPainter* painter,
                             KDChart1Params* params,
                             KDChart1TableDataBase* data,
                             const QRect& drawRect )
{
//qDebug("INVOKING: KDChart1::setupGeometry()");
    if( !params ){
        qDebug("ERROR: setupGeometry::paint() was called with *no* params.");
        return false;
    }
    if( !data ){
        qDebug("ERROR: setupGeometry::paint() was called with *no* data.");
        return false;
    }
    // don't crash due to memory problems when running on windows
#ifdef Q_WS_WIN
    QPixmap::setDefaultOptimization(QPixmap::MemoryOptim);
#endif

    // Install a cleanup routine that is called when the Qt
    // application shuts down and cleans up any potentially still
    // existing painters. Only do this once.
    static bool bFirstCleanUpInstall = true;
    if( bFirstCleanUpInstall ) {
        bFirstCleanUpInstall = false;
        qAddPostRoutine( cleanupPainter );
    }

    // Check whether last call of this methode gave us the same params pointer.
    // If params changed we must create new painter(s).
    bool paramsHasChanged = ( params != oldParams );
    if( paramsHasChanged )
        oldParams = params;

    // Check whether there already is painter and, if that is the
    // case, whether the painter still has the correct type (the chart
    // type might have changed in the meantime).
    if ( paramsHasChanged || !cpainter || cpainterType != params->chartType() )
    {
        delete cpainter; /* save, since always 0 if there was not yet
                            a chart painter */
        // create a new painter
        cpainter = KDChart1Painter::create( params, false );
        cpainterType = params->chartType();
    }

    // Check whether there already is a 2nd painter and, if that is the
    // case, whether the painter still has the correct type (the
    // additional chart type might have changed in the meantime).
    if ( paramsHasChanged || !cpainter2 || cpainterType2 != params->additionalChartType() )
    {
        delete cpainter2; /* save, since always 0 if there was not yet
                             a chart painter */
        // create a new painter
        if( hasCartesianAxes( params->chartType() )
                && hasCartesianAxes( params->additionalChartType() ) ){
            cpainter2 = KDChart1Painter::create( params, true );
            cpainterType2 = params->additionalChartType();
        }else{
            cpainter2 = 0;
            cpainterType2 = KDChart1Params::NoType;
        }
    }

    if ( cpainter ){  // can be 0 if no exceptions are used
        cpainter->setupGeometry( painter, data, drawRect );
    }

    if ( cpainter2 ){  // can be 0 if no exceptions are used
        cpainter2->setupGeometry( painter, data, drawRect );
    }

    return true;
}

/**
  Paints a chart with the specified parameters on the specified
  painter.

  \note If you are passing \c regions pointer, KD Chart will call
  the \c clear() method on it, to delete any regions that might
  still be registered from previous painting.
  Make sure to copy any regions information into your own, private
  data structure, in case you need to keep track of region information,
  that was valid for such previous times.

  \param painter the QPainter onto which the chart should be painted
  \param params the parameters defining the chart
  \param data the data that should be displayed as a chart
  \param regions if not null, this points to a
  KDChart1DataRegionList that will be filled with the regions
  of the data segments. This information is needed internally for both
  recognizing the data segment when reporting mouse clicks and
  for finding the correct position to draw the respective data value texts.
  \param rect the position and size of the drawing area to be used,
  if this parameter is zero the painter's device metrics will be used.
  \param mustCalculateGeometry may be set to false if paint() is called
  immediately after a previous call of setupGeometry() to save some
  time in case you have specified a lot of data cells.
  */
void KDChart1::paint( QPainter*              painter,
                     KDChart1Params*         paraParams,
                     KDChart1TableDataBase*  paraData,
                     KDChart1DataRegionList* regions,
                     const QRect*           rect,
                     bool                   mustCalculateGeometry )
{
//qDebug("KDChart1::paint() mustCalculateGeometry: "+QString(mustCalculateGeometry?"TRUE":"FALSE") );
#if defined KDAB_EVAL
    EvalDialog::checkEvalLicense( "KD Chart" );
#endif

    // delete old contents, to avoid the region from constantly growing
    if( regions )
        regions->clear();

    KDChart1Params*        params = paraParams;
    KDChart1TableDataBase* data   = paraData;
    if( !paraParams && !paraData ){
        qDebug("-----");
        qDebug("Note:  KDChart1::paint() was called without \"params\" and without \"data\".");
        qDebug("-----  Showing a default bar chart.");
        params = new KDChart1Params();
        params->setDatasetGap(3 * params->valueBlockGap());
        params->setPrintDataValues( false );
        params->setLegendPosition( KDChart1Params::NoLegend );
        params->setAxisLabelsVisible( KDChart1AxisParams::AxisPosBottom, false );
        params->setAxisShowGrid( KDChart1AxisParams::AxisPosBottom, false );
        params->setHeader1Text( "KDChart1Widget" );
        data = new KDChart1TableData( 3, 1 );
        // 1st series
        data->setCell( 0, 0,    12.5   );
        // 2nd series
        data->setCell( 1, 0,     8.0   );
        // 3rd series
        data->setCell( 2, 0,    15.0   );
    }

    QRect drawRect;
    bool bOk = true;
    if( mustCalculateGeometry || !cpainter || cpainter->outermostRect().isNull() ){
        if( rect )
            drawRect = *rect;
        else if( !painterToDrawRect( painter, drawRect ) ){
            qDebug("ERROR: KDChart1::paint() could not calculate a drawing area.");
            bOk = false;
        }
        //qDebug("xxx" );
        if( (params || data) && !setupGeometry( painter, params, data, drawRect ) ){
            qDebug("ERROR: KDChart1::paint() could not calculate the chart geometry.");
            bOk = false;
        }
    }else{
        drawRect = cpainter->outermostRect();
    }

    //qDebug("yyy" );

    if( bOk ){
        // Note: the following *must* paint the main-chart first
        //       and the additional chart afterwards
        //       since all axes computations are only done when
        //       the first chart is painted but will be needed for both of course.
        //
        bool paintFirst = true;
        bool paintLast  = ! ( cpainter && cpainter2 );
        if ( cpainter ) {  // can be 0 if no exceptions are used
            //qDebug("zzz" );
            cpainter->paint( painter, data, paintFirst, paintLast, regions, &drawRect, false );

            paintFirst = false;
        }
        paintLast = true;
        if ( cpainter2 )   // can be 0 if no exceptions are used
            cpainter2->paint( painter, data, paintFirst, paintLast, regions, &drawRect, false );
    }

    if( !paraParams && !paraData ){
        delete params;
        delete data;
    }
    KDChart1AutoColor::freeInstance(); // stuff that memory leak
}


/**
  Paints a chart with the specified parameters on the specified
  painter which should use a QPrinter as it's output device.

  This method is provided for your convenience, it behaves
  like the paint() method described above but additionally
  it takes care for the output mode flag: Before painting is
  started the internal optimizeOutputForScreen flag is set
  to FALSE and after painting is done it is restored to
  it's previous value.

  \sa paint
  */
void KDChart1::print( QPainter* painter, KDChart1Params* params,
        KDChart1TableDataBase* data,
        KDChart1DataRegionList* regions,
        const QRect* rect,
        bool mustCalculateGeometry )
{
    bool oldOpt=true;
    if( params ){
        oldOpt = params->optimizeOutputForScreen();
        params->setOptimizeOutputForScreen( false );
    }
    paint( painter, params, data, regions, rect, mustCalculateGeometry );
    if( params )
        params->setOptimizeOutputForScreen( oldOpt );
}


/*
   This method is called at application shut-down and cleans up the
   last created painter.
   */
void cleanupPainter()
{
    delete KDChart1::cpainter;
    delete KDChart1::cpainter2;
    KDChart1::oldParams = 0;
}

#ifdef QSA
void KDChart1::initInterpreter( QSInterpreter* interpreter )
{
    privateInitInterpreter( interpreter );
    interpreter->evaluate( globals() );
}

void KDChart1::initProject( QSProject* project )
{
    project->createScript( QString::fromLatin1( "KDCHART1_Globals" ), globals() );
    privateInitInterpreter( project->interpreter() );
}

QString KDChart1::globals()
{
    QString globals;
    QMap<char*, double> intMap;

    intMap.insert( "KDCHART1_POS_INFINITE", KDCHART1_POS_INFINITE );
    intMap.insert( "KDCHART1_NEG_INFINITE", KDCHART1_NEG_INFINITE );
    intMap.insert( "KDCHART1_AlignAuto", KDCHART1_AlignAuto );
    intMap.insert( "KDCHART1_ALL_AXES", KDCHART1_ALL_AXES );
    intMap.insert( "KDCHART1_NO_AXIS", KDCHART1_NO_AXIS );
    intMap.insert( "KDCHART1_ALL_DATASETS", KDCHART1_ALL_DATASETS );
    intMap.insert( "KDCHART1_NO_DATASET", KDCHART1_NO_DATASET );
    intMap.insert( "KDCHART1_UNKNOWN_CHART", KDCHART1_UNKNOWN_CHART );
    intMap.insert( "KDCHART1_ALL_CHARTS", KDCHART1_ALL_CHARTS );
    intMap.insert( "KDCHART1_NO_CHART", KDCHART1_NO_CHART );
    intMap.insert( "KDCHART1_GLOBAL_LINE_STYLE", KDCHART1_GLOBAL_LINE_STYLE );
    intMap.insert( "KDCHART1_AUTO_SIZE", KDCHART1_AUTO_SIZE );
    intMap.insert( "KDCHART1_DATA_VALUE_AUTO_DIGITS", KDCHART1_DATA_VALUE_AUTO_DIGITS );
    intMap.insert( "KDCHART1_SAGITTAL_ROTATION", KDCHART1_SAGITTAL_ROTATION );
    intMap.insert( "KDCHART1_TANGENTIAL_ROTATION", KDCHART1_TANGENTIAL_ROTATION );
    intMap.insert( "KDCHART1_PROPSET_NORMAL_DATA", KDCHART1_PROPSET_NORMAL_DATA );
    intMap.insert( "KDCHART1_PROPSET_TRANSPARENT_DATA", KDCHART1_PROPSET_TRANSPARENT_DATA );
    intMap.insert( "KDCHART1_PROPSET_HORI_LINE", KDCHART1_PROPSET_HORI_LINE );
    intMap.insert( "KDCHART1_PROPSET_VERT_LINE", KDCHART1_PROPSET_VERT_LINE );
    intMap.insert( "KDCHART1_SAGGITAL_ROTATION", KDCHART1_SAGGITAL_ROTATION );
    intMap.insert( "KDCHART1_CNT_ORDINATES", KDCHART1_CNT_ORDINATES );
    intMap.insert( "KDCHART1_MAX_POLAR_DELIMS_AND_LABELS_POS", KDCHART1_MAX_POLAR_DELIMS_AND_LABELS_POS );
    intMap.insert( "KDCHART1_MAX_AXES", KDCHART1_MAX_AXES );
    intMap.insert( "KDCHART1_AXIS_LABELS_AUTO_DELTA", KDCHART1_AXIS_LABELS_AUTO_DELTA );
    intMap.insert( "KDCHART1_AXIS_LABELS_AUTO_LEAVEOUT", KDCHART1_AXIS_LABELS_AUTO_LEAVEOUT );
    intMap.insert( "KDCHART1_AXIS_LABELS_AUTO_DIGITS", KDCHART1_AXIS_LABELS_AUTO_DIGITS );
    intMap.insert( "KDCHART1_AXIS_GRID_AUTO_LINEWIDTH", KDCHART1_AXIS_GRID_AUTO_LINEWIDTH );
    intMap.insert( "KDCHART1_AXIS_IGNORE_EMPTY_INNER_SPAN", KDCHART1_AXIS_IGNORE_EMPTY_INNER_SPAN );
    intMap.insert( "KDCHART1_DONT_CHANGE_EMPTY_INNER_SPAN_NOW", KDCHART1_DONT_CHANGE_EMPTY_INNER_SPAN_NOW );
    intMap.insert( "DBL_MIN", DBL_MIN );
    intMap.insert( "DBL_MAX", DBL_MAX );

    for( QMapIterator<char*,double> it= intMap.begin(); it != intMap.end(); ++it ) {
        // This is written this way to be efficient
        globals += QString::fromLatin1( "const " );
        globals += it.key();
        globals += " = ";
        globals += QString::number( it.data() );
        globals += ";\n";
    }

    globals += QString::fromLatin1( "const KDCHART1_AXIS_LABELS_AUTO_DATETIME_FORMAT=\"%1\";\n" )
               .arg( QString::fromLatin1( KDCHART1_AXIS_LABELS_AUTO_DATETIME_FORMAT ) );
    globals += QString::fromLatin1( "const KDCHART1_AXIS_LABELS_AUTO_LIMIT = 140319.64;\n" );
    globals += QString::fromLatin1( "const KDCHART1_DEFAULT_AXIS_GRID_COLOR = new Color(\"%1\");\n" )
               .arg(KDCHART1_DEFAULT_AXIS_GRID_COLOR.name());
    globals += QString::fromLatin1( "const KDCHART1_DATA_VALUE_AUTO_COLOR = new Color(\"%1\");\n" )
               .arg( (KDCHART1_DATA_VALUE_AUTO_COLOR)->name());


    QMap<char*,QColor> colorMap;
    colorMap.insert( "Qt.color0", Qt::color0 );
    colorMap.insert( "Qt.color1", Qt::color1 );
    colorMap.insert( "Qt.black", Qt::black );
    colorMap.insert( "Qt.white", Qt::white );
    colorMap.insert( "Qt.darkGray", Qt::darkGray );
    colorMap.insert( "Qt.gray", Qt::gray );
    colorMap.insert( "Qt.lightGray", Qt::lightGray );
    colorMap.insert( "Qt.red", Qt::red );
    colorMap.insert( "Qt.green", Qt::green );
    colorMap.insert( "Qt.blue", Qt::blue );
    colorMap.insert( "Qt.cyan", Qt::cyan );
    colorMap.insert( "Qt.magenta", Qt::magenta );
    colorMap.insert( "Qt.yellow", Qt::yellow );
    colorMap.insert( "Qt.darkRed", Qt::darkRed );
    colorMap.insert( "Qt.darkGreen", Qt::darkGreen );
    colorMap.insert( "Qt.darkBlue", Qt::darkBlue );
    colorMap.insert( "Qt.darkCyan", Qt::darkCyan );
    colorMap.insert( "Qt.darkMagenta", Qt::darkMagenta );
    colorMap.insert( "Qt.darkYellow", Qt::darkYellow );
    for( QMapIterator<char*,QColor> it2= colorMap.begin(); it2 != colorMap.end(); ++it2 ) {
        // This is written this way to be efficient
        globals += QString::fromLatin1( it2.key() );
        globals += QString::fromLatin1( " = new Color( " );
        globals += QString::number( it2.data().red() );
        globals += ',';
        globals += QString::number( it2.data().green() );
        globals += ',';
        globals += QString::number( it2.data().blue() );
        globals += QString::fromLatin1( " );\n" );
    }
    //qDebug( "%s",globals.latin1() );
    return globals;
}

void KDChart1::privateInitInterpreter( QSInterpreter* interpreter )
{
    interpreter->addWrapperFactory( new KDChart1WrapperFactory );
    interpreter->addObjectFactory ( new KDChart1ObjectFactory );
}

#endif
