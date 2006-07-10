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

#ifndef KDCHARTPIEDIAGRAM_H
#define KDCHARTPIEDIAGRAM_H

#include "KDChartAbstractPieDiagram.h"
class QDomDocumentFragment;

namespace KDChart {

class KDCHART_EXPORT PieDiagram : public AbstractPieDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( PieDiagram )
    KDCHART_DECLARE_DERIVED_DIAGRAM( PieDiagram, PolarCoordinatePlane )

public:
    explicit PieDiagram(
        QWidget* parent = 0, PolarCoordinatePlane* plane = 0 );
    virtual ~PieDiagram();

    // Implement AbstractDiagram
    /** \reimpl */
    virtual void paint ( PaintContext* paintContext );
    /** \reimpl */
    virtual void resize ( const QSizeF& area );

    // Implement AbstractPolarDiagram
    /** \reimpl */
    virtual double valueTotals () const;
    /** \reimpl */
    virtual double numberOfValuesPerDataset() const;
    /** \reimpl */
    virtual double numberOfGridRings() const;

    virtual PieDiagram * clone() const;

    virtual QDomDocumentFragment toXML() const;
protected:
    /** \reimpl */
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const;
    void paintEvent( QPaintEvent* );
    void resizeEvent( QResizeEvent* );

private:
    QPointF pointOnCircle( const QRectF& rect, qreal angle );
}; // End of class KDChartPieDiagram

}
#endif // KDCHARTPIEDIAGRA_H
