/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTLEGEND_P_H
#define KDCHARTLEGEND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartAbstractAreaWidget_p.h"
#include "KDChartLegend.h"
#include <KDChartDiagramObserver.h>
#include <KDChartMarkerAttributes.h>
#include <KDChartTextAttributes.h>
#include <QAbstractTextDocumentLayout>
#include <QList>
#include <QPainter>
#include <QVector>

#include <KDABLibFakes>

QT_BEGIN_NAMESPACE
class QGridLayout;
class KDTextDocument;
class QTextDocument;
QT_END_NAMESPACE

namespace KDChart {
class AbstractDiagram;
struct DatasetItem;
class DiagramObserver;
class AbstractLayoutItem;

struct HDatasetItem
{
    HDatasetItem();
    int height() const;

    AbstractLayoutItem *markerLine = nullptr;
    TextLayoutItem *label = nullptr;
    VerticalLineLayoutItem *separatorLine = nullptr;
    QSpacerItem *spacer = nullptr;
};

class DiagramsObserversList : public QList<DiagramObserver *>
{
};

/**
 * \internal
 */
class Legend::Private : public AbstractAreaWidget::Private
{
    friend class Legend;

public:
    Private();
    ~Private() override;

    DiagramObserver *findObserverForDiagram(AbstractDiagram *diagram)
    {
        for (int i = 0; i < observers.size(); ++i) {
            DiagramObserver *obs = observers.at(i);
            if (obs->diagram() == diagram)
                return obs;
        }
        return nullptr;
    }

    void fetchPaintOptions(Legend *q);
    QSizeF markerSize(Legend *q, int dataset, qreal fontHeight) const;
    QSizeF maxMarkerSize(Legend *q, qreal fontHeight) const;
    void reflowHDatasetItems(Legend *q);
    void flowHDatasetItems(Legend *q);
    void destroyOldLayout();

private:
    // user-settable
    const QWidget *referenceArea = nullptr;
    Position position;
    Qt::Alignment alignment;
    Qt::Alignment textAlignment;
    Qt::Alignment legendLineSymbolAlignment;
    RelativePosition relativePosition;
    Qt::Orientation orientation = Qt::Vertical;
    Qt::SortOrder order = Qt::AscendingOrder;
    bool showLines = false;
    QMap<uint, QString> texts;
    QMap<uint, QBrush> brushes;
    QMap<uint, QPen> pens;
    QMap<uint, MarkerAttributes> markerAttributes;
    QList<uint> hiddenDatasets;
    TextAttributes textAttributes;
    QString titleText;
    TextAttributes titleTextAttributes;
    uint spacing = 1;
    bool useAutomaticMarkerSize = true;
    LegendStyle legendStyle = MarkersOnly;

    // internal
    mutable QStringList modelLabels;
    mutable QList<QBrush> modelBrushes;
    mutable QList<QPen> modelPens;
    mutable QList<MarkerAttributes> modelMarkers;
    mutable QSize cachedSizeHint;
    QVector<AbstractLayoutItem *> paintItems;
    QGridLayout *layout;
    QList<HDatasetItem> hLayoutDatasets;
    DiagramsObserversList observers;
};

inline Legend::Legend(Private *p, QWidget *parent)
    : AbstractAreaWidget(p, parent)
{
    init();
}

inline Legend::Private *Legend::d_func()
{
    return static_cast<Private *>(AbstractAreaWidget::d_func());
}

inline const Legend::Private *Legend::d_func() const
{
    return static_cast<const Private *>(AbstractAreaWidget::d_func());
}
}

#endif /* KDCHARTLEGEND_P_H */
