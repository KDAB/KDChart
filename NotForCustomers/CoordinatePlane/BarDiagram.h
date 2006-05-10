#ifndef BARDIAGRAM_H
#define BARDIAGRAM_H

#include "KDChartAbstractCartesianDiagram.h"
#include "KDChartBarAttributes.h"
#include "KDChartThreeDBarAttributes.h"

class QDomDocumentFragment;
class QPainter;

namespace KDChart {

    class Bar2Diagram : public AbstractCartesianDiagram
    {
        Q_OBJECT;

        Q_DISABLE_COPY( Bar2Diagram );
        KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( Bar2Diagram, CartesianCoordinatePlane * );

    public:
        explicit Bar2Diagram( CartesianCoordinatePlane* plane = 0 );
        virtual ~Bar2Diagram();

        virtual Bar2Diagram * clone() const;

        enum BarType { Normal,
                       Stacked,
                       Percent,
                       Rows };

        void setType( BarType type );
        BarType type() const;

        void setBarAttributes( const BarAttributes & a );
        void setBarAttributes( int column, const BarAttributes & a );
        void setBarAttributes( const QModelIndex & index,
                               const BarAttributes & a );
        BarAttributes barAttributes( const QModelIndex & index ) const;

        void setThreeDBarAttributes( const ThreeDBarAttributes & a );
        void setThreeDBarAttributes( int column, const ThreeDBarAttributes & a );
        void setThreeDBarAttributes( const QModelIndex & index,
                                     const ThreeDBarAttributes & a );
        ThreeDBarAttributes threeDBarAttributes( const QModelIndex & index ) const;

        // implement AbstractDiagram
        /** \reimpl */
        const QPair<QPointF, QPointF> dataBoundaries() const;
        // implement AbstractCartesianDiagram
        /** \reimpl */
        const int numberOfAbscissaSegments () const;
        /** \reimpl */
        const int numberOfOrdinateSegments () const;

        void paint ( PaintContext* paintContext );
        void resize ( const QSizeF& area );

#if 0
        // FIXME merge with 3DAttributes?
        void setThreeDimensionalBars( bool threeDBars );
        bool threeDimensionalBars() const;

        void setThreeDimensionalBarsShadowColors( bool shadow );
        bool threeDimensionalBarsShadowColors() const;

        void setThreeDimensionalBarAngle( uint angle );
        uint threeDimensionalBarAngle() const;

        void setThreeDimensionalBarDepth( double depth );
        double threeDimensionalBarDepth() const;

#endif


    protected:
        void paintEvent ( QPaintEvent* );

    private:
        void paintBars( PaintContext* ctx, const QModelIndex& index, const QRectF& bar, double& maxDepth );
        void calculateValueAndGapWidths( int rowCount, int colCount,
                                         double groupWidth,
                                         double& barWidth,
                                         double& spaceBetweenBars,
                                         double& spaceBetweenGroups );
    }; // End of class Bar2Diagram

}

#endif
