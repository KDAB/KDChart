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

#include <QLayoutItem>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QFontMetricsF>
#include "KDChartMarkerAttributes.h"

class QPainter;
class KDTextDocument;

namespace KDChart {
    class AbstractDiagram;

    /** \internal
     */
    class LayoutItem : public QLayoutItem
    {
    public:
        LayoutItem( Qt::Alignment alignment = 0 ) :
            QLayoutItem( alignment ) {}
        virtual void paint( QPainter* ) = 0;
    };

    /** \internal
     */
    class TextLayoutItem : public LayoutItem
    {
    public:
        TextLayoutItem( const QString& text, const QFont& font, const QColor& color,
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
        QSize calcSizeHint() const;

        QRect mRect;
        QString mText;
        QFont mFont;
        QFontMetricsF mMetrics;
        QColor mColor;
        mutable QSize cachedSizeHint;
    };

    /** \internal
     */
    class MarkerLayoutItem : public LayoutItem
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

}

#endif /* KDCHARTLAYOUTITEMS_H */

