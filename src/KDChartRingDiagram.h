/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005 KlarÃ¤lvdalens Datakonsult AB.  All rights reserved.
 **
 ** This file is part of the KDChart library.
 **
 ** This file may be distributed and/or modified under the terms of the
 ** GNU General Public License version 2 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.
 **
 ** Licensees holding valid commercial KDChart licenses may use this file in
 ** accordance with the KDChart Commercial License Agreement provided with
 ** the Software.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** See http://www.klaralvdalens-datakonsult.se/?page=products for
 **   information about KDChart Commercial License Agreements.
 **
 ** Contact info@klaralvdalens-datakonsult.se if any conditions of this
 ** licensing are not clear to you.
 **
 **********************************************************************/

#ifndef KDCHARTRINGDIAGRAM_H
#define KDCHARTRINGDIAGRAM_H

#include <KDChartAbstractPieDiagram.h>
class QDomDocumentFragment;

namespace KDChart {

class KDCHART_EXPORT RingDiagram : public AbstractPieDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( RingDiagram )
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( RingDiagram, PolarCoordinatePlane * )
public:
    RingDiagram( PolarCoordinatePlane* plane = 0 );
    virtual ~RingDiagram();

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

    virtual RingDiagram * clone() const;

    virtual QDomDocumentFragment toXML() const;

    void setRelativeThickness( bool relativeThickness );
    bool relativeThickness() const;
protected:
    void paintEvent( QPaintEvent* );
    void resizeEvent( QResizeEvent* );

}; // End of class RingDiagram

}

#endif // KDCHARTRINGDIAGRAM_H
