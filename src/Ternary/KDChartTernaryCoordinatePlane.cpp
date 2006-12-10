#include <QtDebug>
#include <QPainter>

#include "KDChartPaintContext.h"
#include "KDChartPainterSaver_p.h"
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
    QRectF rect = areaGeometry();
    qDebug() << "TernaryCoordinatePlane::layoutDiagrams: area:" << rect;

    double w = rect.width();
    double h = rect.height();

    double xMargin = 0.0;
    double yMargin = 0.0;
    // FIXME use the real margin every individual label requires (this
    // results in too wide margins, so far):
//     Q_FOREACH( const PrerenderedLabel& label, d->labels ) {
//         QRectF rect = label.pixmap().rect();
//         xMargin = qMax( xMargin, rect.width() );
//         yMargin = qMax( yMargin, rect.height() );
//     }

    d->diagramRectContainer =
        QRectF( xMargin, yMargin, w - 2.0 * xMargin, h - 2.0 * yMargin );

    QPointF zeroZeroPoint = d->diagramRectContainer.bottomLeft();
    w = d->diagramRectContainer.width();
    h = d->diagramRectContainer.height();
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
    painter->setPen( QPen( Qt::gray, 5 ) );
    painter->setBrush( Qt::lightGray );
    painter->drawRect( areaGeometry() );

    AbstractDiagramList diags = diagrams();
    if ( !diags.isEmpty() )
    {
        PaintContext ctx;
        ctx.setPainter ( painter );
        ctx.setCoordinatePlane ( this );
        const QRectF drawArea( areaGeometry() );
        ctx.setRectangle ( drawArea );

        // enabling clipping so that we're not drawing outside
        PainterSaver painterSaver( painter );
//         QRect clipRect = drawArea.toRect().adjusted( -1, -1, 1, 1 );
//         QRegion clipRegion( clipRect );
//         painter->setClipRegion( clipRegion );

        // paint the coordinate system rulers:
        // d->grid->drawGrid( &ctx );

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
//qDebug("  start diags[i]->paint ( &ctx );");
            PainterSaver diagramPainterSaver( painter );
            diags[i]->paint ( &ctx );
//qDebug("  done: diags[i]->paint ( &ctx );");
        }

        //for debugging:
        //    painter->drawRect( drawArea.adjusted(4,4,-4,-4) );
        //    painter->drawRect( drawArea.adjusted(2,2,-2,-2) );
        //    painter->drawRect( drawArea );
    }

}

DataDimensionsList TernaryCoordinatePlane::getDataDimensionsList() const
{
    return DataDimensionsList();
}

#undef d
