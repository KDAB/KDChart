/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005-2007 Klaralvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTLEVEYJENNINGSDIAGRAM_H
#define KDCHARTLEVEYJENNINGSDIAGRAM_H

#include "../KDChartLineDiagram.h"
#include "KDChartLeveyJenningsCoordinatePlane.h"

class QPainter;
class QPolygonF;

namespace KDChart {

    class ThreeDLineAttributes;

/**
 * @brief LineDiagram defines a common line diagram.
 * 
 * It provides different subtypes which are set using \a setType.
 */
class KDCHART_EXPORT LeveyJenningsDiagram : public LineDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( LeveyJenningsDiagram )
//    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( LineDiagram, CartesianCoordinatePlane * )

    KDCHART_DECLARE_DERIVED_DIAGRAM( LeveyJenningsDiagram, LeveyJenningsCoordinatePlane )


public:
    LeveyJenningsDiagram( QWidget* parent = 0, LeveyJenningsCoordinatePlane* plane = 0 );
    virtual ~LeveyJenningsDiagram();

    virtual LeveyJenningsDiagram * clone() const;

    /**
     * Returns true if both diagrams have the same settings.
     */
    bool compare( const LeveyJenningsDiagram* other ) const;

    void setLotChangedSymbolPosition( Qt::Alignment pos );
    Qt::Alignment lotChangedSymbolPosition() const;

    void setExpectedMeanValue( float meanValue );
    float expectedMeanValue() const;

    void setExpectedStandardDeviation( float sd );
    float expectedStandardDeviation() const; 

    float calculatedMeanValue() const;
    float calculatedStandardDeviation() const;

    /* \reimpl */
    void setModel( QAbstractItemModel* model );

protected:
    void paint( PaintContext* paintContext );

    virtual void drawDataPointSymbol( PaintContext* paintContext, const QPointF& pos, bool ok );
    virtual void drawLotChangeSymbol( PaintContext* paintContext, const QPointF& pos );

    /** \reimpl */
    virtual const QPair<QPointF, QPointF> calculateDataBoundaries() const;

protected Q_SLOTS:
    void calculateMeanAndStandardDeviation() const;
}; // End of class KDChartLineDiagram

}

#endif // KDCHARTLINEDIAGRAM_H