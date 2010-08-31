#ifndef KDCHARTCARTESIANAXIS_P_H
#define KDCHARTCARTESIANAXIS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KD Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "KDChartCartesianAxis.h"
#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartAbstractAxis_p.h"

#include <KDABLibFakes>


namespace KDChart {

/**
  * \internal
  */
class CartesianAxis::Private : public AbstractAxis::Private
{
    friend class CartesianAxis;

public:
    Private( AbstractCartesianDiagram* diagram, CartesianAxis* axis )
        : AbstractAxis::Private( diagram, axis )
        , useDefaultTextAttributes( true )
        , cachedHeaderLabels( QStringList() )
        , cachedLabelHeight( 0.0 )
        , cachedFontHeight( 0 )
        , axisTitleSpace( 1.0 )
    {}
    ~Private() {}

    const CartesianAxis* axis() const { return static_cast<CartesianAxis *>( mAxis ); }

    void drawSubUnitRulers( QPainter*, CartesianCoordinatePlane* plane, const DataDimension& dim,
                            const QPointF& rulerRef, const QVector<int>& drawnTicks, const bool diagramIsVertical,
                            const RulerAttributes& rulerAttr) const;
    void drawTitleText( QPainter*, CartesianCoordinatePlane* plane, const QRect& areaGeoRect ) const;

    const TextAttributes titleTextAttributesWithAdjustedRotation() const;

    QSize calculateMaximumSize() const;

private:
    QString titleText;
    TextAttributes titleTextAttributes;
    bool useDefaultTextAttributes;
    Position position;
    QRect geometry;
    int customTickLength;
    QMap< double, QString > annotations;
    QList< double > customTicksPositions;
    mutable QStringList cachedHeaderLabels;
    mutable qreal cachedLabelHeight;
    mutable qreal cachedLabelWidth;
    mutable int cachedFontHeight;
    mutable int cachedFontWidth;
    mutable QSize cachedMaximumSize;
    qreal axisTitleSpace;
};

inline CartesianAxis::CartesianAxis( Private * p, AbstractDiagram* diagram )
    : AbstractAxis( p, diagram )
{
    init();
}
inline CartesianAxis::Private * CartesianAxis::d_func()
{ return static_cast<Private*>( AbstractAxis::d_func() ); }
inline const CartesianAxis::Private * CartesianAxis::d_func() const
{ return static_cast<const Private*>( AbstractAxis::d_func() ); }

}

#endif
