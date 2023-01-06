/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTCHART_P_H
#define KDCHARTCHART_P_H

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

#include <QHBoxLayout>
#include <QObject>
#include <QVBoxLayout>

#include "KDChartAbstractArea.h"
#include "KDChartBackgroundAttributes.h"
#include "KDChartChart.h"
#include "KDChartFrameAttributes.h"
#include "KDChartLayoutItems.h"
#include "KDChartTextArea.h"

#include <KDABLibFakes>

namespace KDChart {

class AbstractAreaWidget;
class CartesianAxis;

/*
  struct PlaneInfo can't be declared inside Chart::Private, otherwise MSVC.net says:
  qhash.h(195) : error C2248: 'KDChart::Chart::Private' : cannot access protected class declared in class 'KDChart::Chart'
  KDChartChart_p.h(58) : see declaration of 'KDChart::Chart::Private'
  KDChartChart.h(61) : see declaration of 'KDChart::Chart'
  KDChartChart.cpp(262) : see reference to class template instantiation 'QHash<Key,T>' being compiled, with
            Key=KDChart::AbstractCoordinatePlane *,
            T=KDChart::Chart::Private::PlaneInfo
*/
/**
 * \internal
 */
struct PlaneInfo
{
    PlaneInfo()
    {
    }
    AbstractCoordinatePlane *referencePlane = nullptr;
    int horizontalOffset = 1;
    int verticalOffset = 1;
    QGridLayout *gridLayout = nullptr;
    QVBoxLayout *topAxesLayout = nullptr;
    QVBoxLayout *bottomAxesLayout = nullptr;
    QHBoxLayout *leftAxesLayout = nullptr;
    QHBoxLayout *rightAxesLayout = nullptr;
};

struct LayoutGraphNode
{
    LayoutGraphNode()
    {
    }
    AbstractCoordinatePlane *diagramPlane = nullptr;
    LayoutGraphNode *leftSuccesor = nullptr;
    LayoutGraphNode *bottomSuccesor = nullptr;
    LayoutGraphNode *sharedSuccesor = nullptr;
    QGridLayout *gridLayout = nullptr;
    bool topAxesLayout = false;
    bool bottomAxesLayout = false;
    bool leftAxesLayout = false;
    bool rightAxesLayout = false;
    int priority = -1;
    bool operator<(const LayoutGraphNode &other) const
    {
        return priority < other.priority;
    }
};

/**
 * \internal
 */
class Chart::Private : public QObject
{
    Q_OBJECT
public:
    Chart *chart;

    enum AxisType
    {
        Abscissa,
        Ordinate
    };
    bool useNewLayoutSystem;
    CoordinatePlaneList coordinatePlanes;
    HeaderFooterList headerFooters;
    LegendList legends;

    QHBoxLayout *layout;
    QVBoxLayout *vLayout;
    QBoxLayout *planesLayout;
    QGridLayout *gridPlaneLayout;
    QGridLayout *headerLayout;
    QGridLayout *footerLayout;
    QGridLayout *dataAndLegendLayout;
    QSpacerItem *leftOuterSpacer;
    QSpacerItem *rightOuterSpacer;
    QSpacerItem *topOuterSpacer;
    QSpacerItem *bottomOuterSpacer;

    QVBoxLayout *innerHdFtLayouts[2][3][3];

    QVector<KDChart::TextArea *> textLayoutItems;
    QVector<KDChart::AbstractLayoutItem *> planeLayoutItems;
    QVector<KDChart::Legend *> legendLayoutItems;

    QSize overrideSize;
    bool isFloatingLegendsLayoutDirty;
    bool isPlanesLayoutDirty;

    // since we do not want to derive Chart from AbstractAreaBase, we store the attributes
    // here and call two static painting methods to draw the background and frame.
    KDChart::FrameAttributes frameAttributes;
    KDChart::BackgroundAttributes backgroundAttributes;

    // ### wrong names, "leading" means inter-line distance of text. spacing? margin?
    int globalLeadingLeft, globalLeadingRight, globalLeadingTop, globalLeadingBottom;

    QList<AbstractCoordinatePlane *> mouseClickedPlanes;

    Qt::LayoutDirection layoutDirection;

    Private(Chart *);

    ~Private() override;

    void createLayouts();
    void updateDirtyLayouts();
    void reapplyInternalLayouts(); // TODO: see if this can be merged with updateDirtyLayouts()
    void paintAll(QPainter *painter);

    struct AxisInfo
    {
        AxisInfo()
        {
        }
        AbstractCoordinatePlane *plane = nullptr;
    };
    QHash<AbstractCoordinatePlane *, PlaneInfo> buildPlaneLayoutInfos();
    QVector<LayoutGraphNode *> buildPlaneLayoutGraph();

public Q_SLOTS:
    void slotLayoutPlanes();
    void slotResizePlanes();
    void slotLegendPositionChanged(AbstractAreaWidget *legend);
    void slotHeaderFooterPositionChanged(HeaderFooter *hf);
    void slotUnregisterDestroyedLegend(Legend *legend);
    void slotUnregisterDestroyedHeaderFooter(HeaderFooter *headerFooter);
    void slotUnregisterDestroyedPlane(AbstractCoordinatePlane *plane);
};
}

#endif
