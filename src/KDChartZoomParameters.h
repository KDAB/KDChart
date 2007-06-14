#ifndef ZOOMPARAMETERS_H
#define ZOOMPARAMETERS_H

namespace KDChart {
    class ZoomParameters {
    public:
        ZoomParameters()
        : xFactor( 1.0 ),
          yFactor( 1.0 ),
          xCenter( 0.5 ),
          yCenter( 0.5)
        {
        }

        ZoomParameters( double xFactor, double yFactor, const QPointF& center )
        : xFactor( xFactor ),
          yFactor( yFactor ),
          xCenter( center.x() ),
          yCenter( center.y() )
        {
        }

        void setCenter( const QPointF& center )
        {
            xCenter = center.x();
            yCenter = center.y();
        }
        const QPointF center() const
        {
            return QPointF( xCenter, yCenter );
        }

        double xFactor;
        double yFactor;

        double xCenter;
        double yCenter;
    };
}

#endif
