/****************************************************************************
** Copyright (C) 2001-2016 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef KDCHARTLAYOUTITEMS_H
#define KDCHARTLAYOUTITEMS_H

#include <QBrush>
#include <QFont>
#include <QFontMetricsF>
#include <QLayout>
#include <QLayoutItem>
#include <QPen>

#include "KDChartTextAttributes.h"
#include "KDChartMarkerAttributes.h"

QT_BEGIN_NAMESPACE
class QPainter;
class KDTextDocument;
QT_END_NAMESPACE

// TODO remove
QRectF rotatedRect( const QRectF& pt, qreal rotation );

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
        AbstractLayoutItem( Qt::Alignment itemAlignment = 0 ) :
            QLayoutItem( itemAlignment ),
            mParent( 0 ),
            mParentLayout( 0 ) {}

        /**
         * Default impl: just call paint.
         *
         * Derived classes like KDChart::AbstractArea are providing
         * additional action here.
         */
        virtual void paintAll( QPainter& painter );

        virtual void paint( QPainter* ) = 0;

        virtual void paintCtx( PaintContext* context );
        virtual void setParentWidget( QWidget* widget );
        virtual void sizeHintChanged() const;

        void setParentLayout( QLayout* lay )
        {
            mParentLayout = lay;
        }
        QLayout* parentLayout()
        {
            return mParentLayout;
        }
        void removeFromParentLayout()
        {
            if ( mParentLayout ) {
                if ( widget() )
                    mParentLayout->removeWidget( widget() );
                else
                    mParentLayout->removeItem( this );
            }
        }
    protected:
        QWidget* mParent;
        QLayout* mParentLayout;
    };

    /**
     * Layout item showing a text
     *\internal
     */
    class KDCHART_EXPORT TextLayoutItem : public AbstractLayoutItem
    {
    public:
        TextLayoutItem();
        TextLayoutItem( const QString& text,
                        const TextAttributes& attributes,
                        const QObject* autoReferenceArea,
                        KDChartEnums::MeasureOrientation autoReferenceOrientation,
                        Qt::Alignment alignment = 0 );

        void setAutoReferenceArea( const QObject* area );
        const QObject* autoReferenceArea() const;

        void setText(const QString & text);
        QString text() const;

        void setTextAlignment( Qt::Alignment );
        Qt::Alignment textAlignment() const;

        void setTextAttributes( const TextAttributes& a );
        TextAttributes textAttributes() const;

        /** pure virtual in QLayoutItem */
        virtual bool isEmpty() const;
        /** pure virtual in QLayoutItem */
        virtual Qt::Orientations expandingDirections() const;
        /** pure virtual in QLayoutItem */
        virtual QSize maximumSize() const;
        /** pure virtual in QLayoutItem */
        virtual QSize minimumSize() const;
        /** pure virtual in QLayoutItem */
        virtual QSize sizeHint() const;
        /** pure virtual in QLayoutItem */
        virtual void setGeometry( const QRect& r );
        /** pure virtual in QLayoutItem */
        virtual QRect geometry() const;

        virtual int marginWidth() const;

        virtual QSize sizeHintUnrotated() const;

        virtual bool intersects( const TextLayoutItem& other, const QPointF& myPos, const QPointF& otherPos ) const;
        virtual bool intersects( const TextLayoutItem& other, const QPoint& myPos, const QPoint& otherPos ) const;

        virtual qreal realFontSize() const;
        virtual QFont realFont() const;

        virtual void paint( QPainter* );

        QPolygon boundingPolygon() const;
    private:
        bool maybeUpdateRealFont() const;
        QSize unrotatedSizeHint( const QFont& fnt = QFont() ) const;
        QSize unrotatedTextSize( QFont fnt = QFont() ) const;
        QSize calcSizeHint( const QFont& font ) const;
        int marginWidth( const QSize& textSize ) const;

        qreal fitFontSizeToGeometry() const;

        QRect mRect;
        QString mText;
        Qt::Alignment mTextAlignment;
        TextAttributes mAttributes;
        const QObject* mAutoReferenceArea;
        KDChartEnums::MeasureOrientation mAutoReferenceOrientation;
        mutable QSize cachedSizeHint;
        mutable QPolygon mCachedBoundingPolygon;
        mutable qreal cachedFontSize;
        mutable QFont cachedFont;
    };

    class KDCHART_EXPORT TextBubbleLayoutItem : public AbstractLayoutItem
    {
    public:
        TextBubbleLayoutItem();
        TextBubbleLayoutItem( const QString& text,
                              const TextAttributes& attributes,
                              const QObject* autoReferenceArea,
                              KDChartEnums::MeasureOrientation autoReferenceOrientation,
                              Qt::Alignment alignment = 0 );

        ~TextBubbleLayoutItem();

        void setAutoReferenceArea( const QObject* area );
        const QObject* autoReferenceArea() const;

        void setText(const QString & text);
        QString text() const;

        void setTextAttributes( const TextAttributes& a );
        TextAttributes textAttributes() const;

        /** pure virtual in QLayoutItem */
        virtual bool isEmpty() const;
        /** pure virtual in QLayoutItem */
        virtual Qt::Orientations expandingDirections() const;
        /** pure virtual in QLayoutItem */
        virtual QSize maximumSize() const;
        /** pure virtual in QLayoutItem */
        virtual QSize minimumSize() const;
        /** pure virtual in QLayoutItem */
        virtual QSize sizeHint() const;
        /** pure virtual in QLayoutItem */
        virtual void setGeometry( const QRect& r );
        /** pure virtual in QLayoutItem */
        virtual QRect geometry() const;

        virtual void paint( QPainter* painter );

    protected:
        int borderWidth() const;

    private:
        TextLayoutItem* const m_text;
    };

    /**
     * Layout item showing a data point marker
     * \internal
     */
    class KDCHART_EXPORT MarkerLayoutItem : public AbstractLayoutItem
    {
        public:
            MarkerLayoutItem( AbstractDiagram* diagram,
                              const MarkerAttributes& marker,
                              const QBrush& brush,
                              const QPen& pen,
                              Qt::Alignment alignment = 0 );

            virtual Qt::Orientations expandingDirections() const;
            virtual QRect geometry() const;
            virtual bool isEmpty() const;
            virtual QSize maximumSize() const;
            virtual QSize minimumSize() const;
            virtual void setGeometry( const QRect& r );
            virtual QSize sizeHint() const;

            virtual void paint( QPainter* );

            static void paintIntoRect(
                    QPainter* painter,
                    const QRect& rect,
                    AbstractDiagram* diagram,
                    const MarkerAttributes& marker,
                    const QBrush& brush,
                    const QPen& pen );

        private:
            AbstractDiagram* mDiagram;
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
            LineLayoutItem( AbstractDiagram* diagram,
                            int length,
                            const QPen& pen,
                            Qt::Alignment mLegendLineSymbolAlignment,
                            Qt::Alignment alignment = 0 );

            virtual Qt::Orientations expandingDirections() const;
            virtual QRect geometry() const;
            virtual bool isEmpty() const;
            virtual QSize maximumSize() const;
            virtual QSize minimumSize() const;
            virtual void setGeometry( const QRect& r );
            virtual QSize sizeHint() const;

            void setLegendLineSymbolAlignment(Qt::Alignment legendLineSymbolAlignment);
            virtual Qt::Alignment legendLineSymbolAlignment() const;

            virtual void paint( QPainter* );

            static void paintIntoRect(
                    QPainter* painter,
                    const QRect& rect,
                    const QPen& pen,
                    Qt::Alignment lineAlignment);

        private:
            AbstractDiagram* mDiagram;  //TODO: not used. remove it
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
            LineWithMarkerLayoutItem( AbstractDiagram* diagram,
                                      int lineLength,
                                      const QPen& linePen,
                                      int markerOffs,
                                      const MarkerAttributes& marker,
                                      const QBrush& markerBrush,
                                      const QPen& markerPen,
                                      Qt::Alignment alignment = 0 );

            virtual Qt::Orientations expandingDirections() const;
            virtual QRect geometry() const;
            virtual bool isEmpty() const;
            virtual QSize maximumSize() const;
            virtual QSize minimumSize() const;
            virtual void setGeometry( const QRect& r );
            virtual QSize sizeHint() const;

            virtual void paint( QPainter* );

        private:
            AbstractDiagram* mDiagram;
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

        virtual Qt::Orientations expandingDirections() const;
        virtual QRect geometry() const;
        virtual bool isEmpty() const;
        virtual QSize maximumSize() const;
        virtual QSize minimumSize() const;
        virtual void setGeometry( const QRect& r );
        virtual QSize sizeHint() const;

        virtual void paint( QPainter* );

    private:
        QRect mRect;
    };

    /**
     * Layout item showing a vertial line
     * \internal
     */
    class KDCHART_EXPORT VerticalLineLayoutItem : public AbstractLayoutItem
    {
        public:
            VerticalLineLayoutItem();

            virtual Qt::Orientations expandingDirections() const;
            virtual QRect geometry() const;
            virtual bool isEmpty() const;
            virtual QSize maximumSize() const;
            virtual QSize minimumSize() const;
            virtual void setGeometry( const QRect& r );
            virtual QSize sizeHint() const;

            virtual void paint( QPainter* );

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
                    bool layoutIsAtLeftPosition,  QVBoxLayout *topBottomLayout );

            virtual Qt::Orientations expandingDirections() const;
            virtual QRect geometry() const;
            virtual bool isEmpty() const;
            virtual QSize maximumSize() const;
            virtual QSize minimumSize() const;
            virtual void setGeometry( const QRect& r );
            virtual QSize sizeHint() const;

            virtual void paint( QPainter* );

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
