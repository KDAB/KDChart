#ifndef KDCHARTABSTRACTTERNARYDIAGRAM_P_H
#define KDCHARTABSTRACTTERNARYDIAGRAM_P_H

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

#include <KDChartAbstractDiagram_p.h>
#include <KDChartAbstractThreeDAttributes.h>
#include <KDChartGridAttributes.h>

#include <KDABLibFakes>


namespace KDChart {

    class CartesianCoordinatePlane;
    class AbstractTernaryDiagram;

/**
 * \internal
 */
    class AbstractTernaryDiagram::Private : public AbstractDiagram::Private
    {
        friend class AbstractTernaryDiagram;
    public:
        Private();
        ~Private();

        Private( const Private& rhs ) :
            AbstractDiagram::Private( rhs ),
        // Do not copy axes and reference diagrams.
        axesList(),
        referenceDiagram( 0 )
        {
        }

        TernaryAxisList axesList;

        AbstractTernaryDiagram* referenceDiagram;
        QPointF referenceDiagramOffset;
    };

    KDCHART_IMPL_DERIVED_DIAGRAM( AbstractTernaryDiagram, AbstractDiagram, CartesianCoordinatePlane )
/*
inline AbstractTernaryDiagram::AbstractTernaryDiagram( Private * p )
    : AbstractDiagram( p ) { init(); }
inline AbstractTernaryDiagram::AbstractTernaryDiagram(
    Private * p, QWidget* parent, CartesianCoordinatePlane* plane )
    : AbstractDiagram( p, parent, plane ) { init(); }
inline AbstractTernaryDiagram::Private * AbstractTernaryDiagram::d_func()
{ return static_cast<Private*>( AbstractDiagram::d_func() ); }
inline const AbstractTernaryDiagram::Private * AbstractTernaryDiagram::d_func() const
{ return static_cast<const Private*>( AbstractDiagram::d_func() ); }
*/

}

#endif /* KDCHARTABSTRACTTERNARYDIAGRAM_P_H */
