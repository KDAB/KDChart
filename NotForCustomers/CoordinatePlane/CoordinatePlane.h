#ifndef COORDINATEPLANE_H
#define COORDINATEPLANE_H

#include <KDChartCartesianCoordinatePlane.h>
#include <CartesianCoordinateTransformation.h>

class CoordinatePlane : public KDChart::CartesianCoordinatePlane
{
    Q_OBJECT

public:
    explicit CoordinatePlane( QWidget* parent );

protected:
    void paintEvent ( QPaintEvent* e );

    inline QPointF translate ( const QPointF& diagramPoint, double perspective );
    inline QPointF translate ( const QPointF& diagramPoint );
    inline double translateDistance ( const QPointF&, const QPointF& );
    inline double translateVerticalDistance ( const double& p1, const double& p2 );
    inline double translateHorizontalDistance ( const double& p1, const double& p2 );

protected:
    void layoutDiagrams();

private:
    mutable KDChart::CoordinateTransformation transformation;

    // diagram coordinate system width and height:
    const double widthF();
    const double heightF();
    QPointF m_bottomLeft;
    QPointF m_topRight;

    // FIXME does not belong into coordinate plane, temp only
    void drawBar ( QPainter& painter, int position, double value );
    void drawAbscissaMarker ( QPainter& painter, int position, const QString& text );
};

#endif
