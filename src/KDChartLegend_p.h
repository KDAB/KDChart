#ifndef KDCHARTLEGEND_P_H
#define KDCHARTLEGEND_P_H

#include "KDChartLegend.h"
#include "KDChartArea_p.h"
#include <KDChartTextAttributes.h>
#include <KDChartMarkerAttributes.h>
#include <QMap>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QVector>
class QGridLayout;
class KDTextDocument;
class QTextDocument;

namespace KDChart {
    class AbstractDiagram;
    class DiagramObserver;
    class LayoutItem;
}

using KDChart::AbstractDiagram;

class KDChart::Legend::Private : public KDChartArea::Private
{
    friend class KDChart::Legend;
public:
    Private();
    ~Private();

private:
    // user-settable
    LegendPosition position;
    Qt::Orientation orientation;
    bool showLines;
    QMap<uint,QString> texts;
    QMap<uint,QBrush> brushes;
    QMap<uint,QPen> pens;
    QMap<uint, MarkerAttributes> markerAttributes;
    TextAttributes textAttributes;
    QString titleText;
    TextAttributes titleTextAttributes;
    uint spacing;
    KDChart::AbstractDiagram* diagram;

    // internal
    QRectF rect;
    mutable QStringList modelLabels;
    mutable QList<QBrush> modelBrushes;
    mutable QList<QPen> modelPens;
    mutable QList<MarkerAttributes> modelMarkers;
    QVector<KDChart::LayoutItem*> layoutItems;
    QGridLayout* layout;
    KDChart::DiagramObserver* observer;
    bool blockBuildLegend;
};

inline KDChart::Legend::Legend( Private* p, QWidget* parent )
    : KDChartArea( p, parent ) { init(); }
inline KDChart::Legend::Private * KDChart::Legend::d_func()
{ return static_cast<Private*>( KDChartArea::d_func() ); }
inline const KDChart::Legend::Private * KDChart::Legend::d_func() const
{ return static_cast<const Private*>( KDChartArea::d_func() ); }




#endif /* KDCHARTLEGEND_P_H */

