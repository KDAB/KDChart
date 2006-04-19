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

#ifndef KDCHARTABSTRACTPIEDIAGRAM_H
#define KDCHARTABSTRACTPIEDIAGRAM_H

#include "KDChartAbstractPolarDiagram.h"
class QDomDocumentFragment;

namespace KDChart {

class KDCHART_EXPORT AbstractPieDiagram : public AbstractPolarDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( AbstractPieDiagram )
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( AbstractPieDiagram, PolarCoordinatePlane * )
public:
    AbstractPieDiagram( PolarCoordinatePlane* plane = 0 );
    virtual ~AbstractPieDiagram();

    virtual QDomDocumentFragment toXML() const;

    /** Enable or disable exploding pie pieces depending on
     * the value of @param explode.  */
    void setExplode( bool explode );

    /** @return whether pie pieces should be exploded.  */
    bool explode() const;

    /** Set the explode factor for a dataset.
     * The explode factor is a double between 0 and 1, and is interpreted
     * as a percentage of the total available radius of the pie.  */
    void setExplodeFactor( int dataset, double factor );

    /** @return the explode factor for a dataset. */
    double explodeFactor( int dataset ) const;

    /** Set the explode factor to be used for all slices.
     * The explode factor is a double between 0 and 1, and is interpreted
     * as a percentage of the total available radius of the pie.  */
    void setExplodeFactor( double factor );

    /** @return the explode factor for all datasets. */
    double explodeFactor() const;

    /**  Set the starting angle for the first dataset. */
    void setStartPosition( double degrees );

    /** @return the starting angle for the first dataset. */
    double startPosition() const;

}; // End of class KDChartAbstractPieDiagram

}

#endif // KDCHARTABSTACTPIEDIAGRAM_H
