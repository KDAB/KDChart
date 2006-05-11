#ifndef COORDINATEPLANE_H
#define COORDINATEPLANE_H

#include <KDChartCartesianCoordinatePlane.h>
#include <CartesianCoordinateTransformation.h>

class CoordinatePlane : public KDChart::CartesianCoordinatePlane
{
    Q_OBJECT

public:
    explicit CoordinatePlane( QWidget* parent );

    const QPointF translate ( const QPointF& diagramPoint, double perspective ) const;
    const QPointF translate ( const QPointF& diagramPoint ) const;
    double translateDistance ( const QPointF&, const QPointF& ) const;
    double translateVerticalDistance ( const double& p1, const double& p2 ) const;
    double translateHorizontalDistance ( const double& p1, const double& p2 ) const;

protected:
    void paintEvent ( QPaintEvent* e );

protected:
    void layoutDiagrams();

private:
    mutable KDChart::CoordinateTransformation transformation;

    // diagram coordinate system width and height:
    const double widthF();
    const double heightF();
    QPointF m_bottomLeft;
    QPointF m_topRight;

    void drawGrid ( QPainter& painter );
    // FIXME does not belong into coordinate plane, temp only
    void drawBar ( QPainter& painter, int position, double value );

    void drawAbscissaMarker ( QPainter& painter, int position, const QString& text );
};

#endif
