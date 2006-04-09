#ifndef CARTESIANCOORDINATETRANSFORMATION_H
#define CARTESIANCOORDINATETRANSFORMATION_H

#include <QList>
#include <QRectF>
#include <QPointF>

namespace KDChart {

    class ZoomParameters {
    public:
        ZoomParameters()
        {
            xFactor = 1;
	    yFactor = 1;
	    xCenter = 0.5;
            yCenter = 0.5;
        }

//    private:
        double xFactor;
        double yFactor;

        double xCenter;
	double yCenter;
    };
	
    // FIXME: if this struct is used more often, we need to make it a class
    // with proper accessor methods:
    struct CoordinateTransformation {
        QRectF diagramRect;
        // represents the distance of the diagram coordinate origin to the
        // origin of the coordinate plane space:
        QPointF originTranslation;
        // make a vector base for R2:
        double unitVectorX;
        double unitVectorY;
        // implement isometric scaling:
        double isoScaleX;
        double isoScaleY;

        ZoomParameters zoom;
	
        inline const QPointF translate( const QPointF& diagramPoint ) //const
        {
            QPointF result = originTranslation;

	    QPointF tempPoint = diagramPoint;
	    tempPoint.setX( tempPoint.x() + diagramRect.width() / (2*zoom.xFactor) );
	    tempPoint.setY( tempPoint.y() + diagramRect.height() / (2*zoom.yFactor ) );

	    tempPoint.setX( tempPoint.x() - diagramRect.width() * zoom.xCenter );
	    tempPoint.setY( tempPoint.y() - diagramRect.height() * zoom.yCenter );

	    result.setX( result.x() + isoScaleX * unitVectorX * tempPoint.x() );
	    result.setY( result.y() + isoScaleY * unitVectorY * tempPoint.y() );
	    
	    result.setX( result.x() * zoom.xFactor );
	    result.setY( result.y() * zoom.yFactor );
	    return result;
        }
    };

    class CoordinateTransformationList : public QList<CoordinateTransformation> {};

}

#endif
