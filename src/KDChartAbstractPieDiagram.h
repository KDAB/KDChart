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

#ifndef KDCHARTABSTRACTPIEDIAGRAM_H
#define KDCHARTABSTRACTPIEDIAGRAM_H

#include "KDChartAbstractPolarDiagram.h"
class QDomDocumentFragment;

namespace KDChart {

class ThreeDAttributes;

class KDCHART_EXPORT AbstractPieDiagram : public AbstractPolarDiagram
{
    Q_OBJECT

    Q_DISABLE_COPY( AbstractPieDiagram )
    KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( AbstractPieDiagram, PolarCoordinatePlane * )
public:
    AbstractPieDiagram( PolarCoordinatePlane* plane = 0 );
    virtual ~AbstractPieDiagram();

    virtual QDomDocumentFragment toXML() const;

    void setExplode( bool explode );
    bool explode() const;

    void setExplodeValues( const QList<int>& explodeList );

    // Unfortunately this is not avaialble from QSA-
    QList<int> explodeValues() const;

    typedef QMap<int,double> ExplodeFactorsMap;

    void setExplodeFactors( ExplodeFactorsMap factors );
    ExplodeFactorsMap explodeFactors() const;

    void setExplodeFactor( double factor );
    double explodeFactor() const;

    void setStartPosition( double degrees );
    double startPosition() const;

}; // End of class KDChartAbstractPieDiagram

}

#endif // KDCHARTABSTACTPIEDIAGRAM_H
