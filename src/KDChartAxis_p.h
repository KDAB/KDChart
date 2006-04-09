#ifndef KDCHARTAXIS_P_H
#define KDCHARTAXIS_P_H

#include "KDChartArea_p.h"
#include <KDChartTextAttributes.h>
#include <KDChartGridAttributes.h>
#include <QMap>
#include <QVariant>
#include <QDateTime>

// see new namespace KDChart::Axis::Private at the very end

class KDChartAxis::Private : public KDChartArea::Private
{
    friend class ::KDChartAxis;
public:
    Private();
    ~Private();

private:
    AxisType type;
    AxisPos position;
    QString title;
    TextAttributes titleTextAttributes;
    QStringList labels;
    QStringList shortLabels;
    TextAttributes labelTextAttributes;
    GridAttributes gridAttributes;
    bool labelsTouchEdges;
    QString startLabel;
    QString endLabel;
    int scaleTo;
    int digitsAfterDecimalPoint;
    QString labelsDateTimeFormat;
    LabelsFromDataRow labelTextsFromDataRow;
    int labelTextsDataRow;
    QString decimalPoint;
    QString thousandsPoint;
    QString labelsPrefix;
    QString labelsPostfix;
    int labelsTotalLen;
    QChar labelsPadFill;
    bool labelsBlockAlign;
    AxisAreaMode areaMode;
    int areaMin;
    int areaMax;
    int trueAreaSize;
    QRectF trueAreaRect;
    int useAvailableSpaceFrom;
    int useAvailableSpaceTo;
    AxisCalcMode calcMode;
    KDChartAxis* referenceAxis;
    bool lineVisible;
    QPen linePen;
    bool valueStartIsExact;
    QVariant startValue;
    QVariant endValue;
    int lowerRangeLimit;
    int upperRangeLimit;
    double valueDelta;
    ValueScale valueDeltaScale;
    int valueLeaveOut;
    bool valuesDecreasing;
    double trueDeltaPixels;
    double trueLow;
    double trueHigh;
    double trueDelta;
    QDateTime trueDtLow;
    QDateTime trueDtHigh;
    ValueScale trueDtScale;
    double zeroLineStartX;
    double zeroLineStartY;
    double dtLowPosX;
    double dtLowPosY;
    double dtHighPosX;
    double dtHighPosY;
    int maxEmptyInnerSpan;
    bool steadyValueCalc;
};

inline KDChartAxis::KDChartAxis( Private * p, QWidget* parent )
    : KDChartArea( p, parent ) { init(); }
inline KDChartAxis::Private * KDChartAxis::d_func()
{ return static_cast<Private*>( KDChartArea::d_func() ); }
inline const KDChartAxis::Private * KDChartAxis::d_func() const
{ return static_cast<const Private*>( KDChartArea::d_func() ); }


////////////////////////////////////////////////////////////////////////////////

// new implementation:
using namespace KDChart;

class Axis::Private
{
    friend class Axis;

public:
    Private();
    ~Private();

    AbstractDiagram* diagram() const;

private:
    AbstractDiagram* parent;
};

inline KDChart::Axis::Axis( Private * p )
    : _d  ( p )
{ init(); }

inline KDChart::Axis::Axis( Private * p, AbstractDiagram* parent_ )
    : _d( p )
{
    _d->parent = parent_;
    init();
}

inline AbstractDiagram* KDChart::Axis::Private::diagram() const
{
    return parent;
}

#endif /* KDCHARTAREA_P_H */

