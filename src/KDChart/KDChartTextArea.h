/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHART_TEXT_AREA_H
#define KDCHART_TEXT_AREA_H

#include <QObject>

#include "KDChartAbstractAreaBase.h"
#include "KDChartGlobal.h"
#include "KDChartLayoutItems.h"

namespace KDChart {

/**
 * @class TextArea KDChartTextArea.h
 * @brief A text area in the chart with a background, a frame, etc.
 *
 * TextArea is the base class for all text containing non-widget chart elements
 * that have a set of background attributes and frame attributes, such as
 * headers or footers.
 *
 * @note This class inherits AbstractAreaBase, TextLayoutItem, and QObject.
 * The reason for this triple inheritance is that neither AbstractAreaBase nor
 * TextLayoutItem inherit QObject.
 */
class KDCHART_EXPORT TextArea : public QObject, public AbstractAreaBase, public TextLayoutItem
{
    Q_OBJECT

    Q_DISABLE_COPY(TextArea)
    KDCHART_DECLARE_PRIVATE_DERIVED(TextArea)

public:
    ~TextArea() override;

    //    virtual TextArea * clone() const = 0;
    /**
     * @brief Draws the background and frame, then calls paint().
     *
     * In most cases there is no need to overwrite this method in a derived
     * class, but you would overwrite TextLayoutItem::paint() instead.
     */
    virtual void paintIntoRect(QPainter &painter, const QRect &rect);

    /**
     * Call paintAll, if you want the background and the frame to be drawn
     * before the normal paint() is invoked automatically.
     */
    void paintAll(QPainter &painter) override;

protected:
    TextArea();
    QRect areaGeometry() const override;
    void positionHasChanged() override;

Q_SIGNALS:
    void positionChanged(TextArea *);

    // KDCHART_DECLARE_PRIVATE_DERIVED(TextArea)
}; // End of class TextArea
}
#endif // KDCHART_TEXT_AREA_H
