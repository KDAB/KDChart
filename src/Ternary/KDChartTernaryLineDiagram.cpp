#include <limits>

#include <QPainter>

#include <KDChartPaintContext>

#include "TernaryPoint.h"
#include "TernaryConstants.h"
#include "KDChartTernaryLineDiagram.h"
#include "KDChartTernaryLineDiagram_p.h"

using namespace KDChart;

#define d d_func()

TernaryLineDiagram::Private::Private()
    : AbstractTernaryDiagram::Private()
{
}

TernaryLineDiagram::TernaryLineDiagram ( QWidget* parent,
                                         TernaryCoordinatePlane* plane )
    : AbstractTernaryDiagram( new Private(), parent, plane )
{
    init();
    setDatasetDimension( 3 ); // the third column is implicit
}

TernaryLineDiagram::~TernaryLineDiagram()
{
}

void TernaryLineDiagram::init()
{
    d->reverseMapper.setDiagram( this );
}

void  TernaryLineDiagram::resize (const QSizeF& area)
{
}

void  TernaryLineDiagram::paint (PaintContext *paintContext)
{
    AbstractTernaryDiagram::paint( paintContext );

    // sanity checks:
    if ( model() == 0 ) return;

    QPainter* p = paintContext->painter();

    TernaryCoordinatePlane* plane =
        (TernaryCoordinatePlane*) paintContext->coordinatePlane();

    // temp:
    Q_UNUSED( plane );

    double x, y, z;

    p->setPen( QPen( QColor( "mediumorchid" ), 2 ) );
    p->setBrush( QColor( "mediumpurple" ) );

    d->reverseMapper.clear();

    int columnCount = model()->columnCount( rootIndex() );
    QPointF start;
    for(int column=0; column<columnCount; column+=datasetDimension() )
    {
        int numrows = model()->rowCount( rootIndex() );
        for( int row = 0; row < numrows; row++ )
        {
            // see if there is data otherwise skip
            if( ! model()->data( model()->index( row, column+0 ) ).isNull() )
            {
                // retrieve data
                x = qMax( model()->data( model()->index( row, column+0 ) ).toDouble(),
                          0.0 );
                y = qMax( model()->data( model()->index( row, column+1 ) ).toDouble(),
                          0.0 );
                z = qMax( model()->data( model()->index( row, column+2 ) ).toDouble(),
                          0.0 );

                double total = x + y + z;
                if ( fabs( total ) > 3 * std::numeric_limits<double>::epsilon() ) {
                    TernaryPoint tPunkt( x / total, y / total );
                    QPointF diagramLocation = translate( tPunkt );
                    QPointF widgetLocation = plane->translate( diagramLocation );

                    if ( row > 0 ) {
                        // FIXME: draw points according to selected point style:
                        p->drawLine( start, widgetLocation );
                        d->drawPoint( p, row, column, widgetLocation );
                        // FIXME draw markers:
                        // this paints nothing, since he attributes are set to  invisible - why?
                        paintMarker( p, model()->index( row, column + 0 ), widgetLocation );
                    } else {
                        d->drawPoint( p, row, column, widgetLocation );
                    }
                    start = widgetLocation;
                } else {
                    // ignore and do not paint this point, garbage data
                    qDebug() << "TernaryPointDiagram::paint: data point x/y/z:"
                             << x << "/" << y << "/" << z << "ignored, unusable.";
                }
            }
        }
    }
}

const QPair< QPointF, QPointF >  TernaryLineDiagram::calculateDataBoundaries () const
{
    // this is a constant, because we defined it to be one:
    static QPair<QPointF, QPointF> Boundaries(
        TriangleBottomLeft,
        QPointF( TriangleBottomRight.x(), TriangleHeight ) );
    return Boundaries;
}
