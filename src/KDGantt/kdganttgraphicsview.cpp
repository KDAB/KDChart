/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "kdganttgraphicsview.h"
#include "kdganttabstractrowcontroller.h"
#include "kdganttconstraintmodel.h"
#include "kdganttgraphicsitem.h"
#include "kdganttgraphicsview_p.h"

#include <QAbstractProxyModel>
#include <QActionGroup>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QPrinter>
#include <QResizeEvent>
#include <QScrollBar>

#include <cassert>

#if defined KDAB_EVAL
#include "../evaldialog/evaldialog.h"
#endif

/*!\class KDGantt::HeaderWidget
 * \internal
 */

using namespace KDGantt;

HeaderWidget::HeaderWidget(GraphicsView *parent)
    : QWidget(parent)
    , m_offset(0.)
{
    assert(parent); // Parent must be set
}

HeaderWidget::~HeaderWidget()
{
}

void HeaderWidget::scrollTo(int v)
{
    m_offset = v;
    // QWidget::scroll() won't work properly for me on Mac
    // scroll( static_cast<int>( old-v ), 0 );
    update();
}

void HeaderWidget::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    view()->grid()->paintHeader(&p, rect(), ev->rect(), m_offset, this);
}

bool HeaderWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        auto *const grid = qobject_cast<DateTimeGrid *>(view()->grid());
        if (grid) {
            auto *e = static_cast<QHelpEvent *>(event);
            QDateTime dt = grid->mapFromChart(view()->mapToScene(e->x(), 0).x()).toDateTime();
            setToolTip(dt.toString());
        }
    }
    return QWidget::event(event);
}

void HeaderWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu contextMenu;

    auto *const grid = qobject_cast<DateTimeGrid *>(view()->grid());
    QAction *actionScaleAuto = nullptr;
    QAction *actionScaleMonth = nullptr;
    QAction *actionScaleWeek = nullptr;
    QAction *actionScaleDay = nullptr;
    QAction *actionScaleHour = nullptr;
    QAction *actionZoomIn = nullptr;
    QAction *actionZoomOut = nullptr;
    if (grid != nullptr) {
        auto *menuScale = new QMenu(tr("Scale"), &contextMenu);
        auto *scaleGroup = new QActionGroup(&contextMenu);
        scaleGroup->setExclusive(true);

        actionScaleAuto = new QAction(tr("Auto"), menuScale);
        actionScaleAuto->setCheckable(true);
        actionScaleAuto->setChecked(grid->scale() == DateTimeGrid::ScaleAuto);
        actionScaleMonth = new QAction(tr("Month"), menuScale);
        actionScaleMonth->setCheckable(true);
        actionScaleMonth->setChecked(grid->scale() == DateTimeGrid::ScaleMonth);
        actionScaleWeek = new QAction(tr("Week"), menuScale);
        actionScaleWeek->setCheckable(true);
        actionScaleWeek->setChecked(grid->scale() == DateTimeGrid::ScaleWeek);
        actionScaleDay = new QAction(tr("Day"), menuScale);
        actionScaleDay->setCheckable(true);
        actionScaleDay->setChecked(grid->scale() == DateTimeGrid::ScaleDay);
        actionScaleHour = new QAction(tr("Hour"), menuScale);
        actionScaleHour->setCheckable(true);
        actionScaleHour->setChecked(grid->scale() == DateTimeGrid::ScaleHour);

        scaleGroup->addAction(actionScaleAuto);
        menuScale->addAction(actionScaleAuto);

        scaleGroup->addAction(actionScaleMonth);
        menuScale->addAction(actionScaleMonth);

        scaleGroup->addAction(actionScaleWeek);
        menuScale->addAction(actionScaleWeek);

        scaleGroup->addAction(actionScaleDay);
        menuScale->addAction(actionScaleDay);

        scaleGroup->addAction(actionScaleHour);
        menuScale->addAction(actionScaleHour);

        contextMenu.addMenu(menuScale);

        contextMenu.addSeparator();

        actionZoomIn = new QAction(tr("Zoom In"), &contextMenu);
        contextMenu.addAction(actionZoomIn);
        actionZoomOut = new QAction(tr("Zoom Out"), &contextMenu);
        contextMenu.addAction(actionZoomOut);
    }

    if (contextMenu.isEmpty()) {
        event->ignore();
        return;
    }

    const QAction *const action = contextMenu.exec(event->globalPos());
    if (action == nullptr) {
    } else if (action == actionScaleAuto) {
        assert(grid != nullptr);
        grid->setScale(DateTimeGrid::ScaleAuto);
    } else if (action == actionScaleMonth) {
        assert(grid != nullptr);
        grid->setScale(DateTimeGrid::ScaleMonth);
    } else if (action == actionScaleWeek) {
        assert(grid != nullptr);
        grid->setScale(DateTimeGrid::ScaleWeek);
    } else if (action == actionScaleDay) {
        assert(grid != nullptr);
        grid->setScale(DateTimeGrid::ScaleDay);
    } else if (action == actionScaleHour) {
        assert(grid != nullptr);
        grid->setScale(DateTimeGrid::ScaleHour);
    } else if (action == actionZoomIn) {
        assert(grid != nullptr);
        grid->setDayWidth(qMax(0.1, grid->dayWidth() + grid->dayWidth() * 0.2));
    } else if (action == actionZoomOut) {
        assert(grid != nullptr);
        grid->setDayWidth(qMax(0.1, grid->dayWidth() - grid->dayWidth() * 0.2));
    }

    event->accept();
}

GraphicsView::Private::Private(GraphicsView *_q)
    : q(_q)
    , rowcontroller(nullptr)
    , headerwidget(_q)
{
}

void GraphicsView::Private::updateHeaderGeometry()
{
    q->setViewportMargins(0, rowcontroller->headerHeight(), 0, 0);
    headerwidget.setGeometry(q->viewport()->x(),
                             q->viewport()->y() - rowcontroller->headerHeight(),
                             q->viewport()->width(),
                             rowcontroller->headerHeight());
}

void GraphicsView::Private::slotGridChanged()
{
    updateHeaderGeometry();
    headerwidget.update();
    q->updateSceneRect();
    q->update();
}

void GraphicsView::Private::slotHorizontalScrollValueChanged(int val)
{
    const QRectF viewRect = q->transform().mapRect(q->sceneRect());
    headerwidget.scrollTo(val - q->horizontalScrollBar()->minimum() + static_cast<int>(viewRect.left()));
}

void GraphicsView::Private::slotColumnsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(start);
    Q_UNUSED(end);
    QModelIndex idx = scene.model()->index(0, 0, scene.summaryHandlingModel()->mapToSource(parent));
    do {
        scene.updateRow(scene.summaryHandlingModel()->mapFromSource(idx));
    } while ((idx = rowcontroller->indexBelow(idx)) != QModelIndex() && rowcontroller->isRowVisible(idx));
    //} while ( ( idx = d->treeview.indexBelow( idx ) ) != QModelIndex() && d->treeview.visualRect(idx).isValid() );
    q->updateSceneRect();
}

void GraphicsView::Private::slotColumnsRemoved(const QModelIndex &parent, int start, int end)
{
    // TODO
    Q_UNUSED(start);
    Q_UNUSED(end);
    Q_UNUSED(parent);
    q->updateScene();
}

void GraphicsView::Private::slotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    // qDebug() << "GraphicsView::slotDataChanged("<<topLeft<<bottomRight<<")";
    const QModelIndex parent = topLeft.parent();
    for (int row = topLeft.row(); row <= bottomRight.row(); ++row) {
        scene.updateRow(scene.summaryHandlingModel()->index(row, 0, parent));
    }
}

void GraphicsView::Private::slotLayoutChanged()
{
    // qDebug() << "slotLayoutChanged()";
    q->updateScene();
}

void GraphicsView::Private::slotModelReset()
{
    // qDebug() << "slotModelReset()";
    q->updateScene();
}

void GraphicsView::Private::slotRowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);
    q->updateScene(); // TODO: This might be optimised
}

void GraphicsView::Private::slotRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    // qDebug() << "GraphicsView::Private::slotRowsAboutToBeRemoved("<<parent<<start<<end<<")";
    for (int row = start; row <= end; ++row) {
        for (int col = 0; col < scene.summaryHandlingModel()->columnCount(parent); ++col) {
            // qDebug() << "removing "<<scene.summaryHandlingModel()->index( row, col, parent );
            scene.removeItem(scene.summaryHandlingModel()->index(row, col, parent));
        }
    }
}

void GraphicsView::Private::slotRowsRemoved(const QModelIndex &parent, int start, int end)
{
    // qDebug() << "GraphicsView::Private::slotRowsRemoved("<<parent<<start<<end<<")";
    //  TODO
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    q->updateScene();
}

void GraphicsView::Private::slotItemClicked(const QModelIndex &idx)
{
    QModelIndex sidx = idx; // scene.summaryHandlingModel()->mapToSource( idx );
    Q_EMIT q->clicked(sidx);
    if (q->style()->styleHint(QStyle::SH_ItemView_ActivateItemOnSingleClick, nullptr, q))
        Q_EMIT q->activated(sidx);
}

void GraphicsView::Private::slotItemDoubleClicked(const QModelIndex &idx)
{
    QModelIndex sidx = idx; // scene.summaryHandlingModel()->mapToSource( idx );
    Q_EMIT q->qrealClicked(sidx);
    if (!q->style()->styleHint(QStyle::SH_ItemView_ActivateItemOnSingleClick, nullptr, q))
        Q_EMIT q->activated(sidx);
}

void GraphicsView::Private::slotHeaderContextMenuRequested(const QPoint &pt)
{
    Q_EMIT q->headerContextMenuRequested(headerwidget.mapToGlobal(pt));
}

/*!\class KDGantt::GraphicsView kdganttgraphicsview.h KDGanttGraphicsView
 * \ingroup KDGantt
 * \brief The GraphicsView class provides a model/view implementation of a gantt chart.
 *
 *
 */

/*! \fn void GraphicsView::activated( const QModelIndex & index ) */
/*! \fn void GraphicsView::clicked( const QModelIndex & index ); */
/*! \fn void GraphicsView::qrealClicked( const QModelIndex & index ); */
/*! \fn void GraphicsView::entered( const QModelIndex & index ); */
/*! \fn void GraphicsView::pressed( const QModelIndex & index ); */
/*! \fn void GraphicsView::headerContextMenuRequested( const QPoint& pt )
 * This signal is emitted when the header has contextMenuPolicy Qt::CustomContextMenu
 * and the widget wants to show a context menu for the header. Unlike in
 * QWidget::customContextMenuRequested() signal, \a pt is here in global coordinates.
 */

/*! Constructor. Creates a new KDGantt::GraphicsView with parent
 * \a parent.
 */
GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , _d(new Private(this))
{
#if defined KDAB_EVAL
    EvalDialog::checkEvalLicense("KD Gantt");
#endif
    connect(horizontalScrollBar(), &QScrollBar::valueChanged,
            this, [this](int value) {
                _d->slotHorizontalScrollValueChanged(value);
            });
    connect(&_d->scene, &GraphicsScene::gridChanged,
            this, [this] {
                _d->slotGridChanged();
            });
    connect(&_d->scene, &GraphicsScene::entered,
            this, &GraphicsView::entered);
    connect(&_d->scene, &GraphicsScene::pressed,
            this, &GraphicsView::pressed);
    connect(&_d->scene, &GraphicsScene::clicked,
            this, [this](const QModelIndex &index) {
                _d->slotItemClicked(index);
            });
    connect(&_d->scene, &GraphicsScene::qrealClicked,
            this, [this](const QModelIndex &index) {
                _d->slotItemDoubleClicked(index);
            });
    connect(&_d->scene, &GraphicsScene::sceneRectChanged,
            this, &GraphicsView::updateSceneRect);
    connect(&_d->headerwidget, &HeaderWidget::customContextMenuRequested,
            this, [this](const QPoint &point) {
                _d->slotHeaderContextMenuRequested(point);
            });
    setScene(&_d->scene);

    // HACK!
    setSummaryHandlingModel(_d->scene.summaryHandlingModel());

    // So that AbstractGrid::drawBackground() and AbstractGrid::drawForeground()
    // works properly
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // setCacheMode( CacheBackground );
}

/*! Destroys this view. */
GraphicsView::~GraphicsView()
{
    delete _d;
}

#define d d_func()

/*! Sets the model to be displayed in this view to
 * \a model. The view does not take ownership of the model.
 *
 * To make a model work well with GraphicsView it must
 * have a certain layout. Whether the model is flat or has a
 * treestrucure is not important, as long as an
 * AbstractRowController is provided that can navigate the
 * model.
 *
 * GraphicsView operates per row in the model. The data is always
 * taken from the _last_ item in the row. The ItemRoles used are
 * Qt::DisplayRole and the roles defined in KDGantt::ItemDataRole.
 */
void GraphicsView::setModel(QAbstractItemModel *model)
{
    if (d->scene.model()) {
        disconnect(d->scene.model());
    }

    d->scene.setModel(model);
    connect(model, &QAbstractItemModel::dataChanged, this, &GraphicsView::updateSceneRect);
    updateScene();
}

/*! \returns the current model displayed by this view
 */
QAbstractItemModel *GraphicsView::model() const
{
    return d->scene.model();
}

void GraphicsView::setSummaryHandlingModel(QAbstractProxyModel *proxyModel)
{
    disconnect(d->scene.summaryHandlingModel());
    d->scene.setSummaryHandlingModel(proxyModel);

    /* Connections. We have to rely on the treeview
     * to receive the signals before we do(!)
     */
    connect(proxyModel, &QAbstractProxyModel::columnsInserted,
            this, [this](const QModelIndex &parent, int first, int last) {
                d->slotColumnsInserted(parent, first, last);
            });
    connect(proxyModel, &QAbstractProxyModel::columnsRemoved,
            this, [this](const QModelIndex &parent, int first, int last) {
                d->slotColumnsRemoved(parent, first, last);
            });
    connect(proxyModel, &QAbstractProxyModel::dataChanged,
            this, [this](const QModelIndex &topLeft, const QModelIndex &bottomRight) {
                d->slotDataChanged(topLeft, bottomRight);
            });
    connect(proxyModel, &QAbstractProxyModel::layoutChanged,
            this, [this] {
                d->slotLayoutChanged();
            });
    connect(proxyModel, &QAbstractProxyModel::modelReset,
            this, [this] {
                d->slotModelReset();
            });
    connect(proxyModel, &QAbstractProxyModel::rowsInserted,
            this, [this](const QModelIndex &parent, int first, int last) {
                d->slotRowsInserted(parent, first, last);
            });
    connect(proxyModel, &QAbstractProxyModel::rowsAboutToBeRemoved,
            this, [this](const QModelIndex &parent, int first, int last) {
                d->slotRowsAboutToBeRemoved(parent, first, last);
            });
    connect(proxyModel, &QAbstractProxyModel::rowsRemoved,
            this, [this](const QModelIndex &parent, int first, int last) {
                d->slotRowsRemoved(parent, first, last);
            });

    updateScene();
}

/*! Sets the constraintmodel displayed by this view.
 * \see KDGantt::ConstraintModel.
 */
void GraphicsView::setConstraintModel(ConstraintModel *cmodel)
{
    d->scene.setConstraintModel(cmodel);
}

/*! \returns the KDGantt::ConstraintModel displayed by this view.
 */
ConstraintModel *GraphicsView::constraintModel() const
{
    return d->scene.constraintModel();
}

/*! \returns the KDGantt::SummaryHandlingProxyModel used by this view.
 */
QAbstractProxyModel *GraphicsView::summaryHandlingModel() const
{
    return d->scene.summaryHandlingModel();
}

/*! Sets the root index of the model displayed by this view.
 * Similar to QAbstractItemView::setRootIndex, default is QModelIndex().
 */
void GraphicsView::setRootIndex(const QModelIndex &idx)
{
    d->scene.setRootIndex(idx);
}

/*! \returns the rootindex for this view.
 */
QModelIndex GraphicsView::rootIndex() const
{
    return d->scene.rootIndex();
}

/*! Sets the QItemSelectionModel used by this view to manage
 * selections. Similar to QAbstractItemView::setSelectionModel
 */
void GraphicsView::setSelectionModel(QItemSelectionModel *model)
{
    d->scene.setSelectionModel(model);
}

/*! \returns the QItemSelectionModel used by this view
 */
QItemSelectionModel *GraphicsView::selectionModel() const
{
    return d->scene.selectionModel();
}

/*! Sets the KDGantt::ItemDelegate used for rendering items on this
 * view. \see ItemDelegate and QAbstractItemDelegate.
 */
void GraphicsView::setItemDelegate(ItemDelegate *delegate)
{
    d->scene.setItemDelegate(delegate);
}

/*! \returns the ItemDelegate used by this view to render items
 */
ItemDelegate *GraphicsView::itemDelegate() const
{
    return d->scene.itemDelegate();
}

/*! Sets the AbstractRowController used by this view. The
 * AbstractRowController deals with the height and position
 * of each row and with which parts of the model are
 * displayed. \see AbstractRowController
 */
void GraphicsView::setRowController(AbstractRowController *rowcontroller)
{
    d->rowcontroller = rowcontroller;
    d->scene.setRowController(rowcontroller);
    updateScene();
}

/*! \returns the AbstractRowController
 * for this view. \see setRowController
 */
AbstractRowController *GraphicsView::rowController() const
{
    return d->rowcontroller;
}

/*! Sets the AbstractGrid for this view. The grid is an
 * object that controls how QModelIndexes are mapped
 * to and from the view and how the background and header
 * is rendered. \see AbstractGrid and DateTimeGrid.
 */
void GraphicsView::setGrid(AbstractGrid *grid)
{
    d->scene.setGrid(grid);
    d->slotGridChanged();
}

/*! \returns the AbstractGrid used by this view.
 */
AbstractGrid *GraphicsView::grid() const
{
    return d->scene.grid();
}

/*! Sets the view to read-only mode if \a to is true. The default is
 * read/write if the model permits it.
 */
void GraphicsView::setReadOnly(bool ro)
{
    d->scene.setReadOnly(ro);
}

/*!\returns true iff the view is in read-only mode
 */
bool GraphicsView::isReadOnly() const
{
    return d->scene.isReadOnly();
}

/*! Sets the context menu policy for the header. The default value
 * Qt::DefaultContextMenu results in a standard context menu on the header
 * that allows the user to set the scale and zoom.
 *
 * Setting this to Qt::CustomContextMenu will cause the signal
 * headerContextMenuRequested(const QPoint& pt) to be emitted instead.
 *
 * \see QWidget::setContextMenuPolicy( Qt::ContextMenuPolicy )
 */
void GraphicsView::setHeaderContextMenuPolicy(Qt::ContextMenuPolicy p)
{
    d->headerwidget.setContextMenuPolicy(p);
}

/*! \returns the context menu policy for the header
 */
Qt::ContextMenuPolicy GraphicsView::headerContextMenuPolicy() const
{
    return d->headerwidget.contextMenuPolicy();
}

/*! Adds a constraint from \a from to \a to. \a modifiers are the
 * keyboard modifiers pressed by the user when the action is invoked.
 *
 * Override this to control how constraints are added. The default
 * implementation adds a soft constraint unless the Shift key is pressed,
 * in that case it adds a hard constraint. If a constraint is already
 * present, it is removed and nothing is added.
 */
void GraphicsView::addConstraint(const QModelIndex &from,
                                 const QModelIndex &to,
                                 Qt::KeyboardModifiers modifiers)
{
    if (isReadOnly())
        return;
    ConstraintModel *cmodel = constraintModel();
    assert(cmodel);
    Constraint c(from, to, (modifiers & Qt::ShiftModifier) ? Constraint::TypeHard : Constraint::TypeSoft);
    if (cmodel->hasConstraint(c))
        cmodel->removeConstraint(c);
    else
        cmodel->addConstraint(c);
}

void GraphicsView::resizeEvent(QResizeEvent *ev)
{
    d->updateHeaderGeometry();
    QRectF r = scene()->itemsBoundingRect();
    // To scroll more to the left than the actual item start, bug #4516
    r.setLeft(qMin<qreal>(0.0, r.left()));
    // TODO: take scrollbars into account (if not always on)
    // The scene should be at least the size of the viewport
    QSizeF size = viewport()->size();
    // TODO: why -2 below? size should be ex. frames etc?
    if (size.width() > r.width()) {
        r.setWidth(size.width() - 2);
    }
    if (size.height() > r.height()) {
        r.setHeight(size.height() - 2);
    }
    const int totalh = rowController()->totalHeight();
    if (r.height() < totalh) {
        r.setHeight(totalh);
    }

    scene()->setSceneRect(r);

    QGraphicsView::resizeEvent(ev);
}

/*!\returns The QModelIndex for the item located at
 * position \a pos in the view or an invalid index
 * if no item was present at that position.
 *
 * This is useful for example contextmenus.
 */
QModelIndex GraphicsView::indexAt(const QPoint &pos) const
{
    QGraphicsItem *item = itemAt(pos);
    if (auto *gitem = qgraphicsitem_cast<GraphicsItem *>(item)) {
        return d->scene.summaryHandlingModel()->mapToSource(gitem->index());
    } else {
        return QModelIndex();
    }
}

/*! \internal */
void GraphicsView::clearItems()
{
    d->scene.clearItems();
}

/*! \internal */
void GraphicsView::updateRow(const QModelIndex &idx)
{
    d->scene.updateRow(d->scene.summaryHandlingModel()->mapFromSource(idx));
}

/*! \internal
 * Adjusts the bounding rectangle of the scene.
 */
void GraphicsView::updateSceneRect()
{
    /* What to do with this? We need to shrink the view to
     * make collapsing items work
     */
    qreal range = horizontalScrollBar()->maximum() - horizontalScrollBar()->minimum();
    const qreal hscroll = horizontalScrollBar()->value() / (range > 0 ? range : 1);
    QRectF r = d->scene.itemsBoundingRect();
    // To scroll more to the left than the actual item start, bug #4516
    r.setTop(0.);
    r.setLeft(qMin<qreal>(0.0, r.left()));
    r.setSize(r.size().expandedTo(viewport()->size()));
    const int totalh = rowController()->totalHeight();
    if (r.height() < totalh)
        r.setHeight(totalh);
    d->scene.setSceneRect(r);

    /* set scrollbar to keep the same time in view */
    range = horizontalScrollBar()->maximum() - horizontalScrollBar()->minimum();
    if (range > 0)
        horizontalScrollBar()->setValue(qRound(hscroll * range));

    /* We have to update here to adjust for any rows with no
     * information because they are painted with a different
     * background brush
     */
    d->scene.invalidate(QRectF(), QGraphicsScene::BackgroundLayer);
}

/*! \internal
 * Resets the state of the view.
 */
void GraphicsView::updateScene()
{
    clearItems();
    if (!model())
        return;
    if (!rowController())
        return;
    QModelIndex idx = model()->index(0, 0, rootIndex());
    do {
        updateRow(idx);
    } while ((idx = rowController()->indexBelow(idx)) != QModelIndex() && rowController()->isRowVisible(idx));
    // constraintModel()->cleanup();
    // qDebug() << constraintModel();
    updateSceneRect();
    if (scene())
        scene()->invalidate(QRectF(), QGraphicsScene::BackgroundLayer);
}

/*! \internal */
GraphicsItem *GraphicsView::createItem(ItemType type) const
{
    return d->scene.createItem(type);
}

/*! \internal */
void GraphicsView::deleteSubtree(const QModelIndex &idx)
{
    d->scene.deleteSubtree(d->scene.summaryHandlingModel()->mapFromSource(idx));
}

/*! Print the Gantt chart using \a printer. If \a drawRowLabels
 * is true (the default), each row will have it's label printed
 * on the left side. If \a drawColumnLabels is true (the
 * default), each column will have it's label printed at the
 * top side.
 *
 * This version of print() will print multiple pages.
 */
void GraphicsView::print(QPrinter *printer, bool drawRowLabels, bool drawColumnLabels)
{
    d->scene.print(printer, drawRowLabels, drawColumnLabels);
}

/*! Print part of the Gantt chart from \a start to \a end using \a printer.
 * If \a drawRowLabels is true (the default), each row will have it's
 * label printed on the left side. If \a drawColumnLabels is true (the
 * default), each column will have it's label printed at the
 * top side.
 *
 * This version of print() will print multiple pages.
 *
 * To print a certain range of a chart with a DateTimeGrid, use
 * qreal DateTimeGrid::mapFromDateTime( const QDateTime& dt) const
 * to figure out the values for \a start and \a end.
 */
void GraphicsView::print(QPrinter *printer, qreal start, qreal end, bool drawRowLabels, bool drawColumnLabels)
{
    d->scene.print(printer, start, end, drawRowLabels, drawColumnLabels);
}

/*! Render the GanttView inside the rectangle \a target using the painter \a painter.
 * If \a drawRowLabels is true (the default), each row will have it's
 * label printed on the left side. If \a drawColumnLabels is true (the
 * default), each column will have it's label printed at the
 * top side.
 */
void GraphicsView::print(QPainter *painter, const QRectF &targetRect, bool drawRowLabels, bool drawColumnLabels)
{
    d->scene.print(painter, targetRect, drawRowLabels, drawColumnLabels);
}

/*! Render the GanttView inside the rectangle \a target using the painter \a painter.
 * If \a drawRowLabels is true (the default), each row will have it's
 * label printed on the left side. If \a drawColumnLabels is true (the
 * default), each column will have it's label printed at the
 * top side.
 *
 * To print a certain range of a chart with a DateTimeGrid, use
 * qreal DateTimeGrid::mapFromDateTime( const QDateTime& dt) const
 * to figure out the values for \a start and \a end.
 */
void GraphicsView::print(QPainter *painter, qreal start, qreal end,
                         const QRectF &targetRect, bool drawRowLabels, bool drawColumnLabels)
{
    d->scene.print(painter, start, end, targetRect, drawRowLabels, drawColumnLabels);
}

#include "moc_kdganttgraphicsview.cpp"
