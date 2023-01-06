/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTABSTRACTAREAWIDGET_H
#define KDCHARTABSTRACTAREAWIDGET_H

#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QWidget>

#include "KDChartAbstractAreaBase.h"

namespace KDChart {

/**
 * @class AbstractAreaWidget KDChartAbstractArea.h
 * @brief An area in the chart with a background, a frame, etc.
 *
 * AbstractAreaWidget is the base for all widget classes that have
 * a set of background attributes and frame attributes, such as
 * KDChart::Chart and KDChart::Legend.
 */
class KDCHART_EXPORT AbstractAreaWidget : public QWidget, public AbstractAreaBase
{
    Q_OBJECT

    Q_DISABLE_COPY(AbstractAreaWidget)
    KDCHART_DECLARE_PRIVATE_DERIVED_QWIDGET(AbstractAreaWidget)

public:
    explicit AbstractAreaWidget(QWidget *parent = nullptr);

    /**
     * @brief Draws the background and frame, then calls paint().
     *
     * In most cases there is no need to overwrite this method in a derived
     * class, but you would overwrite paint() instead.
     * @sa paint
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Draws the background and frame, then calls paint().
     *
     * In most cases there is no need to overwrite this method in a derived
     * class, but you would overwrite paint() instead.
     */
    virtual void paintIntoRect(QPainter &painter, const QRect &rect);

    /**
     * Overwrite this to paint the inner contents of your widget.
     *
     * @note When overriding this method, please let your widget draw
     * itself at the top/left corner of the painter.  You should call rect()
     * (or width(), height(), resp.) to find the drawable area's size:
     * While the paint() method is being executed the frame of the widget
     * is outside of its rect(), so you can use all of rect() for
     * your custom drawing!
     * @sa paint, paintIntoRect
     */
    virtual void paint(QPainter *painter) = 0;

    /**
     * Call paintAll, if you want the background and the frame to be drawn
     * before the normal paint() is invoked automatically.
     */
    void paintAll(QPainter &painter);

    /**
     * Call this to trigger an unconditional re-building of the widget's internals.
     */
    virtual void forceRebuild();

    /**
     * Call this to trigger an conditional re-building of the widget's internals.
     *
     * e.g. AbstractAreaWidget call this, before calling layout()->setGeometry()
     */
    virtual void needSizeHint();
    virtual void resizeLayout(const QSize &);

Q_SIGNALS:
    void positionChanged(AbstractAreaWidget *);

protected:
    ~AbstractAreaWidget() override;
    QRect areaGeometry() const override;
    void positionHasChanged() override;
};
}
#endif // KDCHARTABSTRACTAREAWIDGET_H
