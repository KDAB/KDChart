/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTCARTESIANAXIS_H
#define KDCHARTCARTESIANAXIS_H

#include <QList>

#include "KDChartAbstractAxis.h"

namespace KDChart {

class AbstractCartesianDiagram;

/**
 * The class for cartesian axes.
 *
 * For being useful, axes need to be assigned to a diagram, see
 * AbstractCartesianDiagram::addAxis and AbstractCartesianDiagram::takeAxis.
 *
 * \sa PolarAxis, AbstractCartesianDiagram
 */
class KDCHART_EXPORT CartesianAxis : public AbstractAxis
{
    Q_OBJECT

    Q_DISABLE_COPY(CartesianAxis)
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT(CartesianAxis, AbstractDiagram *)

public:
    enum Position
    {
        Bottom,
        Top,
        Right,
        Left
    };

    /**
     * C'tor of the class for cartesian axes.
     *
     * \note If you pass a null parent to the constructor, you need to call
     * your diagram's addAxis function to add your axis to the diagram.
     * Otherwise there is no need to call addAxis, since the constructor
     * does it already.
     *
     * \sa AbstractCartesianDiagram::addAxis
     */
    explicit CartesianAxis(AbstractCartesianDiagram *diagram = nullptr);
    ~CartesianAxis() override;

    /**
     * Returns true if both axes have the same settings.
     */
    bool compare(const CartesianAxis *other) const;

    /** reimp */
    void paint(QPainter *) override;
    /** reimp */
    void paintCtx(PaintContext *) override;

    /**
     * Sets the optional text displayed as chart title.
     */
    void setTitleText(const QString &text);
    QString titleText() const;

    /**
     * \deprecated
     * Sets the spacing between the title and the diagram.
     * Be aware that setting this value can lead to
     * collisions between axis labels and the title
     */
    Q_DECL_DEPRECATED void setTitleSpace(qreal value);
    /// \deprecated
    Q_DECL_DEPRECATED qreal titleSpace() const;

    /// \deprecated \brief use setTitleTextAttributes() instead
    Q_DECL_DEPRECATED void setTitleSize(qreal value);
    /// \deprecated
    Q_DECL_DEPRECATED qreal titleSize() const;

    void setTitleTextAttributes(const TextAttributes &a);
    /**
     * Returns the text attributes that will be used for displaying the
     * title text.
     * This is either the text attributes as specified by setTitleTextAttributes,
     * or (if setTitleTextAttributes() was not called) the default text attributes.
     * \sa resetTitleTextAttributes, hasDefaultTitleTextAttributes
     */
    TextAttributes titleTextAttributes() const;
    /**
     * Reset the title text attributes to the built-in default:
     *
     * Same font and pen as AbstractAxis::textAttributes()
     * and 1.5 times their size.
     */
    void resetTitleTextAttributes();
    bool hasDefaultTitleTextAttributes() const;

    virtual void setPosition(Position p);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    virtual const Position position() const;
#else
    virtual Position position() const;
#endif

    virtual void layoutPlanes();

    virtual bool isAbscissa() const;
    virtual bool isOrdinate() const;

    /**
     * Sets the axis annotations to \a annotations.
     * Annotations are a QMap of qreals and QStrings defining special
     * markers and their position.
     * If you use annotations, the normal ticks and values will be invisible.
     * To unset the annotations, pass an empty QMap.
     */
    void setAnnotations(const QMultiMap<qreal, QString> &annotations);
    /**
     * Returns the currently set axis annotations.
     */
    QMultiMap<qreal, QString> annotations() const;

    /**
     * Sets custom ticks on the axis.
     * Ticks are a QList of qreals defining their special position.
     */
    void setCustomTicks(const QList<qreal> &ticksPostions);
    /**
     * Returns the currently set custom ticks on the axis.
     */
    QList<qreal> customTicks() const;

    /**
     * Sets the length of custom ticks on the axis.
     */
    void setCustomTickLength(int value);
    /**
     * Returns the length of custom ticks on the axis.
     */
    int customTickLength() const;

    /** pure virtual in QLayoutItem */
    bool isEmpty() const override;
    /** pure virtual in QLayoutItem */
    Qt::Orientations expandingDirections() const override;
    /** pure virtual in QLayoutItem */
    QSize maximumSize() const override;
    /** pure virtual in QLayoutItem */
    QSize minimumSize() const override;
    /** pure virtual in QLayoutItem */
    QSize sizeHint() const override;
    /** pure virtual in QLayoutItem */
    void setGeometry(const QRect &r) override;
    /** pure virtual in QLayoutItem */
    QRect geometry() const override;

public Q_SLOTS:
    void setCachedSizeDirty() const;

    virtual int tickLength(bool subUnitTicks = false) const;
private Q_SLOTS:
    void coordinateSystemChanged();
};

typedef QList<CartesianAxis *> CartesianAxisList;
}

#endif
