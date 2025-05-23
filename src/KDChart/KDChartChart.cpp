/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "KDChartChart.h"
#include "KDChartChart_p.h"

#include <QApplication>
#include <QEvent>
#include <QGridLayout>
#include <QHash>
#include <QLabel>
#include <QLayoutItem>
#include <QList>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QToolTip>
#include <QtDebug>

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartCartesianCoordinatePlane.h"
#include "KDChartEnums.h"
#include "KDChartHeaderFooter.h"
#include "KDChartLayoutItems.h"
#include "KDChartLegend.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartPrintingParameters.h"
#include <KDChartMarkerAttributes.h>
#include <KDChartTextAttributes.h>

#include <algorithm>

#if defined KDAB_EVAL
#include "../evaldialog/evaldialog.h"
#endif

#include <KDABLibFakes>

#if 0
// dumpLayoutTree dumps a QLayout tree in a hopefully easy to read format to stderr - feel free to
// use, improve and extend; it is very useful for looking at any layout problem.

#include <typeinfo>

// this is this different from both QRect::isEmpty() and QRect::isNull() for "wrong" QRects,
// i.e. those where topLeft() is actually below and / or right of bottomRight().
static bool isZeroArea(const QRect &r)
{
    return !r.width() || !r.height();
}

static QString lineProlog(int nestingDepth, int lineno)
{
    QString numbering(QString::number(lineno).rightJustified(5).append(QChar::fromAscii(':')));
    QString indent(nestingDepth * 4, QLatin1Char(' '));
    return numbering + indent;
}

static void dumpLayoutTreeRecurse(QLayout *l, int *counter, int depth)
{
    const QLatin1String colorOn(isZeroArea(l->geometry()) ? "\033[0m" : "\033[32m");
    const QLatin1String colorOff("\033[0m");

    QString prolog = lineProlog(depth, *counter);
    (*counter)++;

    qDebug() << colorOn + prolog << l->metaObject()->className() << l->geometry()
             << "hint" << l->sizeHint()
             << l->hasHeightForWidth() << "min" << l->minimumSize()
             << "max" << l->maximumSize()
             << l->expandingDirections() << l->alignment()
             << colorOff;
    for (int i = 0; i < l->count(); i++) {
        QLayoutItem *child = l->itemAt(i);
        if (QLayout *childL = child->layout()) {
            dumpLayoutTreeRecurse(childL, counter, depth + 1);
        } else {
            // The isZeroArea check culls usually largely useless output - you might want to remove it in
            // some debugging situations. Add a boolean parameter to this and dumpLayoutTree() if you do.
            if (!isZeroArea(child->geometry())) {
                prolog = lineProlog(depth + 1, *counter);
                (*counter)++;
                qDebug() << colorOn + prolog << typeid(*child).name() << child->geometry()
                         << "hint" << child->sizeHint()
                         << child->hasHeightForWidth() << "min" << child->minimumSize()
                         << "max" << child->maximumSize()
                         << child->expandingDirections() << child->alignment()
                         << colorOff;
            }
        }
    }
}

static void dumpLayoutTree(QLayout *l)
{
    int counter = 0;
    dumpLayoutTreeRecurse(l, &counter, 0);
}
#endif

static const Qt::Alignment s_gridAlignments[3][3] = { // [ row ][ column ]
    {Qt::AlignTop | Qt::AlignLeft, Qt::AlignTop | Qt::AlignHCenter, Qt::AlignTop | Qt::AlignRight},
    {Qt::AlignVCenter | Qt::AlignLeft, Qt::AlignVCenter | Qt::AlignHCenter, Qt::AlignVCenter | Qt::AlignRight},
    {Qt::AlignBottom | Qt::AlignLeft, Qt::AlignBottom | Qt::AlignHCenter, Qt::AlignBottom | Qt::AlignRight}};

static void getRowAndColumnForPosition(KDChartEnums::PositionValue pos, int *row, int *column)
{
    switch (pos) {
    case KDChartEnums::PositionNorthWest:
        *row = 0;
        *column = 0;
        break;
    case KDChartEnums::PositionNorth:
        *row = 0;
        *column = 1;
        break;
    case KDChartEnums::PositionNorthEast:
        *row = 0;
        *column = 2;
        break;
    case KDChartEnums::PositionEast:
        *row = 1;
        *column = 2;
        break;
    case KDChartEnums::PositionSouthEast:
        *row = 2;
        *column = 2;
        break;
    case KDChartEnums::PositionSouth:
        *row = 2;
        *column = 1;
        break;
    case KDChartEnums::PositionSouthWest:
        *row = 2;
        *column = 0;
        break;
    case KDChartEnums::PositionWest:
        *row = 1;
        *column = 0;
        break;
    case KDChartEnums::PositionCenter:
        *row = 1;
        *column = 1;
        break;
    default:
        *row = -1;
        *column = -1;
        break;
    }
}

using namespace KDChart;

// Layout widgets even if they are not visible (that's why isEmpty() is overridden) - at least that
// was the original reason...
class MyWidgetItem : public QWidgetItem
{
public:
    explicit MyWidgetItem(QWidget *w, Qt::Alignment alignment = {})
        : QWidgetItem(w)
    {
        setAlignment(alignment);
    }

    // All of the methods are reimplemented from QWidgetItem, and work around some oddity in QLayout and / or
    // KD Chart - I forgot the details between writing this code as an experiment and committing it, very
    // sorry about that!
    // Feel free to comment out any of them and then try the line-breaking feature in horizontal legends in
    // the Legends/Advanced example. It will not work well in various ways - won't get enough space and look
    // very broken, will inhibit resizing the window etc.

    QSize sizeHint() const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        return w->sizeHint();
    }

    QSize minimumSize() const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        return w->minimumSize();
    }

    QSize maximumSize() const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        return w->maximumSize();
    }

    Qt::Orientations expandingDirections() const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        if (isEmpty()) {
            return {};
        }
        Qt::Orientations e = w->sizePolicy().expandingDirections();
        return e;
    }

    void setGeometry(const QRect &g) override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        w->setGeometry(g);
    }

    QRect geometry() const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        return w->geometry();
    }

    bool hasHeightForWidth() const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        bool ret = !isEmpty() && qobject_cast<Legend *>(w)->hasHeightForWidth();
        return ret;
    }

    int heightForWidth(int width) const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        int ret = w->heightForWidth(width);
        return ret;
    }

    bool isEmpty() const override
    {
        QWidget *w = const_cast<MyWidgetItem *>(this)->widget();
        // legend->hide() should indeed hide the legend,
        // but a legend in a chart that hasn't been shown yet isn't hidden
        // (as can happen when using Chart::paint() without showing the chart)
        return w->isHidden() && w->testAttribute(Qt::WA_WState_ExplicitShowHide);
    }
};

// When "abusing" QLayouts to lay out items with different geometry from the backing QWidgets,
// some manual work is required to correctly update all the sublayouts.
// This is because all the convenient ways to deal with QLayouts assume QWidgets somewhere.
// What this does is somewhat similar to QLayout::activate(), but it never refers to the parent
// QWidget which has the wrong geometry.
static void invalidateLayoutTree(QLayoutItem *item)
{
    QLayout *layout = item->layout();
    if (layout) {
        const int count = layout->count();
        for (int i = 0; i < count; i++) {
            invalidateLayoutTree(layout->itemAt(i));
        }
    }
    item->invalidate();
}

void Chart::Private::slotUnregisterDestroyedLegend(Legend *l)
{
    chart->takeLegend(l);
}

void Chart::Private::slotUnregisterDestroyedHeaderFooter(HeaderFooter *hf)
{
    chart->takeHeaderFooter(hf);
}

void Chart::Private::slotUnregisterDestroyedPlane(AbstractCoordinatePlane *plane)
{
    coordinatePlanes.removeAll(plane);
    for (AbstractCoordinatePlane *p : std::as_const(coordinatePlanes)) {
        if (p->referenceCoordinatePlane() == plane) {
            p->setReferenceCoordinatePlane(nullptr);
        }
    }
    plane->layoutPlanes();
}

Chart::Private::Private(Chart *chart_)
    : chart(chart_)
    , useNewLayoutSystem(false)
    , layout(nullptr)
    , vLayout(nullptr)
    , planesLayout(nullptr)
    , headerLayout(nullptr)
    , footerLayout(nullptr)
    , dataAndLegendLayout(nullptr)
    , leftOuterSpacer(nullptr)
    , rightOuterSpacer(nullptr)
    , topOuterSpacer(nullptr)
    , bottomOuterSpacer(nullptr)
    , isFloatingLegendsLayoutDirty(true)
    , isPlanesLayoutDirty(true)
    , globalLeadingLeft(0)
    , globalLeadingRight(0)
    , globalLeadingTop(0)
    , globalLeadingBottom(0)
{
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            for (int i = 0; i < 2; i++) {
                innerHdFtLayouts[i][row][column] = nullptr;
            }
        }
    }
}

Chart::Private::~Private()
{
}

enum VisitorState
{
    Visited,
    Unknown
};
struct ConnectedComponentsComparator
{
    bool operator()(const LayoutGraphNode *lhs, const LayoutGraphNode *rhs) const
    {
        return lhs->priority < rhs->priority;
    }
};

static QVector<LayoutGraphNode *> getPrioritySortedConnectedComponents(QVector<LayoutGraphNode *> &nodeList)
{
    QVector<LayoutGraphNode *> connectedComponents;
    QHash<LayoutGraphNode *, VisitorState> visitedComponents;
    for (LayoutGraphNode *node : std::as_const(nodeList)) {
        visitedComponents[node] = Unknown;
    }

    for (int i = 0; i < nodeList.size(); ++i) {
        LayoutGraphNode *curNode = nodeList[i];
        LayoutGraphNode *representativeNode = curNode;
        if (visitedComponents[curNode] != Visited) {
            QStack<LayoutGraphNode *> stack;
            stack.push(curNode);
            while (!stack.isEmpty()) {
                curNode = stack.pop();
                Q_ASSERT(visitedComponents[curNode] != Visited);
                visitedComponents[curNode] = Visited;
                if (curNode->bottomSuccesor && visitedComponents[curNode->bottomSuccesor] != Visited)
                    stack.push(curNode->bottomSuccesor);
                if (curNode->leftSuccesor && visitedComponents[curNode->leftSuccesor] != Visited)
                    stack.push(curNode->leftSuccesor);
                if (curNode->sharedSuccesor && visitedComponents[curNode->sharedSuccesor] != Visited)
                    stack.push(curNode->sharedSuccesor);
                if (curNode->priority < representativeNode->priority)
                    representativeNode = curNode;
            }
            connectedComponents.append(representativeNode);
        }
    }
    std::sort(connectedComponents.begin(), connectedComponents.end(), ConnectedComponentsComparator());
    return connectedComponents;
}

struct PriorityComparator
{
public:
    PriorityComparator(QHash<AbstractCoordinatePlane *, LayoutGraphNode *> mapping)
        : m_mapping(mapping)
    {
    }
    bool operator()(AbstractCoordinatePlane *lhs, AbstractCoordinatePlane *rhs) const
    {
        const LayoutGraphNode *lhsNode = m_mapping[lhs];
        Q_ASSERT(lhsNode);
        const LayoutGraphNode *rhsNode = m_mapping[rhs];
        Q_ASSERT(rhsNode);
        return lhsNode->priority < rhsNode->priority;
    }

    const QHash<AbstractCoordinatePlane *, LayoutGraphNode *> m_mapping;
};

void checkExistingAxes(LayoutGraphNode *node)
{
    if (node && node->diagramPlane && node->diagramPlane->diagram()) {
        auto *diag = qobject_cast<AbstractCartesianDiagram *>(node->diagramPlane->diagram());
        if (diag) {
            const auto constAxes = diag->axes();
            for (const CartesianAxis *axis : constAxes) {
                switch (axis->position()) {
                case (CartesianAxis::Top):
                    node->topAxesLayout = true;
                    break;
                case (CartesianAxis::Bottom):
                    node->bottomAxesLayout = true;
                    break;
                case (CartesianAxis::Left):
                    node->leftAxesLayout = true;
                    break;
                case (CartesianAxis::Right):
                    node->rightAxesLayout = true;
                    break;
                }
            }
        }
    }
}

static void mergeNodeAxisInformation(LayoutGraphNode *lhs, LayoutGraphNode *rhs)
{
    lhs->topAxesLayout |= rhs->topAxesLayout;
    rhs->topAxesLayout = lhs->topAxesLayout;

    lhs->bottomAxesLayout |= rhs->bottomAxesLayout;
    rhs->bottomAxesLayout = lhs->bottomAxesLayout;

    lhs->leftAxesLayout |= rhs->leftAxesLayout;
    rhs->leftAxesLayout = lhs->leftAxesLayout;

    lhs->rightAxesLayout |= rhs->rightAxesLayout;
    rhs->rightAxesLayout = lhs->rightAxesLayout;
}

static CoordinatePlaneList findSharingAxisDiagrams(AbstractCoordinatePlane *plane,
                                                   const CoordinatePlaneList &list,
                                                   Chart::Private::AxisType type,
                                                   QVector<CartesianAxis *> *sharedAxes)
{
    if (!plane || !plane->diagram())
        return CoordinatePlaneList();
    Q_ASSERT(plane);
    Q_ASSERT(plane->diagram());
    CoordinatePlaneList result;
    auto *diagram = qobject_cast<AbstractCartesianDiagram *>(plane->diagram());
    if (!diagram)
        return CoordinatePlaneList();

    QList<CartesianAxis *> axes;
    {
        const auto constAxes = diagram->axes();
        for (CartesianAxis *axis : constAxes) {
            if ((type == Chart::Private::Ordinate && (axis->position() == CartesianAxis::Left || axis->position() == CartesianAxis::Right))
                || (type == Chart::Private::Abscissa && (axis->position() == CartesianAxis::Top || axis->position() == CartesianAxis::Bottom))) {
                axes.append(axis);
            }
        }
    }

    for (AbstractCoordinatePlane *curPlane : list) {
        auto *diagram =
            qobject_cast<AbstractCartesianDiagram *>(curPlane->diagram());
        if (!diagram)
            continue;
        for (CartesianAxis *curSearchedAxis : axes) {
            const auto constAxes = diagram->axes();
            for (CartesianAxis *curAxis : constAxes) {
                if (curSearchedAxis == curAxis) {
                    result.append(curPlane);
                    if (!sharedAxes->contains(curSearchedAxis))
                        sharedAxes->append(curSearchedAxis);
                }
            }
        }
    }

    return result;
}

/**
 * this method determines the needed layout of the graph
 * taking care of the sharing problematic
 * its NOT allowed to have a diagram that shares
 * more than one axis in the same direction
 */
QVector<LayoutGraphNode *> Chart::Private::buildPlaneLayoutGraph()
{
    QHash<AbstractCoordinatePlane *, LayoutGraphNode *> planeNodeMapping;
    QVector<LayoutGraphNode *> allNodes;
    // create all nodes and a mapping between plane and nodes
    for (AbstractCoordinatePlane *curPlane : std::as_const(coordinatePlanes)) {
        if (curPlane->diagram()) {
            allNodes.append(new LayoutGraphNode);
            allNodes[allNodes.size() - 1]->diagramPlane = curPlane;
            allNodes[allNodes.size() - 1]->priority = allNodes.size();
            checkExistingAxes(allNodes[allNodes.size() - 1]);
            planeNodeMapping[curPlane] = allNodes[allNodes.size() - 1];
        }
    }
    // build the graph connections
    for (LayoutGraphNode *curNode : std::as_const(allNodes)) {
        QVector<CartesianAxis *> sharedAxes;
        CoordinatePlaneList xSharedPlanes = findSharingAxisDiagrams(curNode->diagramPlane, coordinatePlanes, Abscissa, &sharedAxes);
        Q_ASSERT(sharedAxes.size() < 2);
        // TODO duplicated code make a method out of it
        if (sharedAxes.size() == 1 && xSharedPlanes.size() > 1) {
            // xSharedPlanes.removeAll( sharedAxes.first()->diagram()->coordinatePlane() );
            // std::sort( xSharedPlanes.begin(), xSharedPlanes.end(), PriorityComparator( planeNodeMapping ) );
            for (int i = 0; i < xSharedPlanes.size() - 1; ++i) {
                LayoutGraphNode *tmpNode = planeNodeMapping[xSharedPlanes[i]];
                Q_ASSERT(tmpNode);
                LayoutGraphNode *tmpNode2 = planeNodeMapping[xSharedPlanes[i + 1]];
                Q_ASSERT(tmpNode2);
                tmpNode->bottomSuccesor = tmpNode2;
            }
            //            if ( sharedAxes.first()->diagram() &&  sharedAxes.first()->diagram()->coordinatePlane() )
            //            {
            //                LayoutGraphNode *lastNode = planeNodeMapping[ xSharedPlanes.last() ];
            //                Q_ASSERT( lastNode );
            //                Q_ASSERT( sharedAxes.first()->diagram()->coordinatePlane() );
            //                LayoutGraphNode *ownerNode = planeNodeMapping[ sharedAxes.first()->diagram()->coordinatePlane() ];
            //                Q_ASSERT( ownerNode );
            //                lastNode->bottomSuccesor = ownerNode;
            //            }
            // merge AxisInformation, needs a two pass run
            LayoutGraphNode axisInfoNode;
            for (int count = 0; count < 2; ++count) {
                for (int i = 0; i < xSharedPlanes.size(); ++i) {
                    mergeNodeAxisInformation(&axisInfoNode, planeNodeMapping[xSharedPlanes[i]]);
                }
            }
        }
        sharedAxes.clear();
        CoordinatePlaneList ySharedPlanes = findSharingAxisDiagrams(curNode->diagramPlane, coordinatePlanes, Ordinate, &sharedAxes);
        Q_ASSERT(sharedAxes.size() < 2);
        if (sharedAxes.size() == 1 && ySharedPlanes.size() > 1) {
            // ySharedPlanes.removeAll( sharedAxes.first()->diagram()->coordinatePlane() );
            // std::sort( ySharedPlanes.begin(), ySharedPlanes.end(), PriorityComparator( planeNodeMapping ) );
            for (int i = 0; i < ySharedPlanes.size() - 1; ++i) {
                LayoutGraphNode *tmpNode = planeNodeMapping[ySharedPlanes[i]];
                Q_ASSERT(tmpNode);
                LayoutGraphNode *tmpNode2 = planeNodeMapping[ySharedPlanes[i + 1]];
                Q_ASSERT(tmpNode2);
                tmpNode->leftSuccesor = tmpNode2;
            }
            //            if ( sharedAxes.first()->diagram() &&  sharedAxes.first()->diagram()->coordinatePlane() )
            //            {
            //                LayoutGraphNode *lastNode = planeNodeMapping[ ySharedPlanes.last() ];
            //                Q_ASSERT( lastNode );
            //                Q_ASSERT( sharedAxes.first()->diagram()->coordinatePlane() );
            //                LayoutGraphNode *ownerNode = planeNodeMapping[ sharedAxes.first()->diagram()->coordinatePlane() ];
            //                Q_ASSERT( ownerNode );
            //                lastNode->bottomSuccesor = ownerNode;
            //            }
            // merge AxisInformation, needs a two pass run
            LayoutGraphNode axisInfoNode;
            for (int count = 0; count < 2; ++count) {
                for (int i = 0; i < ySharedPlanes.size(); ++i) {
                    mergeNodeAxisInformation(&axisInfoNode, planeNodeMapping[ySharedPlanes[i]]);
                }
            }
        }
        sharedAxes.clear();
        if (curNode->diagramPlane->referenceCoordinatePlane())
            curNode->sharedSuccesor = planeNodeMapping[curNode->diagramPlane->referenceCoordinatePlane()];
    }

    return allNodes;
}

QHash<AbstractCoordinatePlane *, PlaneInfo> Chart::Private::buildPlaneLayoutInfos()
{
    /* There are two ways in which planes can be caused to interact in
     * where they are put layouting wise: The first is the reference plane. If
     * such a reference plane is set, on a plane, it will use the same cell in the
     * layout as that one. In addition to this, planes can share an axis. In that case
     * they will be laid out in relation to each other as suggested by the position
     * of the axis. If, for example Plane1 and Plane2 share an axis at position Left,
     * that will result in the layout: Axis Plane1 Plane 2, vertically. If Plane1
     * also happens to be Plane2's reference plane, both planes are drawn over each
     * other. The reference plane concept allows two planes to share the same space
     * even if neither has any axis, and in case there are shared axis, it is used
     * to decided, whether the planes should be painted on top of each other or
     * laid out vertically or horizontally next to each other. */
    QHash<CartesianAxis *, AxisInfo> axisInfos;
    QHash<AbstractCoordinatePlane *, PlaneInfo> planeInfos;
    for (AbstractCoordinatePlane *plane : std::as_const(coordinatePlanes)) {
        PlaneInfo p;
        // first check if we share space with another plane
        p.referencePlane = plane->referenceCoordinatePlane();
        planeInfos.insert(plane, p);

        const auto constDiagrams = plane->diagrams();
        for (AbstractDiagram *abstractDiagram : constDiagrams) {
            auto *diagram =
                qobject_cast<AbstractCartesianDiagram *>(abstractDiagram);
            if (!diagram) {
                continue;
            }

            const auto constAxes = diagram->axes();
            for (CartesianAxis *axis : constAxes) {
                if (!axisInfos.contains(axis)) {
                    /* If this is the first time we see this axis, add it, with the
                     * current plane. The first plane added to the chart that has
                     * the axis associated with it thus "owns" it, and decides about
                     * layout. */
                    AxisInfo i;
                    i.plane = plane;
                    axisInfos.insert(axis, i);
                } else {
                    AxisInfo i = axisInfos[axis];
                    if (i.plane == plane) {
                        continue; // we don't want duplicates, only shared
                    }

                    /* The user expects diagrams to be added on top, and to the right
                     * so that horizontally we need to move the new diagram, vertically
                     * the reference one. */
                    PlaneInfo pi = planeInfos[plane];
                    // plane-to-plane linking overrides linking via axes
                    if (!pi.referencePlane) {
                        // we're not the first plane to see this axis, mark us as a slave
                        pi.referencePlane = i.plane;
                        if (axis->position() == CartesianAxis::Left || axis->position() == CartesianAxis::Right) {
                            pi.horizontalOffset += 1;
                        }
                        planeInfos[plane] = pi;

                        pi = planeInfos[i.plane];
                        if (axis->position() == CartesianAxis::Top || axis->position() == CartesianAxis::Bottom) {
                            pi.verticalOffset += 1;
                        }

                        planeInfos[i.plane] = pi;
                    }
                }
            }
        }
        // Create a new grid layout for each plane that has no reference.
        p = planeInfos[plane];
        if (p.referencePlane == nullptr) {
            p.gridLayout = new QGridLayout();
            p.gridLayout->setContentsMargins(0, 0, 0, 0);
            planeInfos[plane] = p;
        }
    }
    return planeInfos;
}

void Chart::Private::slotLayoutPlanes()
{
    /*TODO make sure this is really needed */
    const QBoxLayout::Direction oldPlanesDirection = planesLayout ? planesLayout->direction()
                                                                  : QBoxLayout::TopToBottom;
    if (planesLayout && dataAndLegendLayout)
        dataAndLegendLayout->removeItem(planesLayout);

    const bool hadPlanesLayout = planesLayout != nullptr;
    int left, top, right, bottom;
    if (hadPlanesLayout)
        planesLayout->getContentsMargins(&left, &top, &right, &bottom);

    for (AbstractLayoutItem *plane : std::as_const(planeLayoutItems)) {
        plane->removeFromParentLayout();
    }
    // TODO they should get a correct parent, but for now it works
    for (AbstractLayoutItem *plane : std::as_const(planeLayoutItems)) {
        if (dynamic_cast<AutoSpacerLayoutItem *>(plane))
            delete plane;
    }

    planeLayoutItems.clear();
    delete planesLayout;
    // hint: The direction is configurable by the user now, as
    //       we are using a QBoxLayout rather than a QVBoxLayout.  (khz, 2007/04/25)
    planesLayout = new QBoxLayout(oldPlanesDirection);

    isPlanesLayoutDirty = true; // here we create the layouts; we need to "run" them before painting

    if (useNewLayoutSystem) {
        gridPlaneLayout = new QGridLayout;
        planesLayout->addLayout(gridPlaneLayout);

        if (hadPlanesLayout)
            planesLayout->setContentsMargins(left, top, right, bottom);
        planesLayout->setObjectName(QString::fromLatin1("planesLayout"));

        /* First go through all planes and all axes and figure out whether the planes
         * need to coordinate. If they do, they share a grid layout, if not, each
         * get their own. See buildPlaneLayoutInfos() for more details. */

        QVector<LayoutGraphNode *> vals = buildPlaneLayoutGraph();
        // qDebug() << Q_FUNC_INFO << "GraphNodes" << vals.size();
        QVector<LayoutGraphNode *> connectedComponents = getPrioritySortedConnectedComponents(vals);
        // qDebug() << Q_FUNC_INFO << "SubGraphs" << connectedComponents.size();
        int row = 0;
        int col = 0;
        QSet<CartesianAxis *> laidOutAxes;
        for (int i = 0; i < connectedComponents.size(); ++i) {
            LayoutGraphNode *curComponent = connectedComponents[i];
            for (LayoutGraphNode *curRowComponent = curComponent; curRowComponent; curRowComponent = curRowComponent->bottomSuccesor) {
                col = 0;
                for (LayoutGraphNode *curColComponent = curRowComponent; curColComponent; curColComponent = curColComponent->leftSuccesor) {
                    Q_ASSERT(curColComponent->diagramPlane->diagrams().size() == 1);
                    const auto constDiagrams = curColComponent->diagramPlane->diagrams();
                    for (AbstractDiagram *diagram : constDiagrams) {
                        const int planeRowOffset = 1; // curColComponent->topAxesLayout ? 1 : 0;
                        const int planeColOffset = 1; // curColComponent->leftAxesLayout ? 1 : 0;
                        // qDebug() << Q_FUNC_INFO << row << col << planeRowOffset << planeColOffset;

                        // qDebug() << Q_FUNC_INFO << row + planeRowOffset << col + planeColOffset;
                        planeLayoutItems << curColComponent->diagramPlane;
                        auto *cartDiag = qobject_cast<AbstractCartesianDiagram *>(diagram);
                        if (cartDiag) {
                            gridPlaneLayout->addItem(curColComponent->diagramPlane, row + planeRowOffset, col + planeColOffset, 2, 2);
                            curColComponent->diagramPlane->setParentLayout(gridPlaneLayout);
                            QHBoxLayout *leftLayout = nullptr;
                            QHBoxLayout *rightLayout = nullptr;
                            QVBoxLayout *topLayout = nullptr;
                            QVBoxLayout *bottomLayout = nullptr;
                            if (curComponent->sharedSuccesor) {
                                gridPlaneLayout->addItem(curColComponent->sharedSuccesor->diagramPlane, row + planeRowOffset, col + planeColOffset, 2, 2);
                                curColComponent->sharedSuccesor->diagramPlane->setParentLayout(gridPlaneLayout);
                                planeLayoutItems << curColComponent->sharedSuccesor->diagramPlane;
                            }
                            const auto constAxes = cartDiag->axes();
                            for (CartesianAxis *axis : constAxes) {
                                if (axis->isAbscissa()) {
                                    if (curColComponent->bottomSuccesor)
                                        continue;
                                }
                                if (laidOutAxes.contains(axis))
                                    continue;
                                //                        if ( axis->diagram() != diagram )
                                //                            continue;
                                switch (axis->position()) {
                                case (CartesianAxis::Top):
                                    if (!topLayout)
                                        topLayout = new QVBoxLayout;
                                    topLayout->addItem(axis);
                                    axis->setParentLayout(topLayout);
                                    break;
                                case (CartesianAxis::Bottom):
                                    if (!bottomLayout)
                                        bottomLayout = new QVBoxLayout;
                                    bottomLayout->addItem(axis);
                                    axis->setParentLayout(bottomLayout);
                                    break;
                                case (CartesianAxis::Left):
                                    if (!leftLayout)
                                        leftLayout = new QHBoxLayout;
                                    leftLayout->addItem(axis);
                                    axis->setParentLayout(leftLayout);
                                    break;
                                case (CartesianAxis::Right):
                                    if (!rightLayout) {
                                        rightLayout = new QHBoxLayout;
                                    }
                                    rightLayout->addItem(axis);
                                    axis->setParentLayout(rightLayout);
                                    break;
                                }
                                planeLayoutItems << axis;
                                laidOutAxes.insert(axis);
                            }
                            if (leftLayout)
                                gridPlaneLayout->addLayout(leftLayout, row + planeRowOffset, col, 2, 1,
                                                           Qt::AlignRight | Qt::AlignVCenter);
                            if (rightLayout)
                                gridPlaneLayout->addLayout(rightLayout, row, col + planeColOffset + 2, 2, 1,
                                                           Qt::AlignLeft | Qt::AlignVCenter);
                            if (topLayout)
                                gridPlaneLayout->addLayout(topLayout, row, col + planeColOffset, 1, 2,
                                                           Qt::AlignBottom | Qt::AlignHCenter);
                            if (bottomLayout)
                                gridPlaneLayout->addLayout(bottomLayout, row + planeRowOffset + 2,
                                                           col + planeColOffset, 1, 2, Qt::AlignTop | Qt::AlignHCenter);
                        } else {
                            gridPlaneLayout->addItem(curColComponent->diagramPlane, row, col, 4, 4);
                            curColComponent->diagramPlane->setParentLayout(gridPlaneLayout);
                        }
                        col += planeColOffset + 2 + (1);
                    }
                }
                int axisOffset = 2; // curRowComponent->topAxesLayout ? 1 : 0;
                // axisOffset += curRowComponent->bottomAxesLayout ? 1 : 0;
                const int rowOffset = axisOffset + 2;
                row += rowOffset;
            }

            //        if ( planesLayout->direction() == QBoxLayout::TopToBottom )
            //            ++row;
            //        else
            //            ++col;
        }

        qDeleteAll(vals);
        // re-add our grid(s) to the chart's layout
        if (dataAndLegendLayout) {
            dataAndLegendLayout->addLayout(planesLayout, 1, 1);
            dataAndLegendLayout->setRowStretch(1, 1000);
            dataAndLegendLayout->setColumnStretch(1, 1000);
        }
        slotResizePlanes();
#ifdef NEW_LAYOUT_DEBUG
        for (int i = 0; i < gridPlaneLayout->rowCount(); ++i) {
            for (int j = 0; j < gridPlaneLayout->columnCount(); ++j) {
                if (gridPlaneLayout->itemAtPosition(i, j))
                    qDebug() << Q_FUNC_INFO << "item at" << i << j << gridPlaneLayout->itemAtPosition(i, j)->geometry();
                else
                    qDebug() << Q_FUNC_INFO << "item at" << i << j << "no item present";
            }
        }
        // qDebug() << Q_FUNC_INFO << "Relayout ended";
#endif
    } else {
        if (hadPlanesLayout) {
            planesLayout->setContentsMargins(left, top, right, bottom);
        }

        planesLayout->setContentsMargins(0, 0, 0, 0);
        planesLayout->setSpacing(0);
        planesLayout->setObjectName(QString::fromLatin1("planesLayout"));

        /* First go through all planes and all axes and figure out whether the planes
         * need to coordinate. If they do, they share a grid layout, if not, each
         * gets their own. See buildPlaneLayoutInfos() for more details. */
        QHash<AbstractCoordinatePlane *, PlaneInfo> planeInfos = buildPlaneLayoutInfos();
        QHash<AbstractAxis *, AxisInfo> axisInfos;
        for (AbstractCoordinatePlane *plane : std::as_const(coordinatePlanes)) {
            Q_ASSERT(planeInfos.contains(plane));
            PlaneInfo &pi = planeInfos[plane];
            const int column = pi.horizontalOffset;
            const int row = pi.verticalOffset;
            // qDebug() << "processing plane at column" << column << "and row" << row;
            QGridLayout *planeLayout = pi.gridLayout;

            if (!planeLayout) {
                PlaneInfo &refPi = pi;
                // if this plane is sharing an axis with another one, recursively check for the original plane and use
                // the grid of that as planeLayout.
                while (!planeLayout && refPi.referencePlane) {
                    refPi = planeInfos[refPi.referencePlane];
                    planeLayout = refPi.gridLayout;
                }
                Q_ASSERT_X(planeLayout,
                           "Chart::Private::slotLayoutPlanes()",
                           "Invalid reference plane. Please check that the reference plane has been added to the Chart.");
            } else {
                planesLayout->addLayout(planeLayout);
            }

            /* Put the plane in the center of the layout. If this is our own, that's
             * the middle of the layout, if we are sharing, it's a cell in the center
             * column of the shared grid. */
            planeLayoutItems << plane;
            plane->setParentLayout(planeLayout);
            planeLayout->addItem(plane, row, column, 1, 1, {});
            // qDebug() << "Chart slotLayoutPlanes() calls planeLayout->addItem("<< row << column << ")";
            planeLayout->setRowStretch(row, 2);
            planeLayout->setColumnStretch(column, 2);
            const auto constDiagrams = plane->diagrams();
            for (AbstractDiagram *abstractDiagram : constDiagrams) {
                auto *diagram =
                    qobject_cast<AbstractCartesianDiagram *>(abstractDiagram);
                if (!diagram) {
                    continue; // FIXME what about polar ?
                }

                if (pi.referencePlane != nullptr) {
                    pi.topAxesLayout = planeInfos[pi.referencePlane].topAxesLayout;
                    pi.bottomAxesLayout = planeInfos[pi.referencePlane].bottomAxesLayout;
                    pi.leftAxesLayout = planeInfos[pi.referencePlane].leftAxesLayout;
                    pi.rightAxesLayout = planeInfos[pi.referencePlane].rightAxesLayout;
                }

                // collect all axes of a kind into sublayouts
                if (pi.topAxesLayout == nullptr) {
                    pi.topAxesLayout = new QVBoxLayout;
                    pi.topAxesLayout->setContentsMargins(0, 0, 0, 0);
                    pi.topAxesLayout->setObjectName(QString::fromLatin1("topAxesLayout"));
                }
                if (pi.bottomAxesLayout == nullptr) {
                    pi.bottomAxesLayout = new QVBoxLayout;
                    pi.bottomAxesLayout->setContentsMargins(0, 0, 0, 0);
                    pi.bottomAxesLayout->setObjectName(QString::fromLatin1("bottomAxesLayout"));
                }
                if (pi.leftAxesLayout == nullptr) {
                    pi.leftAxesLayout = new QHBoxLayout;
                    pi.leftAxesLayout->setContentsMargins(0, 0, 0, 0);
                    pi.leftAxesLayout->setObjectName(QString::fromLatin1("leftAxesLayout"));
                }
                if (pi.rightAxesLayout == nullptr) {
                    pi.rightAxesLayout = new QHBoxLayout;
                    pi.rightAxesLayout->setContentsMargins(0, 0, 0, 0);
                    pi.rightAxesLayout->setObjectName(QString::fromLatin1("rightAxesLayout"));
                }

                if (pi.referencePlane != nullptr) {
                    planeInfos[pi.referencePlane].topAxesLayout = pi.topAxesLayout;
                    planeInfos[pi.referencePlane].bottomAxesLayout = pi.bottomAxesLayout;
                    planeInfos[pi.referencePlane].leftAxesLayout = pi.leftAxesLayout;
                    planeInfos[pi.referencePlane].rightAxesLayout = pi.rightAxesLayout;
                }

                // pi.leftAxesLayout->setSizeConstraint( QLayout::SetFixedSize );
                const auto constAxes = diagram->axes();
                for (CartesianAxis *axis : constAxes) {
                    if (axisInfos.contains(axis)) {
                        continue; // already laid out this one
                    }
                    Q_ASSERT(axis);
                    axis->setCachedSizeDirty();
                    // qDebug() << "--------------- axis added to planeLayoutItems  -----------------";
                    planeLayoutItems << axis;

                    switch (axis->position()) {
                    case CartesianAxis::Top:
                        axis->setParentLayout(pi.topAxesLayout);
                        pi.topAxesLayout->addItem(axis);
                        break;
                    case CartesianAxis::Bottom:
                        axis->setParentLayout(pi.bottomAxesLayout);
                        pi.bottomAxesLayout->addItem(axis);
                        break;
                    case CartesianAxis::Left:
                        axis->setParentLayout(pi.leftAxesLayout);
                        pi.leftAxesLayout->addItem(axis);
                        break;
                    case CartesianAxis::Right:
                        axis->setParentLayout(pi.rightAxesLayout);
                        pi.rightAxesLayout->addItem(axis);
                        break;
                    default:
                        Q_ASSERT_X(false, "Chart::paintEvent", "unknown axis position");
                        break;
                    };
                    axisInfos.insert(axis, AxisInfo());
                }
                /* Put each stack of axes-layouts in the cells surrounding the
                 * associated plane. We are laying out in the order the planes
                 * were added, and the first one gets to lay out shared axes.
                 * Private axes go here as well, of course. */

                if (!pi.topAxesLayout->parent()) {
                    planeLayout->addLayout(pi.topAxesLayout, row - 1, column);
                }
                if (!pi.bottomAxesLayout->parent()) {
                    planeLayout->addLayout(pi.bottomAxesLayout, row + 1, column);
                }
                if (!pi.leftAxesLayout->parent()) {
                    planeLayout->addLayout(pi.leftAxesLayout, row, column - 1);
                }
                if (!pi.rightAxesLayout->parent()) {
                    planeLayout->addLayout(pi.rightAxesLayout, row, column + 1);
                }
            }

            // use up to four auto-spacer items in the corners around the diagrams:
#define ADD_AUTO_SPACER_IF_NEEDED(                                                                                      \
    spacerRow, spacerColumn, hLayoutIsAtTop, hLayout, vLayoutIsAtLeft, vLayout)                                         \
    {                                                                                                                   \
        if (hLayout || vLayout) {                                                                                       \
            AutoSpacerLayoutItem *spacer = new AutoSpacerLayoutItem(hLayoutIsAtTop, hLayout, vLayoutIsAtLeft, vLayout); \
            planeLayout->addItem(spacer, spacerRow, spacerColumn, 1, 1);                                                \
            spacer->setParentLayout(planeLayout);                                                                       \
            planeLayoutItems << spacer;                                                                                 \
        }                                                                                                               \
    }

            if (plane->isCornerSpacersEnabled()) {
                ADD_AUTO_SPACER_IF_NEEDED(row - 1, column - 1, false, pi.leftAxesLayout, false, pi.topAxesLayout)
                ADD_AUTO_SPACER_IF_NEEDED(row + 1, column - 1, true, pi.leftAxesLayout, false, pi.bottomAxesLayout)
                ADD_AUTO_SPACER_IF_NEEDED(row - 1, column + 1, false, pi.rightAxesLayout, true, pi.topAxesLayout)
                ADD_AUTO_SPACER_IF_NEEDED(row + 1, column + 1, true, pi.rightAxesLayout, true, pi.bottomAxesLayout)
            }
        }
        // re-add our grid(s) to the chart's layout
        if (dataAndLegendLayout) {
            dataAndLegendLayout->addLayout(planesLayout, 1, 1);
            dataAndLegendLayout->setRowStretch(1, 1000);
            dataAndLegendLayout->setColumnStretch(1, 1000);
        }

        slotResizePlanes();
    }
}

void Chart::Private::createLayouts()
{
    // The toplevel layout provides the left and right global margins
    layout = new QHBoxLayout(chart);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setObjectName(QString::fromLatin1("Chart::Private::layout"));
    layout->addSpacing(globalLeadingLeft);
    leftOuterSpacer = layout->itemAt(layout->count() - 1)->spacerItem();

    // The vLayout provides top and bottom global margins and lays
    // out headers, footers and the diagram area.
    vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setObjectName(QString::fromLatin1("vLayout"));

    layout->addLayout(vLayout, 1000);
    layout->addSpacing(globalLeadingRight);
    rightOuterSpacer = layout->itemAt(layout->count() - 1)->spacerItem();

    // 1. the spacing above the header area
    vLayout->addSpacing(globalLeadingTop);
    topOuterSpacer = vLayout->itemAt(vLayout->count() - 1)->spacerItem();
    // 2. the header area
    headerLayout = new QGridLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addLayout(headerLayout);
    // 3. the area containing coordinate planes, axes, and legends
    dataAndLegendLayout = new QGridLayout();
    dataAndLegendLayout->setContentsMargins(0, 0, 0, 0);
    dataAndLegendLayout->setObjectName(QString::fromLatin1("dataAndLegendLayout"));
    vLayout->addLayout(dataAndLegendLayout, 1000);
    // 4. the footer area
    footerLayout = new QGridLayout();
    footerLayout->setContentsMargins(0, 0, 0, 0);
    footerLayout->setObjectName(QString::fromLatin1("footerLayout"));
    vLayout->addLayout(footerLayout);

    // 5. Prepare the header / footer layout cells:
    //    Each of the 9 header cells (the 9 footer cells)
    //    contain their own QVBoxLayout
    //    since there can be more than one header (footer) per cell.
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            const Qt::Alignment align = s_gridAlignments[row][column];
            for (int headOrFoot = 0; headOrFoot < 2; headOrFoot++) {
                auto *innerLayout = new QVBoxLayout();
                innerLayout->setContentsMargins(0, 0, 0, 0);
                innerLayout->setAlignment(align);
                innerHdFtLayouts[headOrFoot][row][column] = innerLayout;

                QGridLayout *outerLayout = headOrFoot == 0 ? headerLayout : footerLayout;
                outerLayout->addLayout(innerLayout, row, column, align);
            }
        }
    }

    // 6. the spacing below the footer area
    vLayout->addSpacing(globalLeadingBottom);
    bottomOuterSpacer = vLayout->itemAt(vLayout->count() - 1)->spacerItem();

    // the data+axes area
    dataAndLegendLayout->addLayout(planesLayout, 1, 1);
    dataAndLegendLayout->setRowStretch(1, 1);
    dataAndLegendLayout->setColumnStretch(1, 1);
}

void Chart::Private::slotResizePlanes()
{
    if (!dataAndLegendLayout) {
        return;
    }
    if (!overrideSize.isValid()) {
        // activate() takes the size from the layout's parent QWidget, which is not updated when overrideSize
        // is set. So don't let the layout grab the wrong size in that case.
        // When overrideSize *is* set, we call layout->setGeometry() in paint( QPainter*, const QRect& ),
        // which also "activates" the layout in the sense that it distributes space internally.
        layout->activate();
    }
    // Adapt diagram drawing to the new size
    for (AbstractCoordinatePlane *plane : std::as_const(coordinatePlanes)) {
        plane->layoutDiagrams();
    }
}

void Chart::Private::updateDirtyLayouts()
{
    if (isPlanesLayoutDirty) {
        for (AbstractCoordinatePlane *p : std::as_const(coordinatePlanes)) {
            p->setGridNeedsRecalculate();
            p->layoutPlanes();
            p->layoutDiagrams();
        }
    }
    if (isPlanesLayoutDirty || isFloatingLegendsLayoutDirty) {
        chart->reLayoutFloatingLegends();
    }
    isPlanesLayoutDirty = false;
    isFloatingLegendsLayoutDirty = false;
}

void Chart::Private::reapplyInternalLayouts()
{
    QRect geo = layout->geometry();

    invalidateLayoutTree(layout);
    layout->setGeometry(geo);
    slotResizePlanes();
}

void Chart::Private::paintAll(QPainter *painter)
{
    updateDirtyLayouts();

    QRect rect(QPoint(0, 0), overrideSize.isValid() ? overrideSize : chart->size());

    // qDebug() << this<<"::paintAll() uses layout size" << currentLayoutSize;

    // Paint the background (if any)
    AbstractAreaBase::paintBackgroundAttributes(*painter, rect, backgroundAttributes);
    // Paint the frame (if any)
    AbstractAreaBase::paintFrameAttributes(*painter, rect, frameAttributes);

    chart->reLayoutFloatingLegends();

    for (AbstractLayoutItem *planeLayoutItem : std::as_const(planeLayoutItems)) {
        planeLayoutItem->paintAll(*painter);
    }
    for (TextArea *textLayoutItem : std::as_const(textLayoutItems)) {
        textLayoutItem->paintAll(*painter);
    }
    for (Legend *legend : std::as_const(legends)) {
        const bool hidden = legend->isHidden() && legend->testAttribute(Qt::WA_WState_ExplicitShowHide);
        if (!hidden) {
            // qDebug() << "painting legend at " << legend->geometry();
            legend->paintIntoRect(*painter, legend->geometry());
        }
    }
}

// ******** Chart interface implementation ***********

#define d d_func()

Chart::Chart(QWidget *parent)
    : QWidget(parent)
    , _d(new Private(this))
{
#if defined KDAB_EVAL
    EvalDialog::checkEvalLicense("KD Chart");
#endif

    FrameAttributes frameAttrs;
    // no frame per default...
    //    frameAttrs.setVisible( true );
    frameAttrs.setPen(QPen(Qt::black));
    frameAttrs.setPadding(1);
    setFrameAttributes(frameAttrs);

    addCoordinatePlane(new CartesianCoordinatePlane(this));

    d->createLayouts();
}

Chart::~Chart()
{
    delete d;
}

void Chart::setFrameAttributes(const FrameAttributes &a)
{
    d->frameAttributes = a;
}

FrameAttributes Chart::frameAttributes() const
{
    return d->frameAttributes;
}

void Chart::setBackgroundAttributes(const BackgroundAttributes &a)
{
    d->backgroundAttributes = a;
}

BackgroundAttributes Chart::backgroundAttributes() const
{
    return d->backgroundAttributes;
}

// TODO KDChart 3.0; change QLayout into QBoxLayout::Direction
void Chart::setCoordinatePlaneLayout(QLayout *layout)
{
    if (layout == d->planesLayout)
        return;
    if (d->planesLayout) {
        // detach all QLayoutItem's the previous planesLayout has cause
        // otherwise deleting the planesLayout would delete them too.
        for (int i = d->planesLayout->count() - 1; i >= 0; --i) {
            d->planesLayout->takeAt(i);
        }
        delete d->planesLayout;
    }
    d->planesLayout = qobject_cast<QBoxLayout *>(layout);
    d->slotLayoutPlanes();
}

QLayout *Chart::coordinatePlaneLayout()
{
    return d->planesLayout;
}

AbstractCoordinatePlane *Chart::coordinatePlane()
{
    if (d->coordinatePlanes.isEmpty()) {
        qWarning() << "Chart::coordinatePlane: warning: no coordinate plane defined.";
        return nullptr;
    } else {
        return d->coordinatePlanes.first();
    }
}

CoordinatePlaneList Chart::coordinatePlanes()
{
    return d->coordinatePlanes;
}

void Chart::addCoordinatePlane(AbstractCoordinatePlane *plane)
{
    // Append
    insertCoordinatePlane(d->coordinatePlanes.count(), plane);
}

void Chart::insertCoordinatePlane(int index, AbstractCoordinatePlane *plane)
{
    if (index < 0 || index > d->coordinatePlanes.count()) {
        return;
    }

    connect(plane, &AbstractCoordinatePlane::destroyedCoordinatePlane,
            d, &Private::slotUnregisterDestroyedPlane);
    connect(plane, &AbstractCoordinatePlane::needUpdate, this, QOverload<>::of(&Chart::update));
    connect(plane, &AbstractCoordinatePlane::needRelayout, d, &Private::slotResizePlanes);
    connect(plane, &AbstractCoordinatePlane::needLayoutPlanes, d, &Private::slotLayoutPlanes);
    connect(plane, &AbstractCoordinatePlane::propertiesChanged, this, &Chart::propertiesChanged);
    d->coordinatePlanes.insert(index, plane);
    plane->setParent(this);
    d->slotLayoutPlanes();
}

void Chart::replaceCoordinatePlane(AbstractCoordinatePlane *plane,
                                   AbstractCoordinatePlane *oldPlane_)
{
    if (plane && oldPlane_ != plane) {
        AbstractCoordinatePlane *oldPlane = oldPlane_;
        if (d->coordinatePlanes.count()) {
            if (!oldPlane) {
                oldPlane = d->coordinatePlanes.first();
                if (oldPlane == plane)
                    return;
            }
            takeCoordinatePlane(oldPlane);
        }
        delete oldPlane;
        addCoordinatePlane(plane);
    }
}

void Chart::takeCoordinatePlane(AbstractCoordinatePlane *plane)
{
    const int idx = d->coordinatePlanes.indexOf(plane);
    if (idx != -1) {
        d->coordinatePlanes.takeAt(idx);
        disconnect(plane, nullptr, d, nullptr);
        disconnect(plane, nullptr, this, nullptr);
        plane->removeFromParentLayout();
        plane->setParent(nullptr);
        d->mouseClickedPlanes.removeAll(plane);
    }
    d->slotLayoutPlanes();
    // Need to Q_EMIT the signal: In case somebody has connected the signal
    // to her own slot for e.g. calling update() on a widget containing the chart.
    Q_EMIT propertiesChanged();
}

void Chart::setGlobalLeading(int left, int top, int right, int bottom)
{
    setGlobalLeadingLeft(left);
    setGlobalLeadingTop(top);
    setGlobalLeadingRight(right);
    setGlobalLeadingBottom(bottom);
}

void Chart::setGlobalLeadingLeft(int leading)
{
    d->globalLeadingLeft = leading;
    d->leftOuterSpacer->changeSize(leading, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    d->reapplyInternalLayouts();
    Q_EMIT globalLeadingChanged();
}

int Chart::globalLeadingLeft() const
{
    return d->globalLeadingLeft;
}

void Chart::setGlobalLeadingTop(int leading)
{
    d->globalLeadingTop = leading;
    d->topOuterSpacer->changeSize(0, leading, QSizePolicy::Minimum, QSizePolicy::Fixed);
    d->reapplyInternalLayouts();
    Q_EMIT globalLeadingChanged();
}

int Chart::globalLeadingTop() const
{
    return d->globalLeadingTop;
}

void Chart::setGlobalLeadingRight(int leading)
{
    d->globalLeadingRight = leading;
    d->rightOuterSpacer->changeSize(leading, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);
    d->reapplyInternalLayouts();
    Q_EMIT globalLeadingChanged();
}

int Chart::globalLeadingRight() const
{
    return d->globalLeadingRight;
}

void Chart::setGlobalLeadingBottom(int leading)
{
    d->globalLeadingBottom = leading;
    d->bottomOuterSpacer->changeSize(0, leading, QSizePolicy::Minimum, QSizePolicy::Fixed);
    d->reapplyInternalLayouts();
    Q_EMIT globalLeadingChanged();
}

int Chart::globalLeadingBottom() const
{
    return d->globalLeadingBottom;
}

void Chart::paint(QPainter *painter, const QRect &target)
{
    if (target.isEmpty() || !painter) {
        return;
    }

    QPaintDevice *prevDevice = GlobalMeasureScaling::paintDevice();
    GlobalMeasureScaling::setPaintDevice(painter->device());

    // Output on a widget
    if (dynamic_cast<QWidget *>(painter->device()) != nullptr) {
        GlobalMeasureScaling::setFactors(qreal(target.width()) / qreal(geometry().size().width()),
                                         qreal(target.height()) / qreal(geometry().size().height()));
    } else {
        // Output onto a QPixmap
        PrintingParameters::setScaleFactor(qreal(painter->device()->logicalDpiX()) / qreal(logicalDpiX()));

        const qreal resX = qreal(logicalDpiX()) / qreal(painter->device()->logicalDpiX());
        const qreal resY = qreal(logicalDpiY()) / qreal(painter->device()->logicalDpiY());

        GlobalMeasureScaling::setFactors(qreal(target.width()) / qreal(geometry().size().width()) * resX,
                                         qreal(target.height()) / qreal(geometry().size().height()) * resY);
    }

    const QPoint translation = target.topLeft();
    painter->translate(translation);

    // the following layout logic has the disadvantage that repeatedly calling this method can
    // cause a relayout every time, but since this method's main use seems to be printing, the
    // gratuitous relayouts shouldn't be much of a performance problem.
    const bool differentSize = target.size() != size();
    QRect oldGeometry;
    if (differentSize) {
        oldGeometry = geometry();
        d->isPlanesLayoutDirty = true;
        d->isFloatingLegendsLayoutDirty = true;
        invalidateLayoutTree(d->dataAndLegendLayout);
        d->dataAndLegendLayout->setGeometry(QRect(QPoint(), target.size()));
    }

    d->overrideSize = target.size();
    d->paintAll(painter);
    d->overrideSize = QSize();

    if (differentSize) {
        invalidateLayoutTree(d->dataAndLegendLayout);
        d->dataAndLegendLayout->setGeometry(oldGeometry);
        d->isPlanesLayoutDirty = true;
        d->isFloatingLegendsLayoutDirty = true;
    }

    // for debugging
    // painter->setPen( QPen( Qt::blue, 8 ) );
    // painter->drawRect( target );

    painter->translate(-translation.x(), -translation.y());

    GlobalMeasureScaling::instance()->resetFactors();
    PrintingParameters::resetScaleFactor();
    GlobalMeasureScaling::setPaintDevice(prevDevice);
}

void Chart::resizeEvent(QResizeEvent *event)
{
    d->isPlanesLayoutDirty = true;
    d->isFloatingLegendsLayoutDirty = true;
    QWidget::resizeEvent(event);
}

void Chart::reLayoutFloatingLegends()
{
    for (Legend *legend : std::as_const(d->legends)) {
        const bool hidden = legend->isHidden() && legend->testAttribute(Qt::WA_WState_ExplicitShowHide);
        if (legend->position().isFloating() && !hidden) {
            // resize the legend
            const QSize legendSize(legend->sizeHint());
            legend->setGeometry(QRect(legend->geometry().topLeft(), legendSize));
            // find the legends corner point (reference point plus any paddings)
            const RelativePosition relPos(legend->floatingPosition());
            QPointF pt(relPos.calculatedPoint(size()));
            // qDebug() << pt;
            //  calculate the legend's top left point
            const Qt::Alignment alignTopLeft = Qt::AlignTop | Qt::AlignLeft;
            if ((relPos.alignment() & alignTopLeft) != alignTopLeft) {
                if (relPos.alignment() & Qt::AlignRight)
                    pt.rx() -= legendSize.width();
                else if (relPos.alignment() & Qt::AlignHCenter)
                    pt.rx() -= 0.5 * legendSize.width();

                if (relPos.alignment() & Qt::AlignBottom)
                    pt.ry() -= legendSize.height();
                else if (relPos.alignment() & Qt::AlignVCenter)
                    pt.ry() -= 0.5 * legendSize.height();
            }
            // qDebug() << pt << endl;
            legend->move(static_cast<int>(pt.x()), static_cast<int>(pt.y()));
        }
    }
}

void Chart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    d->paintAll(&painter);
    Q_EMIT finishedDrawing();
}

void Chart::addHeaderFooter(HeaderFooter *hf)
{
    Q_ASSERT(hf->type() == HeaderFooter::Header || hf->type() == HeaderFooter::Footer);
    int row;
    int column;
    getRowAndColumnForPosition(hf->position().value(), &row, &column);
    if (row == -1) {
        qWarning("Unknown header/footer position");
        return;
    }

    d->headerFooters.append(hf);
    d->textLayoutItems.append(hf);
    connect(hf, &HeaderFooter::destroyedHeaderFooter,
            d, &Private::slotUnregisterDestroyedHeaderFooter);
    connect(hf, &HeaderFooter::positionChanged,
            d, &Private::slotHeaderFooterPositionChanged);

    // set the text attributes (why?)

    TextAttributes textAttrs(hf->textAttributes());
    Measure measure(textAttrs.fontSize());
    measure.setRelativeMode(this, KDChartEnums::MeasureOrientationMinimum);
    measure.setValue(20);
    textAttrs.setFontSize(measure);
    hf->setTextAttributes(textAttrs);

    // add it to the appropriate layout

    int innerLayoutIdx = hf->type() == HeaderFooter::Header ? 0 : 1;
    QVBoxLayout *headerFooterLayout = d->innerHdFtLayouts[innerLayoutIdx][row][column];

    hf->setParentLayout(headerFooterLayout);
    hf->setAlignment(s_gridAlignments[row][column]);
    headerFooterLayout->addItem(hf);

    d->slotResizePlanes();
}

void Chart::replaceHeaderFooter(HeaderFooter *headerFooter,
                                HeaderFooter *oldHeaderFooter_)
{
    if (headerFooter && oldHeaderFooter_ != headerFooter) {
        HeaderFooter *oldHeaderFooter = oldHeaderFooter_;
        if (d->headerFooters.count()) {
            if (!oldHeaderFooter) {
                oldHeaderFooter = d->headerFooters.first();
                if (oldHeaderFooter == headerFooter)
                    return;
            }
            takeHeaderFooter(oldHeaderFooter);
        }
        delete oldHeaderFooter;
        addHeaderFooter(headerFooter);
    }
}

void Chart::takeHeaderFooter(HeaderFooter *headerFooter)
{
    const int idx = d->headerFooters.indexOf(headerFooter);
    if (idx == -1) {
        return;
    }
    disconnect(headerFooter, &HeaderFooter::destroyedHeaderFooter,
               d, &Private::slotUnregisterDestroyedHeaderFooter);

    d->headerFooters.takeAt(idx);
    headerFooter->removeFromParentLayout();
    headerFooter->setParentLayout(nullptr);
    d->textLayoutItems.remove(d->textLayoutItems.indexOf(headerFooter));

    d->slotResizePlanes();
}

void Chart::Private::slotHeaderFooterPositionChanged(HeaderFooter *hf)
{
    chart->takeHeaderFooter(hf);
    chart->addHeaderFooter(hf);
}

HeaderFooter *Chart::headerFooter()
{
    if (d->headerFooters.isEmpty()) {
        return nullptr;
    } else {
        return d->headerFooters.first();
    }
}

HeaderFooterList Chart::headerFooters()
{
    return d->headerFooters;
}

void Chart::Private::slotLegendPositionChanged(AbstractAreaWidget *aw)
{
    auto *legend = qobject_cast<Legend *>(aw);
    Q_ASSERT(legend);
    chart->takeLegend(legend);
    chart->addLegendInternal(legend, false);
}

void Chart::addLegend(Legend *legend)
{
    legend->show();
    addLegendInternal(legend, true);
    Q_EMIT propertiesChanged();
}

void Chart::addLegendInternal(Legend *legend, bool setMeasures)
{
    if (!legend) {
        return;
    }

    KDChartEnums::PositionValue pos = legend->position().value();
    if (pos == KDChartEnums::PositionCenter) {
        qWarning("Not showing legend because PositionCenter is not supported for legends.");
    }

    int row;
    int column;
    getRowAndColumnForPosition(pos, &row, &column);
    if (row < 0 && pos != KDChartEnums::PositionFloating) {
        qWarning("Not showing legend because of unknown legend position.");
        return;
    }

    d->legends.append(legend);
    legend->setParent(this);

    // set text attributes (why?)

    if (setMeasures) {
        TextAttributes textAttrs(legend->textAttributes());
        Measure measure(textAttrs.fontSize());
        measure.setRelativeMode(this, KDChartEnums::MeasureOrientationMinimum);
        measure.setValue(20);
        textAttrs.setFontSize(measure);
        legend->setTextAttributes(textAttrs);

        textAttrs = legend->titleTextAttributes();
        measure.setRelativeMode(this, KDChartEnums::MeasureOrientationMinimum);
        measure.setValue(24);
        textAttrs.setFontSize(measure);

        legend->setTitleTextAttributes(textAttrs);
        legend->setReferenceArea(this);
    }

    // add it to the appropriate layout

    if (pos != KDChartEnums::PositionFloating) {
        legend->needSizeHint();

        // in each edge and corner of the outer layout, there's a grid for the different alignments that we create
        // on demand. we don't remove it when empty.

        QLayoutItem *edgeItem = d->dataAndLegendLayout->itemAtPosition(row, column);
        auto *alignmentsLayout = dynamic_cast<QGridLayout *>(edgeItem);
        Q_ASSERT(!edgeItem || alignmentsLayout); // if it exists, it must be a QGridLayout
        if (!alignmentsLayout) {
            alignmentsLayout = new QGridLayout;
            d->dataAndLegendLayout->addLayout(alignmentsLayout, row, column);
            alignmentsLayout->setContentsMargins(0, 0, 0, 0);
        }

        // in case there are several legends in the same edge or corner with the same alignment, they are stacked
        // vertically using a QVBoxLayout. it is created on demand as above.

        row = 1;
        column = 1;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                Qt::Alignment align = s_gridAlignments[i][j];
                if (align == legend->alignment()) {
                    row = i;
                    column = j;
                    break;
                }
            }
        }

        QLayoutItem *alignmentItem = alignmentsLayout->itemAtPosition(row, column);
        auto *sameAlignmentLayout = dynamic_cast<QVBoxLayout *>(alignmentItem);
        Q_ASSERT(!alignmentItem || sameAlignmentLayout); // if it exists, it must be a QVBoxLayout
        if (!sameAlignmentLayout) {
            sameAlignmentLayout = new QVBoxLayout;
            alignmentsLayout->addLayout(sameAlignmentLayout, row, column);
            sameAlignmentLayout->setContentsMargins(0, 0, 0, 0);
        }

        sameAlignmentLayout->addItem(new MyWidgetItem(legend, legend->alignment()));
    }

    connect(legend, &Legend::destroyedLegend,
            d, &Private::slotUnregisterDestroyedLegend);
    connect(legend, &Legend::positionChanged,
            d, &Private::slotLegendPositionChanged);
    connect(legend, &Legend::propertiesChanged, this, &Chart::propertiesChanged);

    d->slotResizePlanes();
}

void Chart::replaceLegend(Legend *legend, Legend *oldLegend_)
{
    if (legend && oldLegend_ != legend) {
        Legend *oldLegend = oldLegend_;
        if (d->legends.count()) {
            if (!oldLegend) {
                oldLegend = d->legends.first();
                if (oldLegend == legend)
                    return;
            }
            takeLegend(oldLegend);
        }
        delete oldLegend;
        addLegend(legend);
    }
}

void Chart::takeLegend(Legend *legend)
{
    const int idx = d->legends.indexOf(legend);
    if (idx == -1) {
        return;
    }

    d->legends.takeAt(idx);
    disconnect(legend, nullptr, d, nullptr);
    disconnect(legend, nullptr, this, nullptr);
    // the following removes the legend from its layout and destroys its MyWidgetItem (the link to the layout)
    legend->setParent(nullptr);

    d->slotResizePlanes();
    Q_EMIT propertiesChanged();
}

Legend *Chart::legend()
{
    return d->legends.isEmpty() ? 0 : d->legends.first();
}

LegendList Chart::legends() const
{
    return d->legends;
}

void Chart::mousePressEvent(QMouseEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint globalPos = event->globalPosition().toPoint();
#else
    const QPoint globalPos = event->globalPos();
#endif

    const QPoint pos = mapFromGlobal(globalPos);

    for (AbstractCoordinatePlane *plane : std::as_const(d->coordinatePlanes)) {
        if (plane->geometry().contains(event->pos()) && plane->diagrams().size() > 0) {
            QMouseEvent ev(QEvent::MouseButtonPress, pos, globalPos,
                           event->button(), event->buttons(), event->modifiers());
            plane->mousePressEvent(&ev);
            d->mouseClickedPlanes.append(plane);
        }
    }
}

void Chart::mouseDoubleClickEvent(QMouseEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint globalPos = event->globalPosition().toPoint();
#else
    const QPoint globalPos = event->globalPos();
#endif

    const QPoint pos = mapFromGlobal(globalPos);

    for (AbstractCoordinatePlane *plane : std::as_const(d->coordinatePlanes)) {
        if (plane->geometry().contains(event->pos()) && plane->diagrams().size() > 0) {
            QMouseEvent ev(QEvent::MouseButtonPress, pos, globalPos,
                           event->button(), event->buttons(), event->modifiers());
            plane->mouseDoubleClickEvent(&ev);
        }
    }
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
    auto eventReceivers =
#if QT_VERSION > QT_VERSION_CHECK(5, 14, 0)
        QSet<AbstractCoordinatePlane *>(d->mouseClickedPlanes.begin(), d->mouseClickedPlanes.end());
#else
        QSet<AbstractCoordinatePlane *>::fromList(d->mouseClickedPlanes);
#endif

    for (AbstractCoordinatePlane *plane : std::as_const(d->coordinatePlanes)) {
        if (plane->geometry().contains(event->pos()) && plane->diagrams().size() > 0) {
            eventReceivers.insert(plane);
        }
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint globalPos = event->globalPosition().toPoint();
#else
    const QPoint globalPos = event->globalPos();
#endif

    const QPoint pos = mapFromGlobal(globalPos);

    for (AbstractCoordinatePlane *plane : std::as_const(eventReceivers)) {
        QMouseEvent ev(QEvent::MouseMove, pos, globalPos,
                       event->button(), event->buttons(), event->modifiers());
        plane->mouseMoveEvent(&ev);
    }
}

void Chart::mouseReleaseEvent(QMouseEvent *event)
{
    auto eventReceivers =
#if QT_VERSION > QT_VERSION_CHECK(5, 14, 0)
        QSet<AbstractCoordinatePlane *>(d->mouseClickedPlanes.begin(), d->mouseClickedPlanes.end());
#else
        QSet<AbstractCoordinatePlane *>::fromList(d->mouseClickedPlanes);
#endif

    for (AbstractCoordinatePlane *plane : std::as_const(d->coordinatePlanes)) {
        if (plane->geometry().contains(event->pos()) && plane->diagrams().size() > 0) {
            eventReceivers.insert(plane);
        }
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPoint globalPos = event->globalPosition().toPoint();
#else
    const QPoint globalPos = event->globalPos();
#endif

    const QPoint pos = mapFromGlobal(globalPos);

    for (AbstractCoordinatePlane *plane : std::as_const(eventReceivers)) {
        QMouseEvent ev(QEvent::MouseButtonRelease, pos, globalPos,
                       event->button(), event->buttons(), event->modifiers());
        plane->mouseReleaseEvent(&ev);
    }

    d->mouseClickedPlanes.clear();
}

bool Chart::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        const QHelpEvent *const helpEvent = static_cast<QHelpEvent *>(event);
        for (int stage = 0; stage < 2; ++stage) {
            for (const AbstractCoordinatePlane *const plane : std::as_const(d->coordinatePlanes)) {
                const auto constDiagrams = plane->diagrams();
                for (const AbstractDiagram *diagram : constDiagrams) {

                    QModelIndex index;
                    if (stage == 0) {
                        // First search at the exact position
                        index = diagram->indexAt(helpEvent->pos());
                    } else {
                        // Second, search in a larger area, which is easier to hit on screens with higher DPI.
                        const QModelIndexList indexes = diagram->indexesIn(QRect(helpEvent->pos() - QPoint(15, 15), QSize(30, 30)));
                        index = indexes.isEmpty() ? QModelIndex() : indexes.front();
                    }

                    const QVariant toolTip = index.data(Qt::ToolTipRole);
                    if (toolTip.isValid()) {
                        const QPoint pos = mapFromGlobal(helpEvent->pos());
                        const QRect rect(pos - QPoint(1, 1), QSize(3, 3));
                        QToolTip::showText(QCursor::pos(), toolTip.toString(), this, rect);
                        return true;
                    }
                }
            }
        }
    }
    return QWidget::event(event);
}

bool Chart::useNewLayoutSystem() const
{
    return d_func()->useNewLayoutSystem;
}
void Chart::setUseNewLayoutSystem(bool value)
{
    if (d_func()->useNewLayoutSystem != value) {
        d_func()->useNewLayoutSystem = value;
        Q_EMIT globalLeadingChanged();
    }
}
