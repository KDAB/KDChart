/****************************************************************************
** Copyright (C) 2001-2021 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTPIEDIAGRAM_H
#define KDCHARTPIEDIAGRAM_H

#include "KDChartAbstractPieDiagram.h"

namespace KDChart {

    class LabelPaintCache;

/**
  * @brief PieDiagram defines a common pie diagram
  */
class KDCHART_EXPORT PieDiagram : public AbstractPieDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( PieDiagram )
    KDCHART_DECLARE_DERIVED_DIAGRAM( PieDiagram, PolarCoordinatePlane )

public:
    explicit PieDiagram(
        QWidget* parent = nullptr, PolarCoordinatePlane* plane = nullptr );
    ~PieDiagram() override;

protected:
    // Implement AbstractDiagram
    /** \reimpl */
    void paint( PaintContext* paintContext ) override;

public:
    /**
     * Describes which decorations are painted around data labels.
     */
    enum LabelDecoration {
        NoDecoration = 0, ///< No decoration
        FrameDecoration = 1, ///< A rectangular frame is painted around the label text
        LineFromSliceDecoration = 2 ///< A line is drawn from the pie slice to its label
    };
    Q_DECLARE_FLAGS( LabelDecorations, LabelDecoration )
    /// Set the decorations to be painted around data labels according to @p decorations.
    void setLabelDecorations( LabelDecorations decorations );
    /// Return the decorations to be painted around data labels.
    LabelDecorations labelDecorations() const;

    /// If @p enabled is set to true, labels that would overlap will be shuffled to avoid overlap.
    /// \note Collision avoidance may allow labels to be closer than AbstractDiagram with
    ///       allowOverlappingDataValueTexts() == false, so you should usually also call
    ///       setAllowOverlappingDataValueTexts( true ) if you enable this feature.
    void setLabelCollisionAvoidanceEnabled( bool enabled );
    /// Return whether overlapping labels will be moved to until they don't overlap anymore.
    bool isLabelCollisionAvoidanceEnabled() const;

    /** \reimpl */
    void resize ( const QSizeF& area ) override;

    // Implement AbstractPolarDiagram
    /** \reimpl */
    qreal valueTotals () const override;
    /** \reimpl */
    qreal numberOfValuesPerDataset() const override;
    /** \reimpl */
    qreal numberOfGridRings() const override;

    virtual PieDiagram * clone() const;

protected:
    /** \reimpl */
    const QPair<QPointF, QPointF> calculateDataBoundaries() const override;
    void paintEvent( QPaintEvent* ) override;
    void resizeEvent( QResizeEvent* ) override;

private:
    // ### move to private class?
    void placeLabels( PaintContext* paintContext );
    // Solve problems with label overlap by changing label positions inside d->labelPaintCache.
    void shuffleLabels( QRectF* textBoundingRect );
    void paintInternal( PaintContext* paintContext );
    void drawSlice( QPainter* painter, const QRectF& drawPosition, uint slice );
    void drawSliceSurface( QPainter* painter, const QRectF& drawPosition, uint slice );
    void addSliceLabel( LabelPaintCache* lpc, const QRectF& drawPosition, uint slice );
    void draw3DEffect( QPainter* painter, const QRectF& drawPosition, uint slice );
    void draw3dCutSurface( QPainter* painter,
        const QRectF& rect,
        qreal threeDHeight,
        qreal angle );
    void draw3dOuterRim( QPainter* painter,
        const QRectF& rect,
        qreal threeDHeight,
        qreal startAngle,
        qreal endAngle );
    void calcSliceAngles();
    void calcPieSize( const QRectF &contentsRect );
    QRectF twoDPieRect( const QRectF &contentsRect, const ThreeDPieAttributes& threeDAttrs ) const;
    QRectF explodedDrawPosition( const QRectF& drawPosition, uint slice ) const;
    uint findSliceAt( qreal angle, int columnCount );
    uint findLeftSlice( uint slice, int columnCount );
    uint findRightSlice( uint slice, int columnCount );
    QPointF pointOnEllipse( const QRectF& boundingBox, qreal angle );
}; // End of class KDChartPieDiagram

Q_DECLARE_OPERATORS_FOR_FLAGS( PieDiagram::LabelDecorations )

}
#endif // KDCHARTPIEDIAGRAM_H
