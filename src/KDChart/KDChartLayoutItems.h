/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef KDCHARTLAYOUTITEMS_H
#define KDCHARTLAYOUTITEMS_H

#include <QBrush>
#include <QFont>
#include <QFontMetricsF>
#include <QLayout>
#include <QLayoutItem>
#include <QPen>

#include "KDChartMarkerAttributes.h"
#include "KDChartTextAttributes.h"

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

// TODO remove
QRectF rotatedRect(const QRectF &pt, qreal rotation);

namespace KDChart {
class AbstractDiagram;
class PaintContext;

/**
 * Base class for all layout items of KD Chart
 * \internal
 */
class KDCHART_EXPORT AbstractLayoutItem : public QLayoutItem
{
public:
    AbstractLayoutItem(Qt::Alignment itemAlignment = {})
        : QLayoutItem(itemAlignment)
    {
    }

    /**
     * Default impl: just call paint.
     *
     * Derived classes like KDChart::AbstractArea are providing
     * additional action here.
     */
    virtual void paintAll(QPainter &painter);

    virtual void paint(QPainter *) = 0;

    virtual void paintCtx(PaintContext *context);
    virtual void setParentWidget(QWidget *widget);
    virtual void sizeHintChanged() const;

    void setParentLayout(QLayout *lay)
    {
        mParentLayout = lay;
    }
    QLayout *parentLayout()
    {
        return mParentLayout;
    }
    void removeFromParentLayout()
    {
        if (mParentLayout) {
            if (widget())
                mParentLayout->removeWidget(widget());
            else
                mParentLayout->removeItem(this);
        }
    }

protected:
    QWidget *mParent = nullptr;
    QLayout *mParentLayout = nullptr;
};

/**
 * Layout item showing a text
 *\internal
 */
class KDCHART_EXPORT TextLayoutItem : public AbstractLayoutItem
{
public:
    TextLayoutItem();
    TextLayoutItem(const QString &text,
                   const TextAttributes &attributes,
                   const QObject *autoReferenceArea,
                   KDChartEnums::MeasureOrientation autoReferenceOrientation,
                   Qt::Alignment alignment = {});

    void setAutoReferenceArea(const QObject *area);
    const QObject *autoReferenceArea() const;

    void setText(const QString &text);
    QString text() const;

    void setTextAlignment(Qt::Alignment);
    Qt::Alignment textAlignment() const;

    void setTextAttributes(const TextAttributes &a);
    TextAttributes textAttributes() const;

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

    virtual int marginWidth() const;

    virtual QSize sizeHintUnrotated() const;

    virtual bool intersects(const TextLayoutItem &other, const QPointF &myPos, const QPointF &otherPos) const;
    virtual bool intersects(const TextLayoutItem &other, const QPoint &myPos, const QPoint &otherPos) const;

    virtual qreal realFontSize() const;
    virtual QFont realFont() const;

    void paint(QPainter *) override;

    QPolygon boundingPolygon() const;

private:
    bool maybeUpdateRealFont() const;
    QSize unrotatedSizeHint(const QFont &fnt = QFont()) const;
    QSize unrotatedTextSize(QFont fnt = QFont()) const;
    QSize calcSizeHint(const QFont &font) const;
    int marginWidth(const QSize &textSize) const;

    qreal fitFontSizeToGeometry() const;

    QRect mRect;
    QString mText;
    Qt::Alignment mTextAlignment;
    TextAttributes mAttributes;
    const QObject *mAutoReferenceArea = nullptr;
    KDChartEnums::MeasureOrientation mAutoReferenceOrientation = KDChartEnums::MeasureOrientationHorizontal;
    mutable QSize cachedSizeHint;
    mutable QPolygon mCachedBoundingPolygon;
    mutable qreal cachedFontSize = 0.0;
    mutable QFont cachedFont;
};

class KDCHART_EXPORT TextBubbleLayoutItem : public AbstractLayoutItem
{
public:
    TextBubbleLayoutItem();
    TextBubbleLayoutItem(const QString &text,
                         const TextAttributes &attributes,
                         const QObject *autoReferenceArea,
                         KDChartEnums::MeasureOrientation autoReferenceOrientation,
                         Qt::Alignment alignment = {});

    ~TextBubbleLayoutItem() override;

    void setAutoReferenceArea(const QObject *area);
    const QObject *autoReferenceArea() const;

    void setText(const QString &text);
    QString text() const;

    void setTextAttributes(const TextAttributes &a);
    TextAttributes textAttributes() const;

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

    void paint(QPainter *painter) override;

protected:
    int borderWidth() const;

private:
    TextLayoutItem *const m_text;
};

/**
 * Layout item showing a data point marker
 * \internal
 */
class KDCHART_EXPORT MarkerLayoutItem : public AbstractLayoutItem
{
public:
    MarkerLayoutItem(AbstractDiagram *diagram,
                     const MarkerAttributes &marker,
                     const QBrush &brush,
                     const QPen &pen,
                     Qt::Alignment alignment = {});

    Qt::Orientations expandingDirections() const override;
    QRect geometry() const override;
    bool isEmpty() const override;
    QSize maximumSize() const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &r) override;
    QSize sizeHint() const override;

    void paint(QPainter *) override;

    static void paintIntoRect(
        QPainter *painter,
        const QRect &rect,
        AbstractDiagram *diagram,
        const MarkerAttributes &marker,
        const QBrush &brush,
        const QPen &pen);

private:
    AbstractDiagram *mDiagram;
    QRect mRect;
    MarkerAttributes mMarker;
    QBrush mBrush;
    QPen mPen;
};

/**
 * Layout item showing a coloured line
 * \internal
 */
class KDCHART_EXPORT LineLayoutItem : public AbstractLayoutItem
{
public:
    LineLayoutItem(AbstractDiagram *diagram,
                   int length,
                   const QPen &pen,
                   Qt::Alignment mLegendLineSymbolAlignment,
                   Qt::Alignment alignment = {});

    Qt::Orientations expandingDirections() const override;
    QRect geometry() const override;
    bool isEmpty() const override;
    QSize maximumSize() const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &r) override;
    QSize sizeHint() const override;

    void setLegendLineSymbolAlignment(Qt::Alignment legendLineSymbolAlignment);
    virtual Qt::Alignment legendLineSymbolAlignment() const;

    void paint(QPainter *) override;

    static void paintIntoRect(
        QPainter *painter,
        const QRect &rect,
        const QPen &pen,
        Qt::Alignment lineAlignment);

private:
    AbstractDiagram *mDiagram; // TODO: not used. remove it
    int mLength;
    QPen mPen;
    QRect mRect;
    Qt::Alignment mLegendLineSymbolAlignment;
};

/**
 * Layout item showing a coloured line and a data point marker
 * \internal
 */
class KDCHART_EXPORT LineWithMarkerLayoutItem : public AbstractLayoutItem
{
public:
    LineWithMarkerLayoutItem(AbstractDiagram *diagram,
                             int lineLength,
                             const QPen &linePen,
                             int markerOffs,
                             const MarkerAttributes &marker,
                             const QBrush &markerBrush,
                             const QPen &markerPen,
                             Qt::Alignment alignment = {});

    Qt::Orientations expandingDirections() const override;
    QRect geometry() const override;
    bool isEmpty() const override;
    QSize maximumSize() const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &r) override;
    QSize sizeHint() const override;

    void paint(QPainter *) override;

private:
    AbstractDiagram *mDiagram;
    QRect mRect;
    int mLineLength;
    QPen mLinePen;
    int mMarkerOffs;
    MarkerAttributes mMarker;
    QBrush mMarkerBrush;
    QPen mMarkerPen;
};

/**
 * Layout item showing a horizontal line
 * \internal
 */
class KDCHART_EXPORT HorizontalLineLayoutItem : public AbstractLayoutItem
{
public:
    HorizontalLineLayoutItem();

    Qt::Orientations expandingDirections() const override;
    QRect geometry() const override;
    bool isEmpty() const override;
    QSize maximumSize() const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &r) override;
    QSize sizeHint() const override;

    void paint(QPainter *) override;

private:
    QRect mRect;
};

/**
 * Layout item showing a vertical line
 * \internal
 */
class KDCHART_EXPORT VerticalLineLayoutItem : public AbstractLayoutItem
{
public:
    VerticalLineLayoutItem();

    Qt::Orientations expandingDirections() const override;
    QRect geometry() const override;
    bool isEmpty() const override;
    QSize maximumSize() const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &r) override;
    QSize sizeHint() const override;

    void paint(QPainter *) override;

private:
    QRect mRect;
};

/**
     * @brief An empty layout item
     * \internal
     *
     * The AutoSpacerLayoutItem is automatically put into each corner cell of
     * the planeLayout grid: one of its reference-layouts is a QVBoxLayout (for
     * the top, or bottom axes resp.), the other one is a QHBoxLayout (for the
     * left/right sided axes).
     *
     * The spacer reserves enough space so all of the AbstractAreas contained
     * in the two reference-layouts can display not only their in-bounds
     * content but also their overlapping content reaching out of their area.
     *
     * KD Chart's layouting is applying this schema:
\verbatim
    +------------------+-------------------------+-----------------+
    | +--------------+ | +---------------------+ | +-------------+ |
    | |              | | |  QVBoxLayout for    | | |             | |
    | |     AUTO     | | |  the top axis/axes  | | |    AUTO     | |
    | |    SPACER    | | +---------------------+ | |   SPACER    | |
    | |     ITEM     | | |                     | | |    ITEM     | |
    | |              | | |                     | | |             | |
    | +--------------+ | +---------------------+ | +-------------+ |
    +------------------+-------------------------+-----------------+
    | +--------+-----+ | +---------------------+ | +-------+-----+ |
    | |        |     | | |                     | | |       |     | |
    | |        |     | | |                     | | |       |     | |
    | | QHBox- |     | | |                     | | | Right |     | |
    | | Layout |     | | |                     | | |       |     | |
    | |        |     | | |                     | | | axes  |     | |
    | | for    |     | | |                     | | |       |     | |
    | |        |     | | |                     | | | layout|     | |
    | | the    |     | | |      DIAGRAM(s)     | | |       |     | |
    | |        |     | | |                     | | |       |     | |
    | | left   |     | | |                     | | |       |     | |
    | |        |     | | |                     | | |       |     | |
    | | axis   |     | | |                     | | |       |     | |
    | | or     |     | | |                     | | |       |     | |
    | | axes   |     | | |                     | | |       |     | |
    | |        |     | | |                     | | |       |     | |
    | +--------+-----+ | +---------------------+ | +-------+-----+ |
    +------------------+-------------------------+-----------------+
    | +--------------+ | +---------------------+ | +-------------+ |
    | |              | | |   QVBoxLayout for   | | |             | |
    | |    AUTO      | | |   the bottom axes   | | |    AUTO     | |
    | |   SPACER     | | +---------------------+ | |   SPACER    | |
    | |    ITEM      | | |                     | | |    ITEM     | |
    | |              | | |                     | | |             | |
    | +--------------+ | +---------------------+ | +-------------+ |
    +------------------+-------------------------+-----------------+
\endverbatim
     *
     * A typical use case is an Abscissa axis with long labels:
\verbatim
    2 -|
       |
    1 -|
       |
    0 -+------------------------------------
       |        |        |        |        |
    Monday  Tuesday  Wednesday Thursday Friday
\endverbatim
     * The last letters of the word "Friday" would have been
     * cut off in previous versions of KD Chart - that is
     * if you did not call KDChart::Chart::setGlobalLeading().
     *
     * Now the word will be shown completely because there
     * is an auto-spacer-item taking care for the additional
     * space needed in the lower/right corner.
     */
class KDCHART_EXPORT AutoSpacerLayoutItem : public AbstractLayoutItem
{
public:
    AutoSpacerLayoutItem(
        bool layoutIsAtTopPosition, QHBoxLayout *rightLeftLayout,
        bool layoutIsAtLeftPosition, QVBoxLayout *topBottomLayout);

    Qt::Orientations expandingDirections() const override;
    QRect geometry() const override;
    bool isEmpty() const override;
    QSize maximumSize() const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect &r) override;
    QSize sizeHint() const override;

    void paint(QPainter *) override;

private:
    QRect mRect;
    bool mLayoutIsAtTopPosition;
    QHBoxLayout *mRightLeftLayout;
    bool mLayoutIsAtLeftPosition;
    QVBoxLayout *mTopBottomLayout;

    mutable QBrush mCommonBrush;
    mutable QSize mCachedSize;
};
}

#endif /* KDCHARTLAYOUTITEMS_H */
