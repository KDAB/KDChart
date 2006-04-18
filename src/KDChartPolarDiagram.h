/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KD Chart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KD Chart licenses may use this file in
 ** accordance with the KD Chart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.kdab.net/kdchart for
 **   information about KD Chart Commercial License Agreements.
 **
 ** Contact info@kdab.net if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTPOLARDIAGRAM_H
#define KDCHARTPOLARDIAGRAM_H

#include "KDChartAbstractArea.h"
#include "KDChartAbstractPolarDiagram.h"
class QDomDocumentFragment;
class QPolygonF;

namespace KDChart {

class KDCHART_EXPORT PolarDiagram : public AbstractPolarDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( PolarDiagram )
    KDCHART_DECLARE_PRIVATE_DERIVED( PolarDiagram )
protected:
    explicit inline PolarDiagram( Private *p, PolarCoordinatePlane *parent );
public:
    PolarDiagram( PolarCoordinatePlane* plane = 0 );
    virtual ~PolarDiagram();

    // Implement AbstractDiagram
    /** \reimpl */
    virtual void paint ( PaintContext* paintContext );
    /** \reimpl */
    virtual void resize ( const QSizeF& area );
    /** \reimpl */
    virtual const QPair<QPointF, QPointF> dataBoundaries() const;

    // Implement AbstractPolarDiagram
    /** \reimpl */
    virtual double valueTotals () const;
    /** \reimpl */
    virtual double numberOfValuesPerDataset() const;
    /** \reimpl */
    virtual double numberOfGridRings() const;

    virtual PolarDiagram * clone() const;

    virtual QDomDocumentFragment toXML() const;

    void setZeroDegreePosition( int degrees );
    int zeroDegreePosition() const;

    void setRotateCircularLabels( bool rotateCircularLabels );
    bool rotateCircularLabels() const;

    void setShowDelimitersAtPosition( AbstractArea::DockingPointType position,
                                      bool showDelimiters );
    void setShowLabelsAtPosition( AbstractArea::DockingPointType position,
                                  bool showLabels );

    bool showDelimitersAtPosition( AbstractArea::DockingPointType position ) const;

    bool showLabelsAtPosition( AbstractArea::DockingPointType position ) const;

protected:
    void paintEvent ( QPaintEvent* );
    void resizeEvent ( QResizeEvent* );
    virtual void paintPolarMarkers( PaintContext* ctx, const QPolygonF& polygon );

}; // End of class PolarDiagram

}


#endif // KDCHARTPOLARDIAGRAM_H
