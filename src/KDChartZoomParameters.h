#ifndef ZOOMPARAMETERS_H
#define ZOOMPARAMETERS_H

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

namespace KDChart {
    /**
      * ZoomParameters stores the center and the factor of zooming internally
      * \internal
      */
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
