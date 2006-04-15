#ifndef KDCHARTAXIS_P_H
#define KDCHARTAXIS_P_H

#include "KDChartAbstractArea_p.h"
#include <KDChartTextAttributes.h>
#include <KDChartAbstractDiagram.h>
#include <QMap>
#include <QVariant>
#include <QDateTime>

// see new namespace KDChart::AbstractAxis::Private at the very end

class KDChartAxis::Private : public AbstractArea::Private
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
    : AbstractArea( p, parent ) { init(); }
inline KDChartAxis::Private * KDChartAxis::d_func()
{ return static_cast<Private*>( AbstractArea::d_func() ); }
inline const KDChartAxis::Private * KDChartAxis::d_func() const
{ return static_cast<const Private*>( AbstractArea::d_func() ); }


////////////////////////////////////////////////////////////////////////////////

// new implementation:
namespace KDChart {

class AbstractAxis::Private : public AbstractArea::Private
{
    friend class AbstractAxis;

public:
    Private();
    ~Private();

    AbstractDiagram* diagram() const;

private:
    AbstractDiagram* parent;
};


inline AbstractAxis::AbstractAxis( Private * p, AbstractDiagram* parent_ )
    :  AbstractArea( p, parent_ )
{
    p->parent = parent_;
    init();
}

inline AbstractDiagram* AbstractAxis::Private::diagram() const
{
    return parent;
}

inline AbstractAxis::Private * AbstractAxis::d_func()
{ return static_cast<Private*>( AbstractArea::d_func() ); }
inline const AbstractAxis::Private * AbstractAxis::d_func() const
{ return static_cast<const Private*>( AbstractArea::d_func() ); }

}
#endif /* KDCHARTAREA_P_H */

