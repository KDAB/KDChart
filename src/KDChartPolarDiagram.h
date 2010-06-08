#ifndef KDCHARTPOLARDIAGRAM_H
#define KDCHARTPOLARDIAGRAM_H


#include "KDChartPosition.h"
#include "KDChartAbstractPolarDiagram.h"


class QPolygonF;


namespace KDChart {

/**
  * @brief PolarDiagram defines a common polar diagram
  */
class KDCHART_EXPORT PolarDiagram : public AbstractPolarDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( PolarDiagram )
    KDCHART_DECLARE_DERIVED_DIAGRAM( PolarDiagram, PolarCoordinatePlane )

public:
    explicit PolarDiagram(
        QWidget* parent = 0, PolarCoordinatePlane* plane = 0 );
    virtual ~PolarDiagram();

protected:
    // Implement AbstractDiagram
    /** \reimpl */
    virtual void paint ( PaintContext* paintContext );

public:
    /** \reimpl */
    virtual void resize ( const QSizeF& area );

    // Implement AbstractPolarDiagram
    /** \reimpl */
    virtual double valueTotals () const;
    /** \reimpl */
    virtual double numberOfValuesPerDataset() const;
    /** \reimpl */
    virtual double numberOfGridRings() const;

    virtual PolarDiagram * clone() const;

    /** \deprecated Use PolarCoordinatePlane::setStartPosition( qreal degrees ) instead. */
    void setZeroDegreePosition( int degrees );
    /** \deprecated Use qreal PolarCoordinatePlane::startPosition instead. */
    int zeroDegreePosition() const;

    void setRotateCircularLabels( bool rotateCircularLabels );
    bool rotateCircularLabels() const;

    /** Close each of the data series by connecting the last point to its
     * respective start point
     */
    void setCloseDatasets( bool closeDatasets );
    bool closeDatasets() const;

    void setShowDelimitersAtPosition( Position position,
                                      bool showDelimiters );
    void setShowLabelsAtPosition( Position position,
                                  bool showLabels );

    bool showDelimitersAtPosition( Position position ) const;

    bool showLabelsAtPosition( Position position ) const;

    virtual void paint ( PaintContext* paintContext,
                         bool calculateListAndReturnScale,
                         qreal& newZoomX, qreal& newZoomY );

protected:
    /** \reimpl */
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const;
    void paintEvent ( QPaintEvent* );
    void resizeEvent ( QResizeEvent* );
    virtual void paintPolarMarkers( PaintContext* ctx, const QPolygonF& polygon );

}; // End of class PolarDiagram

}


#endif // KDCHARTPOLARDIAGRAM_H
