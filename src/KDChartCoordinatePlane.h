#ifndef KDCHARTCOORDINATEPLANE_H
#define KDCHARTCOORDINATEPLANE_H

#include <QWidget>

#include "KDChartAbstractDiagram.h"

namespace KDChart {

    class KDCHART_EXPORT CoordinatePlane : public QWidget
    {
        Q_OBJECT

    public:
        class Private;

        explicit CoordinatePlane ( QWidget* parent = 0 );
        virtual ~CoordinatePlane();

        virtual void addDiagram ( AbstractDiagram* diagram );
        virtual void replaceDiagram ( AbstractDiagram* diagram, int position = 0 );
        virtual void removeDiagram( int position = 0 );

        AbstractDiagram* diagram();
        AbstractDiagramList diagrams();

        virtual void layoutDiagrams() = 0;

        virtual const QPointF translate ( const QPointF& diagramPoint ) const = 0;

        virtual QSize sizeHint() const;
        virtual QSize minimumSizeHint() const;
        virtual QSizePolicy sizePolicy() const;

        virtual double zoomFactorX() const { return 1.0; }
        virtual double zoomFactorY() const { return 1.0; }

        virtual void setZoomFactorX( double /* factor */ ) {}
        virtual void setZoomFactorY( double /* factor */ ) {}

        virtual QPointF zoomCenter() const { return QPointF(0.0, 0.0); }

        virtual void setZoomCenter( QPointF /* center */ ) {}

    private:
        Private* d;
    };

}

#endif
