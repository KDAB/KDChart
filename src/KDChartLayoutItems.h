#ifndef KDCHARTLAYOUTITEMS_H
#define KDCHARTLAYOUTITEMS_H

#include <QLayoutItem>
#include <QFont>
#include <QBrush>
#include <QFontMetrics>
class QPainter;
class KDTextDocument;

namespace KDChart {

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
        TextLayoutItem( const QString& text, const QFont& font, Qt::Alignment alignment = 0 );

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
        QFontMetrics mMetrics;
        mutable QSize cachedSizeHint;
    };

    /** \internal
     */
    class MarkerLayoutItem : public LayoutItem
    {
    public:
        MarkerLayoutItem( int marker, const QBrush& brush, Qt::Alignment alignment = 0 );

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
        int mMarker;
        QBrush mBrush;
    };

}

#endif /* KDCHARTLAYOUTITEMS_H */

