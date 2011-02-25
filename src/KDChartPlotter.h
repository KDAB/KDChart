#ifndef KDCHARTPLOTTER_H
#define KDCHARTPLOTTER_H

#include "KDChartAbstractCartesianDiagram.h"

#include "KDChartLineAttributes.h"
#include "KDChartValueTrackerAttributes.h"

namespace KDChart {

    class ThreeDLineAttributes;

/**
  * @brief Plotter defines a diagram type plotting two-dimensional data.
  */
class KDCHART_EXPORT Plotter : public AbstractCartesianDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( Plotter )

    KDCHART_DECLARE_DERIVED_DIAGRAM( Plotter, CartesianCoordinatePlane )
    Q_PROPERTY( bool useDataCompression READ useDataCompression WRITE setUseDataCompression )
    Q_PROPERTY( qreal mergeRadiusPercentage READ mergeRadiusPercentage WRITE setMergeRadiusPercentage )

public:
    class PlotterType;
    friend class PlotterType;

    explicit Plotter( QWidget* parent = 0, CartesianCoordinatePlane* plane = 0 );
    virtual ~Plotter();

    virtual Plotter* clone() const;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare( const Plotter* other ) const;

    enum PlotType {
        Normal =  0,
        Percent
    };


    void setType( const PlotType type );
    PlotType type() const;

    void setLineAttributes( const LineAttributes & a );
    void setLineAttributes( int column, const LineAttributes & a );
    void setLineAttributes( const QModelIndex & index, const LineAttributes & a );
    void resetLineAttributes( int column );
    void resetLineAttributes( const QModelIndex & index );
    LineAttributes lineAttributes() const;
    LineAttributes lineAttributes( int column ) const;
    LineAttributes lineAttributes( const QModelIndex & index ) const;

    void setThreeDLineAttributes( const ThreeDLineAttributes & a );
    void setThreeDLineAttributes( int column, const ThreeDLineAttributes & a );
    void setThreeDLineAttributes( const QModelIndex & index,
                                  const ThreeDLineAttributes & a );

    //FIXME(khz): big TODO(khz): add a lot of reset...Attributes() methods to all
    // appropriate places, for 2.1 (that is: after we have release 2.0.2)  :-)

    ThreeDLineAttributes threeDLineAttributes() const;
    ThreeDLineAttributes threeDLineAttributes( int column ) const;
    ThreeDLineAttributes threeDLineAttributes( const QModelIndex & index ) const;

    void setValueTrackerAttributes( const QModelIndex & index,
                                    const ValueTrackerAttributes & a );
    ValueTrackerAttributes valueTrackerAttributes( const QModelIndex & index ) const;

    void setModel( QAbstractItemModel *model );

    bool useDataCompression() const;
    void setUseDataCompression( bool value );

    qreal mergeRadiusPercentage() const;
    void setMergeRadiusPercentage( qreal value );

#if QT_VERSION < 0x040400 || defined(Q_COMPILER_MANGLES_RETURN_TYPE)
    // implement AbstractCartesianDiagram
    /* reimpl */
    const int numberOfAbscissaSegments () const;
    /* reimpl */
    const int numberOfOrdinateSegments () const;
#else
    // implement AbstractCartesianDiagram
    /* reimpl */
    int numberOfAbscissaSegments () const;
    /* reimpl */
    int numberOfOrdinateSegments () const;
#endif

protected:
    void paint ( PaintContext* paintContext );

public:
    void resize ( const QSizeF& area );

protected:
    virtual double threeDItemDepth( const QModelIndex & index ) const;
    virtual double threeDItemDepth( int column ) const;
    /** \reimpl */
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const;
    void paintEvent ( QPaintEvent* );
    void resizeEvent ( QResizeEvent* );
protected Q_SLOTS:
    void setDataBoundariesDirty();
    void calcMergeRadius();
}; // End of class KDChart::Plotter

}

#endif // KDCHARTLINEDIAGRAM_H
