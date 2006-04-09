#ifndef KDCHARTCARTESIANCOORDINATEPLANE_H
#define KDCHARTCARTESIANCOORDINATEPLANE_H

#include "KDChartAbstractCoordinatePlane.h"

namespace KDChart {

    class PaintContext;
    class AbstractDiagram;

    class KDCHART_EXPORT CartesianCoordinatePlane : public CoordinatePlane
    {
        Q_OBJECT

    public:
        class Private;

        explicit CartesianCoordinatePlane ( QWidget* parent = 0 );
        ~CartesianCoordinatePlane();

        void addDiagram ( AbstractDiagram* diagram );

        void setIsometricScaling ( bool onOff );

        bool doesIsometricScaling();

        const QPointF translate ( const QPointF& diagramPoint ) const;

        virtual double zoomFactorX() const;
        virtual double zoomFactorY() const;

        virtual void setZoomFactorX( double factor );
        virtual void setZoomFactorY( double factor );

        virtual QPointF zoomCenter() const;

        virtual void setZoomCenter( QPointF center );

    protected:
        void paintEvent ( QPaintEvent* );
        void resizeEvent ( QResizeEvent* );

        void layoutDiagrams();

    protected slots:
        void slotLayoutChanged( AbstractDiagram* );

    private:
        void paintGrid( PaintContext* );

        Private *d;
    };

}

#endif
