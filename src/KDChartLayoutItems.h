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

