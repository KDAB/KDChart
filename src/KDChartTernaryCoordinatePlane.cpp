#include <QFont>
#include <QList>
#include <QtDebug>
#include <QPainter>


#include <KDChartAbstractAxis>
#include <KDChartAbstractDiagram>
#include "KDChartTernaryAxis.h"
#include "KDChartTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"
#include "KDChartTernaryCoordinatePlane_p.h"
#include <KDChartPaintContext>
#include "KDChartPainterSaver_p.h"

#include "math.h"

#define d d_func()

using namespace KDChart;

TernaryCoordinatePlane::Private::Private()
    :bPaintIsRunning( false ),
    bNextPaintPending( false ),
    southRange( DoubleRange( 0.0, 1.0 ) ),
    eastRange( DoubleRange( 0.0, 1.0 ) ),
    westRange( DoubleRange( 0.0, 1.0 ) )
{

}

TernaryCoordinatePlane::TernaryCoordinatePlane ( Chart* parent ):
        AbstractCoordinatePlane( new Private(), parent )
{

}

TernaryCoordinatePlane::~TernaryCoordinatePlane()
{
}

void TernaryCoordinatePlane::addDiagram ( AbstractDiagram* diagram )
{
   AbstractCoordinatePlane::addDiagram ( diagram );

   // TODO: what's this for ?
       connect ( diagram,  SIGNAL ( layoutChanged ( AbstractDiagram* ) ),
                 SLOT ( slotLayoutChanged ( AbstractDiagram* ) ) );
   
       connect( diagram, SIGNAL( propertiesChanged() ),this, SIGNAL( propertiesChanged() ) );
}

void TernaryCoordinatePlane::addAxis( TernaryAxis * axis )
{
//   TernaryAxis* tmp = dynamic_cast<TernaryAxis*>( axis );
//   if( tmp )
//   {
//      // TODO check if we don't already have an axis on this position.
      d->axes.append( axis );
//   }
}


TernaryAxis* TernaryCoordinatePlane::axisAt(int index) const
{
	return  d->axes[index];
}

const QPointF TernaryCoordinatePlane::translate ( const QPointF& diagramPoint ) const
{
   QPointF p;
   return p;
}

const QPointF TernaryCoordinatePlane::translate ( const double x, const double y, const double z ) const
{
   double X,Y;
   double onscreenX, onscreenY;
   
   X = (1.0 - x + y) / 2.0;
   Y = z;
   
   onscreenX = X * drawingArea().width();
   onscreenY = drawingArea().height() - (Y * drawingArea().height());
   
   return QPointF( onscreenX, onscreenY);
}

void TernaryCoordinatePlane::paint( QPainter* painter )
{
   // prevent recursive call:
   if( d->bPaintIsRunning ){
      d->bNextPaintPending = true;
      return;
   }
   d->bNextPaintPending = false;
   d->bPaintIsRunning = true;

   // paint the background
   paintBackground( *painter, areaGeometry() );

   // setup a paint context.
   PaintContext ctx;
   ctx.setPainter ( painter );
   ctx.setCoordinatePlane ( this );
   const QRectF drawArea( drawingArea() );
   ctx.setRectangle ( drawArea );

   // draw the grid.
   {
       PainterSaver diagramPainterSaver( painter );
       // translate to drawing position
       painter->translate( drawArea.topLeft() );

       // paint the grid
       drawGrid( &ctx );
   }

    // draw the diagram(s) 
    // most likely there is only one diagram for these type of plots
    AbstractDiagramList diags = diagrams();
    if ( !diags.isEmpty() )
    {
        PainterSaver diagramPainterSaver( painter );

        // translate to drawing position
        painter->translate( drawArea.topLeft() );

        // paint the diagrams:
        for ( int i = 0; i < diags.size(); i++ )
        {
            diags[i]->paint ( &ctx );
        }
    }
   // draw the axis
    for( int i=0; i<d->axes.size(); i++ )
    {
        PainterSaver diagramPainterSaver( painter );

        // get the axis
        TernaryAxis* axis = d->axes.at(i);

        // see where it goes and:
        // - set it's drawing area
        // - translate the painter
        // - rotate the painter if required.
        switch( axis->position().value() )
        {
        case KDChartEnums::PositionSouth:
        {
            painter->translate( drawArea.bottomLeft() );
            painter->translate( QPointF( 0.0, 2.0 ) );

            QRectF axisArea( 0.0, 0.0, drawArea.width(), 100 );
            ctx.setRectangle( axisArea );
            break;
        }
        case KDChartEnums::PositionEast:
        {
            // translate to rotation point
            painter->translate( drawArea.bottomRight() );
            // rotate correct amount of degrees
            double angle = atan( (0.5 * drawArea.width()) / drawArea.height() );
            double height =  drawArea.height() / cos(angle);
            painter->rotate( (-180/M_PI)*angle  );
            // shift up to top of triangle
            painter->translate( QPointF( 2.0, -1*height ) );
    
            QRectF axisArea( 0.0, 0.0, 100.0, height );
            ctx.setRectangle( axisArea );
    
            break;
        }
        case KDChartEnums::PositionWest:
        {
            // translate to rotation point
            painter->translate( drawArea.bottomLeft() );
            // rotate correct amount of degrees
            double angle = atan( (0.5 * drawArea.width()) / drawArea.height() );
            double height =  drawArea.height() / cos(angle);
            painter->rotate( (180/M_PI)*angle  );
            // shift up to top of triangle
            painter->translate( QPointF( -102.0, -1*height ) );

            QRectF axisArea( 0.0, 0.0, 100.0, height );
            ctx.setRectangle( axisArea );

            break;
        }
        default:
        {
            Q_ASSERT( false );
            break;
        }

        }
        // paint it
        axis->paintCtx( &ctx );
    }
    d->bPaintIsRunning = false;
}

void TernaryCoordinatePlane::drawGrid( PaintContext* paintContext )
{
   QPainter* p = paintContext->painter();
   TernaryCoordinatePlane* plane = (TernaryCoordinatePlane*) paintContext->coordinatePlane();
   
   // draw a triangle
   p->setPen( Qt::black );
   QPointF triangle[4] = {
      plane->translate( 1.0, 0.0, 0.0 ),
      plane->translate( 0.0, 1.0, 0.0 ),
      plane->translate( 0.0, 0.0, 1.0 ),
      plane->translate( 1.0, 0.0, 0.0 ),
   };
   p->drawPolyline( triangle, 4 );
   
   // draw a grid
   p->setPen( Qt::lightGray );
   QPointF line[2];
   double dim[3];
   
   for(int dd=0; dd<3; dd++)
   {
      for(int i=1; i<10; i++)
      {
         double value = (double) i / 10.0;
         dim[0] = (dd==0) ? value : (dd==1) ? 1.0 - value : 0.0;
         dim[1] = (dd==1) ? value : (dd==2) ? 1.0 - value : 0.0;
         dim[2] = (dd==2) ? value : (dd==0) ? 1.0 - value : 0.0;
         
         line[0] = plane->translate( dim[0], dim[1], dim[2] );
         
         dim[0] = (dd==0) ? value : (dd==1) ? 0.0 : 1.0 - value;
         dim[1] = (dd==1) ? value : (dd==2) ? 0.0 : 1.0 - value;
         dim[2] = (dd==2) ? value : (dd==0) ? 0.0 : 1.0 - value;
         
         line[1] = plane->translate( dim[0], dim[1], dim[2] );
         
         p->drawLine( line[0], line[1] );
      }
   }

   // draw the labels
   if( d->labelAttributes.isVisible() )
   {
      p->setFont( d->labelAttributes.font() );
      p->setPen( d->labelAttributes.pen() );

      // top
      p->drawText( QRectF( 0.0, -25.0, paintContext->rectangle().width(), 50.0 ),
                   d->northLabel,
                   QTextOption( Qt::AlignHCenter ) );
      // left
      p->drawText( QRectF( -100.0, paintContext->rectangle().height()+5.0, 100.0 , 50.0 ),
                   d->westLabel,
                   QTextOption( Qt::AlignRight ) );
      // right
      p->drawText( QRectF( paintContext->rectangle().width(), paintContext->rectangle().height()+5.0, 100.0 , 50.0 ),
                   d->eastLabel,
                   QTextOption( Qt::AlignLeft ) );

   }
}

QRectF TernaryCoordinatePlane::calculateRawDataBoundingRect() const
{
   QRectF r;
   return r;
}

DataDimensionsList TernaryCoordinatePlane::getDataDimensionsList() const
{
   DataDimensionsList l;
   return l;
}

QRectF TernaryCoordinatePlane::drawingArea() const
{
   const QRect rect( areaGeometry() );
   return QRectF ( rect.left()+80, rect.top()+80, rect.width() - 160, rect.height() - 160 );
}

void TernaryCoordinatePlane::paintEvent ( QPaintEvent* )
{
   
}

void TernaryCoordinatePlane::layoutDiagrams()
{
   
}


void TernaryCoordinatePlane::slotLayoutChanged( AbstractDiagram* )
{
   
}

void TernaryCoordinatePlane::setLabelAttributes( const TextAttributes &a )
{
   d->labelAttributes = a;
}

TextAttributes TernaryCoordinatePlane::labelAttributes( ) const
{
   return d->labelAttributes;
}

void TernaryCoordinatePlane::setLabelText( Position position, const QString & text )
{
   switch( position.value() ) 
   {
   case KDChartEnums::PositionNorth:
      {
         d->northLabel = text;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         d->westLabel = text;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         d->eastLabel = text;
         break;
      }
   default:
      {
         Q_ASSERT( false );
         break;
      }
   }
}

QString TernaryCoordinatePlane::labelText( Position position ) const
{
   QString ret;

   switch( position.value() ) 
   {
   case KDChartEnums::PositionNorth:
      {
         ret = d->northLabel;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         ret = d->westLabel;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         ret = d->eastLabel;
         break;
      }
   default:
      {
         Q_ASSERT( false );
         break;
      }
   }

   return ret;
}

DoubleRange TernaryCoordinatePlane::range ( Position position ) const  
{
   DoubleRange ret;

   switch( position.value() ) 
   {
   case KDChartEnums::PositionSouth:
      {
         ret = d->southRange;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         ret = d->westRange;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         ret = d->eastRange;
         break;
      }
   default:
      {
         Q_ASSERT( false );
         break;
      }
   }

   return ret;
}

void TernaryCoordinatePlane::setRange( Position position, const DoubleRange& range )
{
  switch( position.value() ) 
   {
   case KDChartEnums::PositionSouth:
      {
         d->southRange = range;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         d->westRange = range;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         d->eastRange = range;
         break;
      }
   default:
      {
         Q_ASSERT( false );
         break;
      }
   }
}

#undef d
