#ifndef KDCHARTTERNARYLINEDIAGRAM_P_H
#define KDCHARTTERNARYLINEDIAGRAM_P_H

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

#include "KDChartAbstractTernaryDiagram_p.h"

#include <KDABLibFakes>

namespace KDChart {

/**
 * \internal
 */
    class TernaryLineDiagram::Private : public AbstractTernaryDiagram::Private
    {
        friend class TernaryLineDiagram;
    public:
        Private();
        ~Private() {}

        Private( const Private& rhs )
            : AbstractTernaryDiagram::Private( rhs )
        {
        }

    };

    KDCHART_IMPL_DERIVED_DIAGRAM( TernaryLineDiagram, AbstractTernaryDiagram, TernaryCoordinatePlane )
/*
  inline LineDiagram::LineDiagram( Private * p, TernaryCoordinatePlane* plane )
  : AbstractTernaryDiagram( p, plane ) { init(); }
  inline LineDiagram::Private * LineDiagram::d_func()
  { return static_cast<Private*>( AbstractTernaryDiagram::d_func() ); }
  inline const LineDiagram::Private * LineDiagram::d_func() const
  { return static_cast<const Private*>( AbstractTernaryDiagram::d_func() ); }
*/

}

#endif /* KDCHARTTERNARYLINEDIAGRAM_P_H */

