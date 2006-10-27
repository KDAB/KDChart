#include <QFont>
#include <QList>
#include <QtDebug>
#include <QPainter>
#include <QApplication>


#include <KDChartAbstractAxis>
#include <KDChartAbstractDiagram>
#include "KDChartTernaryAxis.h"
#include "KDChartTernaryDiagram.h"
#include "KDChartTernaryCoordinatePlane.h"
#include <KDChartPaintContext>

#include "math.h"

#define M_PI 3.14159265359

using namespace KDChart;


TernaryCoordinatePlane::TernaryCoordinatePlane ( Chart* parent ):
AbstractCoordinatePlane( parent )
{
   bPaintIsRunning = false;
   bNextPaintPending = false;

   m_southRange = DoubleRange( 0.0, 1.0 );
   m_eastRange =  DoubleRange( 0.0, 1.0 );
   m_westRange =  DoubleRange( 0.0, 1.0 );

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
      m_axes.append( axis );
//   }
}


TernaryAxis* TernaryCoordinatePlane::axisAt(int index) const
{
	return  m_axes[index];
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
   if( bPaintIsRunning ){
      bNextPaintPending = true;
      return;
   }
   bNextPaintPending = false;
   bPaintIsRunning = true;

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
      // save the painter
      painter->save();

      // translate to drawing position     
      painter->translate( drawArea.topLeft() );
      
      // paint the grid
      drawGrid( &ctx );

      // restore the painter
      painter->restore();

   }

   // draw the diagram(s) 
   // most likely there is only one diagram for these type of plots
   AbstractDiagramList diags = diagrams();
   {
      // save the painter
      painter->save();

      // translate to drawing position     
      painter->translate( drawArea.topLeft() );
      
      // paint the diagrams:
      for ( int i = 0; i < diags.size(); i++ )
      {
         ((TernaryDiagram*) diags[i])->paint ( &ctx );
      }
      
      // restore the painter
      painter->restore();
   }
   
   // draw the axis
   {
      for( int i=0; i<m_axes.size(); i++ )
      {
         // save the painter
         painter->save();

         // get the axis
         TernaryAxis* axis = m_axes.at(i);

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

         // restore the painter
         painter->restore();
      }
   }
   
   bPaintIsRunning = false;
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
   
   for(int d=0; d<3; d++)
   {
      for(int i=1; i<10; i++)
      {
         double value = (double) i / 10.0;
         dim[0] = (d==0) ? value : (d==1) ? 1.0 - value : 0.0;
         dim[1] = (d==1) ? value : (d==2) ? 1.0 - value : 0.0;
         dim[2] = (d==2) ? value : (d==0) ? 1.0 - value : 0.0;
         
         line[0] = plane->translate( dim[0], dim[1], dim[2] );
         
         dim[0] = (d==0) ? value : (d==1) ? 0.0 : 1.0 - value;
         dim[1] = (d==1) ? value : (d==2) ? 0.0 : 1.0 - value;
         dim[2] = (d==2) ? value : (d==0) ? 0.0 : 1.0 - value;
         
         line[1] = plane->translate( dim[0], dim[1], dim[2] );
         
         p->drawLine( line[0], line[1] );
      }
   }

   // draw the labels
   if( m_labelAttributes.isVisible() )
   {
      p->setFont( m_labelAttributes.font() );
      p->setPen( m_labelAttributes.pen() );

      // top
      p->drawText( QRectF( 0.0, -25.0, paintContext->rectangle().width(), 50.0 ),
                   m_northLabel,
                   QTextOption( Qt::AlignHCenter ) );
      // left
      p->drawText( QRectF( -100.0, paintContext->rectangle().height()+5.0, 100.0 , 50.0 ),
                   m_westLabel,
                   QTextOption( Qt::AlignRight ) );
      // right
      p->drawText( QRectF( paintContext->rectangle().width(), paintContext->rectangle().height()+5.0, 100.0 , 50.0 ),
                   m_eastLabel,
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
   m_labelAttributes = a;
}

TextAttributes TernaryCoordinatePlane::labelAttributes( ) const
{
   return m_labelAttributes;
}

void TernaryCoordinatePlane::setLabelText( Position position, const QString & text )
{
   switch( position.value() ) 
   {
   case KDChartEnums::PositionNorth:
      {
         m_northLabel = text;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         m_westLabel = text;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         m_eastLabel = text;
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
         ret = m_northLabel;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         ret = m_westLabel;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         ret = m_eastLabel;
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
         ret = m_southRange;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         ret = m_westRange;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         ret = m_eastRange;
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
         m_southRange = range;
         break;
      }
   case KDChartEnums::PositionWest:
      {
         m_westRange = range;
         break;
      }
   case KDChartEnums::PositionEast:
      {
         m_eastRange = range;
         break;
      }
   default:
      {
         Q_ASSERT( false );
         break;
      }
   }
}