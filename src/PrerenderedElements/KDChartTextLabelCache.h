#ifndef KDCHARTTEXTLABELCACHE_H
#define KDCHARTTEXTLABELCACHE_H

#include <QPixmap>

#include "KDChartEnums.h"

// base class for prerendered elements like labels, pixmaps, markers,
// etc
class PrerenderedElement {
public:
    PrerenderedElement();
    virtual ~PrerenderedElement() {}
    virtual const QPixmap& pixmap() const = 0;
    virtual QPointF referencePointLocation( KDChartEnums::PositionValue ) const = 0;

    void setPosition( const QPointF& position );
    const QPointF& position() const;

    void setReferencePoint( KDChartEnums::PositionValue );
    KDChartEnums::PositionValue referencePoint() const;

private:
    QPointF m_position;
    KDChartEnums::PositionValue m_referencePoint;
};

/** CachedLabel is an internal KDChart class that simplifies creation
    and caching of cached text labels.
    It provides referenze points to anchor the text to other
    elements. Reference points use the positions defined in
    KDChartEnums.

    Usage:
    <pre>
    double angle = M_PI/2.0;
    CachedLabel label;
    label.paint( font, tr("Label"), angle );
    </pre>
*/
class PrerenderedLabel : public PrerenderedElement
{
public:
    PrerenderedLabel();

    /** Create a label with the given text and the given rotation
        angle. */
    // FIXME this may need a brush parameter
    void paint( const QFont& font, const QString& text, double angle );

    // reimpl PrerenderedElement:
    const QPixmap& pixmap() const;
    QPointF referencePointLocation( KDChartEnums::PositionValue position ) const;

private:
    QPixmap m_pixmap;
    QPointF m_referenceBottomLeft;
    QPointF m_textBaseLineVector;
    QPointF m_textAscendVector;
};

#endif
