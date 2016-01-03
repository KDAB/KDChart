/****************************************************************************
** Copyright (C) 2001-2016 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

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
    Q_ENUMS( CompressionMode )

    KDCHART_DECLARE_DERIVED_DIAGRAM( Plotter, CartesianCoordinatePlane )
    Q_PROPERTY( CompressionMode useDataCompression READ useDataCompression WRITE setUseDataCompression )
    Q_PROPERTY( qreal mergeRadiusPercentage READ mergeRadiusPercentage WRITE setMergeRadiusPercentage )

public:
    // SLOPE enables a compression based on minmal slope changes
    // DISTANCE is still buggy and can fail, same for BOTH, NONE is the default mode
    enum CompressionMode{ SLOPE, DISTANCE, BOTH, NONE };
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

    ThreeDLineAttributes threeDLineAttributes() const;
    ThreeDLineAttributes threeDLineAttributes( int column ) const;
    ThreeDLineAttributes threeDLineAttributes( const QModelIndex & index ) const;

    void setValueTrackerAttributes( const QModelIndex & index,
                                    const ValueTrackerAttributes & a );
    ValueTrackerAttributes valueTrackerAttributes( const QModelIndex & index ) const;

    CompressionMode useDataCompression() const;
    void setUseDataCompression( CompressionMode value );

    qreal maxSlopeChange() const;
    void setMaxSlopeChange( qreal value );

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

protected Q_SLOTS:
    void connectAttributesModel( AttributesModel* );

protected:
    void paint ( PaintContext* paintContext );

public:
    void resize ( const QSizeF& area );

protected:
    virtual qreal threeDItemDepth( const QModelIndex & index ) const;
    virtual qreal threeDItemDepth( int column ) const;
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
