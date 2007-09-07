#ifndef KDCHARTTERNARYCOORDINATEPLANE_H
#define KDCHARTTERNARYCOORDINATEPLANE_H

#include <KDChartAbstractCoordinatePlane>
#include <KDChartPosition>
#include <KDChartTextAttributes>
#include <QString>


namespace KDChart {

class AbstractAxis;
class AbstractDiagram;
class Chart;
class PaintContext;
class TernaryAxis;

typedef QPair< double, double > DoubleRange;

class TernaryCoordinatePlane : public AbstractCoordinatePlane
{
   Q_OBJECT

   Q_DISABLE_COPY( TernaryCoordinatePlane )
   KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( TernaryCoordinatePlane, Chart* )

public:
   explicit TernaryCoordinatePlane ( Chart* parent = 0 );
   virtual ~TernaryCoordinatePlane();

   void addDiagram ( AbstractDiagram* diagram );

   
   void           addAxis( TernaryAxis* axis );
   TernaryAxis*   axisAt(int index) const;


   // need to implement from AbstractCoordinatePlane but is not used.
   const QPointF translate ( const QPointF& diagramPoint ) const;
   // this version is used.
   const QPointF translate ( const double x, const double y, const double z ) const;

   virtual void paint( QPainter* );

   // axis ranges
   // valid position options are: SOUTH, EAST and WEST
   virtual DoubleRange  range ( Position ) const;
   virtual void setRange( Position, const DoubleRange& );

   // triangle label stuff
   // valid positions are: NORTH, EAST and WEST
   virtual void setLabelAttributes( const TextAttributes& );
   virtual TextAttributes labelAttributes() const; 
   virtual void setLabelText( Position, const QString& );
   virtual QString labelText( Position ) const;


protected:
   virtual QRectF calculateRawDataBoundingRect() const;
   virtual DataDimensionsList getDataDimensionsList() const;
   virtual QRectF drawingArea() const;
   void paintEvent ( QPaintEvent* );
   void layoutDiagrams();
   virtual void drawGrid( PaintContext* paintContext );

protected Q_SLOTS:
   void slotLayoutChanged( AbstractDiagram* );

private:
   // TODO, move class variables to private class
   bool bPaintIsRunning;
   bool bNextPaintPending;

   QList<TernaryAxis*> m_axes;

   TextAttributes m_labelAttributes;
   QString m_northLabel;
   QString m_eastLabel;
   QString m_westLabel;

   DoubleRange m_southRange;
   DoubleRange m_eastRange;
   DoubleRange m_westRange;
};

}

#endif
