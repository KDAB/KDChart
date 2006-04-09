/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2005 Klarälvdalens Datakonsult AB.  All rights reserved.
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

#ifndef KDCHARTREALTIVEPOSITION_H
#define KDCHARTREALTIVEPOSITION_H

#include <Qt>
#include "KDChartGlobal.h"
#include "KDChartEnums.h"

class QDomDocumentFragment;
class KDChartMeasure;

class KDCHART_EXPORT KDChartRelativePosition
{
public:
  KDChartRelativePosition();
  KDChartRelativePosition( const KDChartRelativePosition& );
  KDChartRelativePosition &operator= ( const KDChartRelativePosition& );

  ~KDChartRelativePosition();

  QDomDocumentFragment toXML() const;

  void setAlignment( Qt::Alignment flags );
  Qt::Alignment alignment() const;

  void setHorizontalPadding( const KDChartMeasure& padding );
  KDChartMeasure horizontalPadding() const;

  void setVerticalPadding( const KDChartMeasure& padding );
  KDChartMeasure verticalPadding() const;

  void setRotation( int rotation );
  int rotation() const;

    bool operator==( const KDChartRelativePosition& );

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }

}; // End of class KDChartRelativePosition


#endif // KDCHARTREALTIVEPOSITION_H
