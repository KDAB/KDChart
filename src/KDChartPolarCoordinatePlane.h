#ifndef KDCHART_POLAR_COORDINATEPLANE_H
#define KDCHART_POLAR_COORDINATEPLANE_H

#include "KDChartAbstractCoordinatePlane.h"

namespace KDChart {

    class PaintContext;

    class KDCHART_EXPORT PolarCoordinatePlane : public AbstractCoordinatePlane
    {
        Q_OBJECT

    public:
        class Private;
        struct CoordinateTransformation;
        class CoordinateTransformationList;

        explicit PolarCoordinatePlane ( QWidget* parent = 0 );
        ~PolarCoordinatePlane();

        void addDiagram ( AbstractDiagram* diagram );

        const QPointF translate ( const QPointF& diagramPoint ) const;
        const QPointF translatePolar ( const QPointF& diagramPoint ) const;

    protected:
        void paintEvent ( QPaintEvent* );
        void resizeEvent ( QResizeEvent* );

        void layoutDiagrams();
    protected slots:
        void slotLayoutChanged( AbstractDiagram* diagram );

    private:
        void paintRulers ( PaintContext* );

        Private *p;
    };

}

#endif
