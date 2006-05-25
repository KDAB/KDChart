/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/
#ifndef __KDCHARTENUMS_H__
#define __KDCHARTENUMS_H__

#include "KDChartGlobal.h"

#include <QRectF>
#include <QObject>
#include <QVector>

/** \file KDChartEnums.h
  \brief Definition of global enums.
  */

/**
  Project global class providing some enums needed both by KDChartParams
  and by KDChartCustomBox.
  */
class KDCHART_EXPORT KDChartEnums :public QObject
{
    Q_OBJECT
    Q_ENUMS( TextLayoutPolicy )
    Q_ENUMS( AreaName )
    Q_ENUMS( PositionFlag )

public:
    /**
      Text layout policy: what to do if text that is to be drawn would
      cover neighboring text or neighboring areas.

      \li \c LayoutJustOverwrite Just ignore the layout collision and write the text nevertheless.
      \li \c LayoutPolicyRotate Try counter-clockwise rotation to make the text fit into the space.
      \li \c LayoutPolicyShiftVertically Shift the text baseline upwards (or downwards, resp.) and draw a connector line between the text and its anchor.
      \li \c LayoutPolicyShiftHorizontally Shift the text baseline to the left (or to the right, resp.) and draw a connector line between the text and its anchor.
      \li \c LayoutPolicyShrinkFontSize Reduce the text font size.

      \sa KDChartParams::setPrintDataValues
      */
    enum TextLayoutPolicy { LayoutJustOverwrite,
        LayoutPolicyRotate,
        LayoutPolicyShiftVertically,
        LayoutPolicyShiftHorizontally,
        LayoutPolicyShrinkFontSize };

    /**
      Converts the specified text layout policy enum to a
      string representation.

      \param type the text layout policy to convert
      \return the string representation of the text layout policy enum
      */
    static QString layoutPolicyToString( TextLayoutPolicy type );


    /**
      Converts the specified string to a text layout policy enum value.

      \param string the string to convert
      \return the text layout policy enum value
      */
    static TextLayoutPolicy stringToLayoutPolicy( const QString& string );


    /**
      Measure calculation mode: the way how the absolute value of a KDChart::Measure is determined during KD Chart's internal geometry calculation time.

      KDChart::Measure values either are relative (calculated in relation to a given AbstractArea), or they are absolute (used as fixed values).

      Values stored in relative measure always are interpreted as per-mille of a reference area's height (or width, resp.) depending on the orientation set for the KDChart::Measure.

      \li \c MeasureCalculationAbsolute Value set by setValue() is absolute, to be used unchanged.
      \li \c MeasureCalculationAuto Value is relative, KD Chart will automatically determine which reference area to use, and it will determine the orientation too.
      \li \c MeasureCalculationAutoArea Value is relative, Orientation is specified by setOrientation(), and KD Chart will automatically determine which reference area to use.
      \li \c MeasureCalculationAutoOrientation Value is relative, Area is specified by setReferenceArea(), and KD Chart will automatically determine which orientation to use.
      \li \c MeasureCalculationArea Value is relative, the reference area is specified by setReferenceArea(), and orientation specified by setOrientation().

      \sa KDChart::Measure::setCalculationMode
      */
    enum MeasureCalculationMode { MeasureCalculationModeAbsolute,
        MeasureCalculationModeAuto,
        MeasureCalculationModeAutoArea,
        MeasureCalculationModeAutoOrientation,
        MeasureCalculationModeArea };

    /**
      Converts the specified measure calculation mode enum to a
      string representation.

      \param type the measure calculation mode to convert
      \return the string representation of the Measure calculation mode enum
      */
    static QString measureCalculationModeToString( MeasureCalculationMode mode ) {
        switch( mode ) {
            case MeasureCalculationModeAbsolute:
                return QString::fromLatin1("MeasureCalculationModeAbsolute");
            case MeasureCalculationModeAuto:
                return QString::fromLatin1("MeasureCalculationModeAuto");
            case MeasureCalculationModeAutoArea:
                return QString::fromLatin1("MeasureCalculationModeAutoArea");
            case MeasureCalculationModeAutoOrientation:
                return QString::fromLatin1("MeasureCalculationModeAutoOrientation");
            case MeasureCalculationModeArea:
                return QString::fromLatin1("MeasureCalculationModeArea");
            default: // should not happen
        qDebug( "Unknown measure calculation mode" );
        return QString::fromLatin1("JustOverwrite");
        }
    }


    /**
      Converts the specified string to a measure calculation mode enum value.

      \param string the string to convert
      \return the measure calculation mode enum value
      */
    static MeasureCalculationMode stringToMeasureCalculationMode( const QString& string ) {
      if( string == QString::fromLatin1("MeasureCalculationModeAbsolute") )
          return MeasureCalculationModeAbsolute;
      if( string == QString::fromLatin1("MeasureCalculationModeAuto") )
          return MeasureCalculationModeAuto;
      if( string == QString::fromLatin1("MeasureCalculationModeAutoArea") )
          return MeasureCalculationModeAutoArea;
      if( string == QString::fromLatin1("MeasureCalculationModeAutoOrientation") )
          return MeasureCalculationModeAutoOrientation;
      if( string == QString::fromLatin1("MeasureCalculationModeArea") )
          return MeasureCalculationModeArea;
      // default, should not happen
      return MeasureCalculationModeAuto;
    }


    /**
      Areas of the chart that may have their own backgrounds
      and/or may be surrounded by a simple or complex border.

      \li \c AreaData surrounding the data area
      \li \c AreaAxes surrounding the axes but leaving out the data area
      \li \c AreaDataAxes surrounding the data+axes area
      \li \c AreaLegend surrounding the legend area
      \li \c AreaDataAxesLegend surrounding the data+axes+legend area
      \li \c AreaHeaders surrounding the headers area
      \li \c AreaFooters surrounding the footers area
      \li \c AreaDataAxesLegendHeadersFooters surrounding the data+axes+legend+headers+footers area
      \li \c AreaInnermost covering the complete drawing area but <b>not</b> covering the global left/top/right/bottom leading
      \li \c AreaOutermost covering the complete drawing area including the global left/top/right/bottom leading

      \li \c AreaChartDataRegion covering the area used to display one data entry (i.e. one point, bar, line, pie slice,...).
      The respective data coordinates are specified by additional parameters, this is used by
      KDChartCustomBox where you have the parameters \c dataRow, \c dataCol, \c data3rd.

      In addition there is a special value specifying a <b>list</b> of regions:

      \li \c AreasCustomBoxes specifies many small areas surrounding all the custom boxes that you might have added to the chart,
      this is useful in case you want to specify some default frame settings to be used for all custom boxes
      not having frame settings of their own.

      Finally there are three special values that you may use to specify
      a single axis area (or a header/footer area, or a custom box area resp.).
      Just add the number of the axis (or header/footer, or custom box resp.)
      to the respective base value:

      \li \c AreaAxisBASE value to be added to the axis number in case you want to specify a single axis area,
      e.g. for specifying the area of the left ordinate axis just type <b>AreaAxisBASE + AxisPosLeft</b>.
      \li \c AreaHdFtBASE value to be added to the header/footer number in case you want to specify a single header (or footer, resp.) area,
      e.g. for specifying the area of the main header just type <b>AreaHdFtBASE + HdFtPosHeader</b>.
      \li \c AreaCustomBoxBASE value to be added to the number of a custom box that you might have added to your chart,
      e.g. for specifying the area a custom box you have added to the chart
      (let us assume the index of that box is in \c boxIdx1) just type <b>AreaCustBoxBASE + boxIdx1</b>.

      \sa KDChartParams::setSimpleFrame, KDChartParams::setFrame
      \sa KDChartParams::insertCustomBox, KDChartCustomBox
      */
    enum AreaName { AreaUNKNOWN                      = 0x0000,
        AreaData                         = 0x0001,
        AreaAxes                         = 0x0002,
        AreaDataAxes                     = 0x0003,
        AreaLegend                       = 0x0004,
        AreaDataAxesLegend               = 0x0005,
        AreaHeaders                      = 0x0006,
        AreaFooters                      = 0x0007,
        AreaDataAxesLegendHeadersFooters = 0x0008,
        AreaInnermost                    = 0x0009,
        AreaOutermost                    = 0x000a,
        AreaChartDataRegion              = 0x000b,
        AreasCustomBoxes                 = 0x000d,
        AreaAxisBASE                     = 0x1000,
        AreaHdFtBASE                     = 0x2000,
        AreaCustomBoxesBASE              = 0x4000,
        AreaBASEMask                     = 0xF000 };


    /**
      The general position flag to specify a point of
      an area, for example this could be the anchor point
      which an annotation box should be aligned to.

      The following picture shows the different positions:

      \image html "../refman_images/positions.png"
      \image latex "../refman_images/positions.png" "the PositionFlag enum" width=4in

      \note The position and alignment of content to be printed at (or
      inside of, resp.) an area or a point -- like for printing data value texts next
      to their graphical representations (which might be a bar, line, pie slice,...) --
      is specified by two parameters: a \c PositionFlag and a uint holding a combination of \c Qt::AlignmentFlags.
      Remember that Qt::AlignmentFlags are used to specify <b>with which edge</b> something
      is to be aligned to its anchor, e.g. \c AlignLeft means align with the left edge.

      The position of content and the way it is aligned to this
      position is shown in the following drawing, note that annotation #2 and annotation #3
      share the same PositionFlag but have different alignment flags set:

      \image html "../refman_images/alignment.png"
      \image latex "../refman_images/alignment.png" "positioning and aligning" width=4in

      \sa KDChartParams::setPrintDataValues
    */
    enum PositionFlag { PosTopLeft   =0, PosTopCenter   =1, PosTopRight   =2,
        PosCenterLeft=3, PosCenter      =4, PosCenterRight=5,
        PosBottomLeft=6, PosBottomCenter=7, PosBottomRight=8 };


    /**
      Returns the point representing a position of a rectangle.
      */
    static QPointF positionFlagToPoint( const QRectF& rect,
                                        PositionFlag pos )
    {
        QPointF pt;
        if( rect.isValid() ) {
            switch( pos ) {
                case KDChartEnums::PosTopLeft:
                    pt = rect.topLeft();
                    break;
                case KDChartEnums::PosTopCenter:
                    pt.setY( rect.top() );
                    pt.setX( rect.center().x() );
                    break;
                case KDChartEnums::PosTopRight:
                    pt = rect.topRight();
                    break;
                case KDChartEnums::PosCenterLeft:
                    pt.setY( rect.center().y() );
                    pt.setX( rect.left() );
                    break;
                case KDChartEnums::PosCenter:
                    pt = rect.center();
                    break;
                case KDChartEnums::PosCenterRight:
                    pt.setY( rect.center().y() );
                    pt.setX( rect.right() );
                    break;
                case KDChartEnums::PosBottomLeft:
                    pt = rect.bottomLeft();
                    break;
                case KDChartEnums::PosBottomCenter:
                    pt.setY( rect.bottom() );
                    pt.setX( rect.center().x() );
                    break;
                case KDChartEnums::PosBottomRight:
                    pt = rect.bottomRight();
                    break;
            }
        }
        return pt;
    }


    /**
      Returns the point representing a position of a corresponding
      QPointArray.

      \note The array \c points <b>must</b> have at least nine elements.
      */
    static QPointF positionFlagToPoint( const QVector<QPointF>& points,
            PositionFlag pos )
    {
        QPointF pt;
        if( 9 <= points.size() )
            pt = points[ pos ];
        return pt;
    }


    /**
      Converts the specified content position enum to a
      string representation.

      \param type the content position to convert
      \return the string representation of the type enum
      */
    static QString positionFlagToString( PositionFlag type );

    /**
      Converts the specified string to a content position enum value.

      \param string the string to convert
      \return the content position enum value
      */
    static PositionFlag stringToPositionFlag( const QString& string );
};


#endif
