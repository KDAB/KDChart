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

#ifndef KDCHARTDATAVALUEATTRIBUTES_H
#define KDCHARTDATAVALUEATTRIBUTES_H

#include <Qt>
#include <QMetaType>
#include "KDChartGlobal.h"
#include "KDChartEnums.h"

class KDChartRelativePosition;
class QDomDocumentFragment;

namespace KDChart {
  class TextAttributes;
  class BackgroundAttributes;
  class FrameAttributes;
  class MarkerAttributes;

class KDCHART_EXPORT DataValueAttributes
{
public:
  DataValueAttributes();
  DataValueAttributes( const DataValueAttributes& );
  DataValueAttributes &operator= ( const DataValueAttributes& );

  ~DataValueAttributes();

  void setVisible( bool visible );
  bool isVisible() const;

  QDomDocumentFragment toXML() const;

  void setTextAttributes( const TextAttributes &a );
  TextAttributes textAttributes() const;

  void setFrameAttributes( const FrameAttributes &a );
  FrameAttributes frameAttributes() const;

  void setBackgroundAttributes( const BackgroundAttributes &a );
  BackgroundAttributes backgroundAttributes() const;

  void setMarkerAttributes( const MarkerAttributes &a );
  MarkerAttributes markerAttributes() const;

  void setDecimalDigits( int digits );
  int decimalDigits() const;

  void setPowerOfTenDivisor( int powerOfTenDivisor );
  int powerOfTenDivisor() const;

  void setShowInfinite( bool infinite );
  bool showInfinite() const;

  void setNegativeAnchorPosition( KDChartEnums::PositionFlag anchorPosition );
  KDChartEnums::PositionFlag negativeAnchorPosition() const;

  void setNegativeRelativePosition( const KDChartRelativePosition& relPosition );
  KDChartRelativePosition negativeRelativePosition() const;

  void setPositiveAnchorPosition( KDChartEnums::PositionFlag anchorPosition );
  KDChartEnums::PositionFlag positiveAnchorPosition() const;

  void setPositiveRelativePosition( const KDChartRelativePosition& relPosition );
  KDChartRelativePosition positiveRelativePosition() const;

  static const DataValueAttributes& defaultAttributes();

  bool operator==( const DataValueAttributes& ) const;
  inline bool operator!=( const DataValueAttributes& other ) const { return !operator==(other); }

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }

}; // End of class DataValueAttributes

}

Q_DECLARE_METATYPE( KDChart::DataValueAttributes )

#endif // KDCHARTDATAVALUEATTRIBUTES_H
