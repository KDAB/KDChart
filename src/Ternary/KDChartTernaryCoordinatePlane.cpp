#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartTernaryCoordinatePlane_p.h"

using namespace KDChart;

TernaryCoordinatePlane::TernaryCoordinatePlane( Chart* parent )
    : AbstractCoordinatePlane( parent )
{

}

TernaryCoordinatePlane::~TernaryCoordinatePlane()
{
}

void TernaryCoordinatePlane::addDiagram( AbstractDiagram* diagram )
{
    Q_ASSERT_X ( dynamic_cast<AbstractTernaryDiagram*>( diagram ),
                 "TernaryCoordinatePlane::addDiagram", "Only ternary "
                 "diagrams can be added to a ternary coordinate plane!" );
    AbstractCoordinatePlane::addDiagram ( diagram );
//     connect ( diagram,  SIGNAL ( layoutChanged ( AbstractDiagram* ) ),
//               SLOT ( slotLayoutChanged ( AbstractDiagram* ) ) );

//     connect( diagram, SIGNAL( propertiesChanged() ),this, SIGNAL( propertiesChanged() ) );
}

void TernaryCoordinatePlane::layoutDiagrams()
{
    // all diagrams always take the same space, nothing to be done here
}

const QPointF TernaryCoordinatePlane::translate( const QPointF& diagramPoint ) const
{
    // FIXME temp
    return QPointF();
}

QSize TernaryCoordinatePlane::minimumSizeHint() const
{
    // FIXME temp
    return QSize();
}

QSizePolicy TernaryCoordinatePlane::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::MinimumExpanding,
                        QSizePolicy::MinimumExpanding );
}

void TernaryCoordinatePlane::paint( QPainter* )
{
}

DataDimensionsList TernaryCoordinatePlane::getDataDimensionsList() const
{
    return DataDimensionsList();
}

