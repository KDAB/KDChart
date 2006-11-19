#include "KDChartTernaryDiagram.h"
#include <KDChartPaintContext>
#include <QPainter>



using namespace KDChart;

   

TernaryDiagram::TernaryDiagram ( QWidget* parent, TernaryCoordinatePlane* plane ) :
   AbstractDiagram( parent, plane )
{
   setDatasetDimension( 3 );
}

TernaryDiagram::~TernaryDiagram()
{

}

void  TernaryDiagram::resize (const QSizeF &area)
{

}

void  TernaryDiagram::paint (PaintContext *paintContext)
{
   QPainter* p = paintContext->painter();
   TernaryCoordinatePlane* plane = (TernaryCoordinatePlane*) paintContext->coordinatePlane();

   double x,y,z;
   QPolygonF points;

   int columnCount = model()->columnCount( rootIndex() );
   for(int column=0; column<columnCount; column+=datasetDimension() )
   {
      points.clear();
      int numrows = model()->rowCount( rootIndex() );
      for( int row = 0; row < numrows; row++ )
      {
         // see if there is data otherwise skip
         if( ! model()->data( model()->index( row, column+0 ) ).isNull() )
         {
            // retrieve data
            x = model()->data( model()->index( row, column+0 ) ).toDouble();
            y = model()->data( model()->index( row, column+1 ) ).toDouble();
            z = model()->data( model()->index( row, column+2 ) ).toDouble();

            // translate the points to onscreen coordinates.
            // and add to pointlist
            points += plane->translate( x, y, z );
         }
      }

      // draw line from pointlist
      p->setPen( pen( column+2 ) );
      p->setBrush( brush( column+2 ) );
      p->drawPolyline( points );
   
      // paint markers
      for (int i = 0; i < points.size(); ++i) 
      {
         // use dummy index (same markers for all datapoints)
         // good enough for now.
         paintMarker( p, model()->index( 0, column+2 ), points.at(i) );
      }
   }
}

const QPair< QPointF, QPointF >  TernaryDiagram::calculateDataBoundaries () const
{
   QPair< QPointF, QPointF> t;
   return t;
}
   
