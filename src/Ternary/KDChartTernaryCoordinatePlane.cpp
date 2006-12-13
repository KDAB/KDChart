#include <QtDebug>
#include <QPainter>

#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
#include "KDChartTernaryAxis.h"
#include "KDChartAbstractTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartTernaryCoordinatePlane_p.h"

#include "TernaryConstants.h"

using namespace KDChart;

#define d d_func()

TernaryCoordinatePlane::Private::Private()
    : AbstractCoordinatePlane::Private()
{
}

TernaryCoordinatePlane::TernaryCoordinatePlane( Chart* parent )
    : AbstractCoordinatePlane( new Private(), parent )
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
{   // this is our "resize event":
    // all diagrams always take the same space, nothing to be done here
    qDebug() << "TernaryCoordinatePlane::layoutDiagrams: area:" << areaGeometry();

    QPair<QSizeF, QSizeF> margins = grid()->requiredMargins();
    d->diagramRect = areaGeometry();
    d->diagramRectContainer =
        d->diagramRect.adjusted( margins.first.width(),
                                 margins.first.height(),
                                 -margins.second.width(),
                                 -margins.second.height() );

    QPointF zeroZeroPoint = d->diagramRectContainer.bottomLeft();
    double w = d->diagramRectContainer.width();
    double h = d->diagramRectContainer.height();
    double usableWidth;
    double usableHeight;

    if ( TriangleHeight * w > h ) {
        // shorten width:
        usableWidth = h / TriangleHeight;
        usableHeight = h;
        zeroZeroPoint.setX( zeroZeroPoint.x() + ( w - usableWidth ) / 2 );
    } else {
        // reduce height:
        usableWidth = w;
        usableHeight = TriangleHeight * w;
        zeroZeroPoint.setY( zeroZeroPoint.y() - ( h - usableHeight ) / 2 );
    }
    // the rectangle has 1 as it's width, and TriangleHeight as it's
    // height - so this is how we translate that to widget coordinates:
    d->xUnit = usableWidth; // only because we normalize the values to [0..1]
    d->yUnit = -usableHeight / TriangleHeight;

    d->diagramRect.setBottomLeft( zeroZeroPoint );
    d->diagramRect.setTopRight( QPointF( usableWidth, -usableHeight ) + zeroZeroPoint );
}

const QPointF TernaryCoordinatePlane::translate( const QPointF& point ) const
{
    return QPointF( d->diagramRect.bottomLeft().x() + point.x() * d->xUnit,
                    d->diagramRect.bottomLeft().y() + point.y() * d->yUnit );
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

void TernaryCoordinatePlane::paint( QPainter* painter )
{
    qDebug() << "TernaryCoordinatePlane::paint:" << this;
    PainterSaver s( painter );
    // FIXME: this is not a good location for that:
    painter->setRenderHint(QPainter::Antialiasing, true );

    AbstractDiagramList diags = diagrams();
    if ( !diags.isEmpty() )
    {
        PaintContext ctx;
        ctx.setPainter ( painter );
        ctx.setCoordinatePlane ( this );
        const QRectF drawArea( areaGeometry() );
        ctx.setRectangle ( drawArea );

        // enabling clipping so that we're not drawing outside
//         QRect clipRect = drawArea.toRect().adjusted( -1, -1, 1, 1 );
//         QRegion clipRegion( clipRect );
//         painter->setClipRegion( clipRegion );

        // paint the coordinate system rulers:
        Q_ASSERT( d->grid != 0 );
        d->grid->drawGrid( &ctx );

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
            PainterSaver diagramPainterSaver( painter );
            diags[i]->paint ( &ctx );
        }
    }
}

DataDimensionsList TernaryCoordinatePlane::getDataDimensionsList() const
{   // not needed
    return DataDimensionsList();
}

TernaryGrid* TernaryCoordinatePlane::grid() const
{
    TernaryGrid* ternaryGrid = static_cast<TernaryGrid*>( d->grid );
    Q_ASSERT( dynamic_cast<TernaryGrid*>( d->grid ) );
    return ternaryGrid;
}

#undef d
