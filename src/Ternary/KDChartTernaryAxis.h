#ifndef KDCHARTTERNARYAXIS_H
#define KDCHARTTERNARYAXIS_H


#include <KDChartAbstractAxis>
#include <KDChartPosition>
#include <KDChartTextAttributes>
#include "KDChartTernaryDiagram.h"

namespace KDChart {

   class TernaryAxis : public AbstractAxis
   {
      Q_OBJECT
         
      Q_DISABLE_COPY( TernaryAxis )
      KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( TernaryAxis, AbstractDiagram* )
      
   public:      
      explicit TernaryAxis ( TernaryDiagram* diagram = 0 );
      ~TernaryAxis();

      virtual void  paintAll( QPainter &);
      virtual void  paint (QPainter *);
      virtual void  paintCtx (PaintContext *); 

      virtual QRect  geometry () const; 
      virtual void  setGeometry (const QRect &rect);

      virtual bool  isEmpty () const;
      virtual QSize  minimumSize () const;  
      virtual QSize  maximumSize () const; 
      virtual QSize  sizeHint () const;  
      virtual Qt::Orientations  expandingDirections () const  ;

      virtual const Position  position () const;  
      virtual void  setPosition (Position p); 

      void setTitleText( const QString& text );
      QString titleText() const;
      void setTitleTextAttributes( const TextAttributes &a );
      TextAttributes titleTextAttributes() const;
      void resetTitleTextAttributes();
      bool hasDefaultTitleTextAttributes() const;



   private:
      // TODO, move class variables to private class
      QRect m_geometry;
      Position m_position;

      QString m_title;
      TextAttributes m_titleAttributes;

   };
};

#endif