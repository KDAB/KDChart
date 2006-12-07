#ifndef KDCHARTTERNARYDIAGRAM_H
#define KDCHARTTERNARYDIAGRAM_H

#include "KDChartTernaryCoordinatePlane.h"
#include <KDChartAbstractDiagram>


namespace KDChart {
   
class TernaryDiagram : public AbstractDiagram
{
   Q_OBJECT
   Q_DISABLE_COPY( TernaryDiagram )
   KDCHART_DECLARE_DERIVED_DIAGRAM( TernaryDiagram, TernaryCoordinatePlane )

   friend class TernaryCoordinatePlane;

public:
   explicit TernaryDiagram ( QWidget* parent = 0, TernaryCoordinatePlane* plane = 0 );
   virtual ~TernaryDiagram();

   virtual void  resize (const QSizeF &area); 
   virtual void  paint (PaintContext *paintContext);

   

protected:
   virtual const QPair< QPointF, QPointF >  calculateDataBoundaries () const; 

};
   
};

#endif
