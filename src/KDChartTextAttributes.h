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

#ifndef KDCHARTTEXTATTRIBUTES_H
#define KDCHARTTEXTATTRIBUTES_H

#include <QDebug>
#include <QMetaType>
#include "KDChartGlobal.h"

class QColor;
class QFont;
class QDomDocumentFragment;

namespace KDChart {

class KDCHART_EXPORT TextAttributes
{
public:
  TextAttributes();
  TextAttributes( const TextAttributes& );
  TextAttributes &operator= ( const TextAttributes& );

  ~TextAttributes();

  void setVisible( bool visible );
  bool isVisible() const;

  void setFont( const QFont& font );
  QFont font() const;

  void setUseRelativeSize( bool useRelativeSize );
  bool useRelativeSize() const;

  void setRelativeSize( int relativeSize );
  int relativeSize() const;

  void setMinimalSize( int minimalSize );
  int minimalSize() const;

  void setAutoRotate( bool autoRotate );
  bool autoRotate() const;

  void setAutoShrink( bool autoShrink );
  bool autoShrink() const;

  void setRotation( int rotation );
  int rotation() const;

  void setColor( const QColor& color );
  QColor color() const;

  bool operator==( const TextAttributes& ) const;
  inline bool operator!=( const TextAttributes& other ) const { return !operator==(other); }

  // XML serialization
  QDomDocumentFragment toXML() const;
  // FIXME QPen?
  //
  // FIXME KDChartEnums::TextLayoutPolicy?

private:
    class Private;
    Private * _d;
    Private * d_func() { return _d; }
    const Private * d_func() const { return _d; }

}; // End of class TextAttributes

}

#if !defined(QT_NO_DEBUG_STREAM)
KDCHART_EXPORT QDebug operator<<(QDebug, const KDChart::TextAttributes& );
#endif /* QT_NO_DEBUG_STREAM */

Q_DECLARE_METATYPE( KDChart::TextAttributes )

#endif // KDCHARTTEXTATTRIBUTES_H
