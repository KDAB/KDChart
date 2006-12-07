#ifndef KDCHARTTERNARYCOORDINATEPLANE_H
#define KDCHARTTERNARYCOORDINATEPLANE_H

#include "../KDChartAbstractCoordinatePlane.h"

namespace KDChart {

    class KDCHART_EXPORT TernaryCoordinatePlane
        : public AbstractCoordinatePlane
    {
        Q_OBJECT
        Q_DISABLE_COPY( TernaryCoordinatePlane )
        KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( TernaryCoordinatePlane, Chart* )

    public:
        explicit TernaryCoordinatePlane( Chart* parent = 0 );
        ~TernaryCoordinatePlane();

        void addDiagram( AbstractDiagram* diagram );

        void layoutDiagrams();

        const QPointF translate ( const QPointF& diagramPoint ) const;

        void paint( QPainter* );
        DataDimensionsList getDataDimensionsList() const;

        /** \reimpl */
        QSize minimumSizeHint() const;
        /** \reimpl */
        QSizePolicy sizePolicy() const;
    };

};

#endif
