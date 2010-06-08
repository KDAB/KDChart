#ifndef KDCHARTRINGDIAGRAM_H
#define KDCHARTRINGDIAGRAM_H

#include "KDChartAbstractPieDiagram.h"

namespace KDChart {

/**
  * @brief RingDiagram defines a common ring diagram
  */
class KDCHART_EXPORT RingDiagram : public AbstractPieDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( RingDiagram )
    KDCHART_DECLARE_DERIVED_DIAGRAM( RingDiagram, PolarCoordinatePlane )

public:
    explicit RingDiagram(
        QWidget* parent = 0, PolarCoordinatePlane* plane = 0 );
    virtual ~RingDiagram();

protected:
    // Implement AbstractDiagram
    /** \reimpl */
    virtual void paint ( PaintContext* paintContext );
public:
    /** \reimpl */
    virtual void resize ( const QSizeF& area );

    // Implement AbstractPolarDiagram
    /** \reimpl */
    virtual double valueTotals () const;
    /** \reimpl */
    virtual double numberOfValuesPerDataset() const;
    virtual double numberOfDatasets() const;
    /** \reimpl */
    virtual double numberOfGridRings() const;

    double valueTotals( int dataset ) const;

    virtual RingDiagram * clone() const;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare( const RingDiagram* other ) const;

    void setRelativeThickness( bool relativeThickness );
    bool relativeThickness() const;

    virtual void setExpandWhenExploded( bool expand );
    virtual bool expandWhenExploded() const;

protected:
    /** \reimpl */
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const;
    void paintEvent( QPaintEvent* );
    void resizeEvent( QResizeEvent* );

private:
    //QRectF piePosition( uint dataset, uint pie ) const;
    void drawOnePie( QPainter* painter,
        uint dataset, uint pie,
        qreal granularity );
    void drawPieSurface( QPainter* painter,
        uint dataset, uint pie,
        qreal granularity );
    QPointF pointOnCircle( const QRectF& rect, int dataset, int pie, bool outer, qreal angle, qreal totalGapFactor, qreal totalExplodeFactor );
    //qreal radiusOf( const QRectF& rect, int dataset, int pie, bool outer );
}; // End of class RingDiagram

}

#endif // KDCHARTRINGDIAGRAM_H
