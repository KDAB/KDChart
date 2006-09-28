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

#ifndef KDCHART_TEXT_AREA_H
#define KDCHART_TEXT_AREA_H

#include <QObject>

#include "KDChartGlobal.h"
#include "KDChartAbstractAreaBase.h"

namespace KDChart {

    class TextLayoutItem;

/**
  * @class TextArea KDChartTextArea.h
  * @brief A text area in the chart with a background, a frame, etc.
  *
  * TextArea is the base class for all text containing non-widget chart elements
  * that have a set of background attributes and frame attributes, such as
  * headers or footers.
  */
class KDCHART_EXPORT TextArea : public QObject, public AbstractAreaBase
{
    Q_OBJECT

    Q_DISABLE_COPY( TextArea )
    KDCHART_DECLARE_PRIVATE_DERIVED( TextArea )
    friend class ::KDChart::TextLayoutItem;

public:
    TextArea();
    ~TextArea();

    void setAutoReferenceArea( const QObject* area );
    const QObject* autoReferenceArea() const;

    void setAutoReferenceOrientation( KDChartEnums::MeasureOrientation o );
    KDChartEnums::MeasureOrientation autoReferenceOrientation() const;

    void setText( const QString & text );
    QString text() const;

    void setTextAttributes( const TextAttributes & a );
    TextAttributes textAttributes() const;

//    virtual TextArea * clone() const = 0;
    /**
      * @brief Draws the background and frame, then calls paint().
      *
      * In most cases there is no need to overwrite this method in a derived
      * class, but you would overwrite TextLayoutItem::paint() instead.
      */
    virtual void paintIntoRect( QPainter& painter, const QRect& rect );

    /**
      * Call paintAll, if you want the background and the frame to be drawn
      * before the normal paint() is invoked automatically.
      */
    void paintAll( QPainter& painter );

    virtual qreal realFontSize() const;
    virtual QFont realFont() const;

    virtual void paint( QPainter* );

    virtual QSize sizeHint() const;

    virtual bool intersects( const TextArea & other, const QPointF& myPos, const QPointF& otherPos ) const;
    virtual bool intersects( const TextArea & other, const QPoint& myPos, const QPoint& otherPos ) const;

    virtual QRect areaGeometry() const;
    virtual void setAreaGeometry( const QRect & rect );
protected:
    virtual void positionHasChanged();
    void sizeHintChanged() const;

Q_SIGNALS:
    void positionChanged( TextArea * );

}; // End of class TextArea

}
#endif // KDCHART_TEXT_AREA_H
