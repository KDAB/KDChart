#ifndef COORDINATEPLANE_H
#define COORDINATEPLANE_H

#include <KDChartCartesianCoordinatePlane.h>
#include <CartesianCoordinateTransformation.h>

class CoordinatePlane : public KDChart::CartesianCoordinatePlane
{
    Q_OBJECT

public:
    CoordinatePlane();

protected:
    void paintEvent ( QPaintEvent* e );
    void resizeEvent ( QResizeEvent* e );

    QPointF translate ( const QPointF& diagramPoint, double perspective );
    inline QPointF translate ( const QPointF& diagramPoint );

private:
    mutable KDChart::CoordinateTransformation transformation;

    // diagram coordinate system width and height:
    const double widthF();
    const double heightF();
    const QPointF m_bottomLeft;
    const QPointF m_topRight;

    void drawBar ( QPainter& painter, int position, double value );
};

#endif
