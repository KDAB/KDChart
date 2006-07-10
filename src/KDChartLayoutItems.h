/****************************************************************************
 ** Copyright (C) 2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
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

class QPainter;
class KDTextDocument;

namespace KDChart {
    class AbstractDiagram;
    class PaintContext;

    /** \internal
     */
    class KDCHART_EXPORT AbstractLayoutItem : public QLayoutItem
    {
    public:
        AbstractLayoutItem( Qt::Alignment alignment = 0 ) :
            QLayoutItem( alignment ),
            mParent( 0 ),
            mParentLayout( 0 ) {}

        virtual void paint( QPainter* ) = 0;

        virtual void paintCtx( PaintContext* context );
        virtual void setParentWidget( QWidget* widget );
        virtual void sizeHintChanged()const;

        void setParentLayout( QLayout* layout )
        {
            mParentLayout = layout;
        }
        void removeFromParentLayout()
        {
            if( mParentLayout )
                mParentLayout->removeItem( this );
        }
    protected:
        QWidget* mParent;
        QLayout* mParentLayout;
    };

    /** \internal
     */
    class TextLayoutItem : public AbstractLayoutItem
    {
    public:
        TextLayoutItem( const QString& text,
                        const TextAttributes& attributes,
                        const QObject*  autoReferenceArea,
                        KDChartEnums::MeasureOrientation autoReferenceOrientation,
                        Qt::Alignment alignment = 0 );

        void setText(const QString & text);
        QString text() const;
        TextAttributes attributes() const;

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

        virtual qreal realFontSize() const;
        virtual QFont realFont() const;

        virtual void paint( QPainter* );

    private:
        bool realFontWasRecalculated() const;
        QSize calcSizeHint( QFont fnt ) const;

        QRect mRect;
        QString mText;
        TextAttributes mAttributes;
        const QObject* mAutoReferenceArea;
        KDChartEnums::MeasureOrientation mAutoReferenceOrientation;
        mutable QSize cachedSizeHint;
        mutable qreal cachedFontSize;
        mutable QFont cachedFont;
    };

    /** \internal
     */
    class MarkerLayoutItem : public AbstractLayoutItem
    {
    public:
        MarkerLayoutItem( AbstractDiagram* diagram, const MarkerAttributes& marker,
                          const QBrush& brush, const QPen& pen,
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
        MarkerAttributes mMarker;
        QBrush mBrush;
        QPen mPen;
    };

    /** \internal
     */
    class HorizontalLineLayoutItem : public AbstractLayoutItem
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

    /** \internal
     */
    class VerticalLineLayoutItem : public AbstractLayoutItem
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

}

#endif /* KDCHARTLAYOUTITEMS_H */

