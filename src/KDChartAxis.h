  /* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005 KlarÃ£Â£Ã¢Â¤lvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart licenses may use this file in
 ** accordance with the KDChart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTAXIS_H
#define KDCHARTAXIS_H

#include <QObject>
#include <QRectF>
#include <QtXml/QDomDocumentFragment>

#include "kdchart_export.h"
#include "KDChartGlobal.h"

class QPainter;
class QSizeF;
class QRectF;


namespace KDChart {

    class TextAttributes;
    class Area;
    class CoordinatePlane;
    class PaintContext;
    class AbstractDiagram;

    class KDCHART_EXPORT Axis
    // : public KDChartArea FIXME maybe reintroduce later
    {
        Q_DISABLE_COPY( Axis )
        KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC( Axis )
    protected:
        explicit inline Axis( Private *p, AbstractDiagram* parent = 0 );
    public:
        explicit Axis( AbstractDiagram* parent = 0 );
        virtual ~Axis();

        // FIXME implement when code os ready for it:
        // virtual Area* clone() const = 0;

        // FIXME (Mirko) readd when needed
        // void copyRelevantDetailsFrom( const KDChartAxis* axis );

        virtual void paint( PaintContext* ) const = 0;
        virtual QSize sizeHint() const = 0;
        virtual void setGeometry( const QRectF& rect ) = 0;
        virtual QRectF geometry() const = 0;

        virtual QDomDocumentFragment toXML() const = 0;
    };
}


// all the following will be merged into the namespaced class and finally
// removed:

#include "KDChartArea.h"
#include "KDChartPaintContext.h"
#include <QString>
#include <QChar>

class QStringList;
class QPen;
class QVariant;
class QDateTime;

namespace KDChart {
  class TextAttributes;
  class GridAttributes;
}

using KDChart::TextAttributes;
using KDChart::GridAttributes;


class KDCHART_EXPORT KDChartAxis : public KDChartArea
{
    Q_DISABLE_COPY( KDChartAxis )
    KDCHART_DECLARE_PRIVATE_DERIVED_QWIDGET( KDChartAxis )
public:
    KDChartAxis( QWidget* );
    virtual ~KDChartAxis();

    // PENDING(kalle) Review
//    virtual KDChartArea * clone() const;

    void copyRelevantDetailsFrom( const KDChartAxis* axis );

    // PENDING(kalle,miroslav) Review
    virtual void paint( QPainter *, const PaintContext & context ) const;
    virtual void setGeometry( const QRect& rect, double averageValueP1000 );
    virtual QRect geometry() const;

    virtual QDomDocumentFragment toXML() const;

/**
  Type of the axis.

  In two-dimentional charts axes can be either X ("abscissa") or Y ("ordinate").

  Three-dimensional diagrams however use a Z axis too.

  \note While in a 3-dimensional chart the Z axis nearly allways is used to
  indicate the data value, both the X axis, and the Y axis, can be just counted,
  but they can also be used to indicate values.

  Therefor, while in 2-dimensional charts a DisplayRoleCoordY value is needed,
  for a 3-dimensional chart, you must specify a DisplayRoleCoordZ value for
  your data cells - otherwise KD Chart would not know how to display them
  and would just ommit them - no matter if the other coordinate's values
  are set or not.

  Also note then Polar charts are using two axes as well:
  \li AxisPosSagittal are leading from the center point of the chart to the outside.
  \li AxisPosCircular are being drawn around the outer edge of the chart.
*/
    enum AxisType { AxisTypeUnknown,
                    AxisTypeX,
                    AxisTypeY,
                    AxisTypeZ };

/**
  Layout position of the axis.

  In two-dimentional charts axes can be at the bottom side, left side, top side, right side of the data area.

  Three-dimensional diagrams can also have axes along their lower right edge,
  or along their lower left edge, resp.

  \note The final position of the axis is determined by KD Charts geometry management,
  and it depends on several things, e.g. an newer axis that overlapping an older axis
  will be shifted away from the data area's edge to make room for the other axis.
  Older here meaning either: A default left (or bottom, resp.) axis,
  or an axis which was inserted into the KDChartView specified before the newer one.

  Also note then Polar charts are using two axes as well:
  \li AxisPosSagittal are leading from the center point of the chart to the outside.
  \li AxisPosCircular are being drawn around the outer edge of the chart.
*/
    enum AxisPos { AxisPosSTART = 0,

                   // default axis positions
                   AxisPosBottom         = 0,
                   AxisPosSagittal            = 0,  //   <-- for POLAR charts
                   AxisPosLeft           = 1,
                   AxisPosCircular            = 1,  //   <-- for POLAR charts
                   AxisPosLowerRightEdge = 2,

                   // alternative, additional positions:
                   AxisPosTop = 3,
                   AxisPosRight = 4,
                   AxisPosLowerLeftEdge = 5,

                   AxisPosEND = 5 };

    enum LabelsFromDataRow { LabelsFromDataRowYes,
                             LabelsFromDataRowNo,
                             LabelsFromDataRowGuess };

    enum AxisAreaMode { AxisAreaModeFixedSize,
                        AxisAreaModeAutoSize,
                        AxisAreaModeMinMaxSize };

    enum ValueScale { ValueScaleNumerical =   0, // have gaps here to allow specifying of additional scaling steps in between
                      ValueScaleSecond    =  20,
                      ValueScaleMinute    =  30,
                      ValueScaleHour      =  40,
                      ValueScaleDay       =  50,
                      ValueScaleWeek      =  60,
                      ValueScaleMonth     =  70,
                      ValueScaleQuarter   =  80,
                      ValueScaleYear      =  90 };

    void setType( AxisType axisType );
    AxisType type() const;

    void setPosition( AxisPos position );
    AxisPos position() const;
    virtual bool hasVerticalPosition() const;
    virtual bool hasHorizontalPosition() const;

    void setTitle( const QString & title );
    QString title() const;

    void setTitleTextAttributes( const TextAttributes & );
    TextAttributes titleTextAttributes() const;

    void setLabels( const QStringList &labels );
    QStringList labels() const;

    void setShortLabels( const QStringList &shortLabels );
    QStringList shortLabels() const;

    void setLabelTextAttributes( const TextAttributes & );
    TextAttributes labelTextAttributes() const;

    void setGridAttributes( const GridAttributes & );
    GridAttributes gridAttributes() const;

    void setAxisLabelsTouchEdges( bool labelsTouchEdges );
    bool axisLabelsTouchEdges() const;

    void setStartLabel( const QString& startLabel );
    QString startLabel() const;

    void setEndLabel( const QString& endLabel );
    QString endLabel() const;

    void setLabelFormat( int scaleTo , int digitsAfterDecimalPoint );
    int labelScale() const;
    int digitsAfterDecimalPoint() const;

    void setLabelsDateTimeFormat( const QString& format );
    QString labelsDateTimeFormat() const;

    void setLabelTextsFromDataRow( int row, LabelsFromDataRow mode );
    LabelsFromDataRow axisLabelTextsFromDataRow() const;
    int labelTextsDataRow() const;

    // FIXME this is per chart, if not per system, and should maybe be a global config option
    // PENDING(kalle) Or maybe even using QLocale
    void setAxisLabelsRadix( const QString& decimalPoint,
                             const QString& thousandsPoint );
    QString axisLabelsDecimalPoint()   const;
    QString axisLabelsThousandsPoint() const;

    void setAxisLabelsFormat( const QString& prefix = QString::null,
                              const QString& postfix = QString::null,
                              int      totalLen = 0, // needed?
                              const QChar&   padFill = QChar::fromAscii( ' ' ),
                              bool     blockAlign = true ); // needed ?
    QString axisLabelsPrefix()         const;
    QString axisLabelsPostfix()        const;
    int axisLabelsTotalLen()           const;
    QChar axisLabelsPadFill()          const;
    bool axisLabelsBlockAlign()        const;

    void setAxisAreaMode( AxisAreaMode areaMode );
    AxisAreaMode axisAreaMode() const;


  // FIXME are these needed publically?
   void setAxisAreaMin( int areaMin );
  //Returns the axis area minimum width (or height, resp.).
    int axisAreaMin() const;
  void setAxisUseAvailableSpace( int useAvailableSpaceFrom,
                                 int useAvailableSpaceTo );
    int axisUseAvailableSpaceFrom() const;
    int axisUseAvailableSpaceTo() const;
  void setAxisAreaMax( int areaMax );
    int axisAreaMax() const;
  void setAxisArea( AxisAreaMode areaMode,
                    int areaMin,
                    int areaMax );


  // FIXME would Scale be better than Calc?
  enum AxisCalcMode { AxisCalcLinear, AxisCalcLogarithmic };
  void setAxisCalcMode( AxisCalcMode calcMode );
  AxisCalcMode axisCalcMode() const;


  void setIsometricReferenceAxis( KDChartAxis *referenceAxis );
  KDChartAxis * isometricReferenceAxis() const;

    void setTrueAreaSize( int trueAreaSize );
    int trueAreaSize() const;
    void setTrueAreaRect( const QRectF& trueAreaRect );
    QRectF trueAreaRect() const;

  // line manipulation
  void setLineVisible( bool lineVisible );
  bool lineVisible() const;

  void setLinePen(  const QPen & pen );
  QPen linePen() const;

  // value manipulation
  void setValueStartIsExact( bool isExactValue=true );
  bool valueStartIsExact() const;
  void setStartValue( const QVariant& axisValueStart );
  QVariant startValue() const;
  void setEndValue( const QVariant& axisValueEnd );
  QVariant endValue() const;

  void setLowerRangeLimit( int lowerLimit );
  int lowerRangeLimit() const;

  void setUpperRangeLimit( int upperLimit );
  int upperRangeLimit() const;

  void setValueDelta( const double valueDelta,
                      ValueScale scale = ValueScaleNumerical );
  double valueDelta() const;
  ValueScale valueDeltaScale() const;

  void setValueLeaveOut( const int leaveOut );
  int valueLeaveOut() const;
  void setValuesDecreasing( bool valuesDecreasing = true );
  bool valuesDecreasing() const;

  // PENDING(khz) do these need to be public? impl detail?
  void setTrueDeltaPixels( double nDeltaPixels );
  double trueDeltaPixels() const;
  void setTrueLowHighDelta( double nLow, double nHigh, double nDelta );
  double trueLow() const;
  double trueHigh() const;
  double trueDelta() const;
  void setTrueDtLowHighDeltaScale( const QDateTime& dtLow, const QDateTime& dtHigh,
                                   ValueScale dtDeltaScale );
  void setTrueDtLow( const QDateTime& dtLow );
  void setTrueDtHigh( const QDateTime& dtHigh );
  void setTrueDtScale( ValueScale scale );
  QDateTime trueDtLow() const;
  QDateTime trueDtHigh() const;
  ValueScale trueDtDeltaScale() const;

    void setZeroLineStart( double x, double y );
    double zeroLineStartX() const;
    double zeroLineStartY() const;
    void setDtLowPos( double x, double y );
    double dtLowPosX() const;
    double dtLowPosY() const;
    void setDtHighPos( double x, double y );
    double dtHighPosX() const;
    double dtHighPosY() const;

    // PENDING(khz) impl detail?
    void setMaxEmptyInnerSpan( const int maxEmpty );
  int axisMaxEmptyInnerSpan() const;

    void setAxisSteadyValueCalc( bool steadyValueCalc );
  bool axisSteadyValueCalc() const;

}; // End of class KDChartAxis

#endif // KDCHARTAXIS_H
