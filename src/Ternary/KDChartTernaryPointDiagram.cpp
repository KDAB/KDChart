#include <QPainter>

#include <KDChartPaintContext>

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
    qDebug() << "TernaryPointDiagram::paint:" << this;

    AbstractTernaryDiagram::paint( paintContext );

    // sanity checks:
    if ( model() == 0 ) return;

    QPainter* p = paintContext->painter();
    TernaryCoordinatePlane* plane =
        (TernaryCoordinatePlane*) paintContext->coordinatePlane();

    double x, y, z;

    int columnCount = model()->columnCount( rootIndex() );
    for(int column=0; column<columnCount; column+=datasetDimension() )
    {
        int numrows = model()->rowCount( rootIndex() );
        for( int row = 0; row < numrows; row++ )
        {
            // see if there is data otherwise skip
            if( ! model()->data( model()->index( row, column+0 ) ).isNull() )
            {
                // retrieve datasetDimension
                x = model()->data( model()->index( row, column+0 ) ).toDouble();
                y = model()->data( model()->index( row, column+1 ) ).toDouble();
                z = model()->data( model()->index( row, column+2 ) ).toDouble();
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

