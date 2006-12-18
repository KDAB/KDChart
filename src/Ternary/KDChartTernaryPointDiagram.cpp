#include <limits>

#include <QPainter>

#include <KDChartPaintContext>

#include "TernaryPoint.h"
#include "TernaryConstants.h"
#include "KDChartTernaryPointDiagram.h"
#include "KDChartTernaryPointDiagram_p.h"

using namespace KDChart;

TernaryPointDiagram::Private::Private()
    : AbstractTernaryDiagram::Private()
{
}

TernaryPointDiagram::TernaryPointDiagram ( QWidget* parent,
                                           TernaryCoordinatePlane* plane )
    : AbstractTernaryDiagram( new Private(), parent, plane )
{
    setDatasetDimension( 2 ); // the third column is implicit
}

TernaryPointDiagram::~TernaryPointDiagram()
{
}

void  TernaryPointDiagram::resize (const QSizeF& area)
{
}

void  TernaryPointDiagram::paint (PaintContext *paintContext)
{
    AbstractTernaryDiagram::paint( paintContext );

    // sanity checks:
    if ( model() == 0 ) return;

    QPainter* p = paintContext->painter();

    TernaryCoordinatePlane* plane =
        (TernaryCoordinatePlane*) paintContext->coordinatePlane();

    double x, y, z;

    p->setPen( QPen( QColor( "steelblue" ), 2 ) );
    p->setBrush( QColor( "slateblue" ) );

    int columnCount = model()->columnCount( rootIndex() );
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

                // fix messed up data values (paint as much as possible)
                double total = x + y + z;
                if ( fabs( total ) > 3 * std::numeric_limits<double>::epsilon() ) {
                    TernaryPoint tPunkt( x / total, y / total );
                    QPointF diagramLocation = translate( tPunkt );
                    QPointF widgetLocation = plane->translate( diagramLocation );
                    // FIXME: draw points according to selected point style:
                    {
                        static const double Diameter = 5.0;
                        static const double Radius = Diameter / 2.0;
                        p->drawEllipse(
                            QRectF( widgetLocation - QPointF( Radius, Radius ),
                                    QSizeF( Diameter, Diameter ) ) );
                    }
                    // FIXME draw markers:
                    // this paints nothing, since he attributes are set to  invisible - why?
                    paintMarker( p, model()->index( row, column + 0 ), widgetLocation );
                } else {
                    // ignore and do not paint this point, garbage data
                    qDebug() << "TernaryPointDiagram::paint: data point x/y/z:"
                             << x << "/" << y << "/" << z << "ignored, unusable.";
                }
            }
        }
    }
}

const QPair< QPointF, QPointF >  TernaryPointDiagram::calculateDataBoundaries () const
{
    // this is a constant, because we defined it to be one:
    static QPair<QPointF, QPointF> Boundaries(
        TriangleBottomLeft,
        QPointF( TriangleBottomRight.x(), TriangleHeight ) );
    return Boundaries;
}

