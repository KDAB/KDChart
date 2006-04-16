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

#ifndef KDCHARTAREA_H
#define KDCHARTAREA_H

#include <QPointF>
#include <QSizeF>
#include <QRectF>
#include <QWidget>

#include "KDChartGlobal.h"

class QDomDocumentFragment;
class QPainter;
class QString;
namespace KDChart {
    class TextAttributes;
    class BackgroundAttributes;
    class FrameAttributes;
    class PaintContext;


    /**
     * @class AbstractArea KDChartAbstractArea.h
     * @brief An area in the chart with a background, a frame, etc.
     *
     * AbstractArea is the base class for all chart elements that have
     * a set of background attributes and frame attributes, such as
     * legends or axes.
     */
class KDCHART_EXPORT AbstractArea : public QWidget
{
    Q_OBJECT

    Q_DISABLE_COPY( AbstractArea )
    KDCHART_DECLARE_PRIVATE_BASE_POLYMORPHIC_QWIDGET( AbstractArea )


protected:
    AbstractArea( QWidget* parent );
    virtual ~AbstractArea() ;

public:
    virtual QDomDocumentFragment toXML() const;

//    virtual AbstractArea * clone() const = 0;


    enum DockingPointType {
      TopLeft,
      Top,
      TopRight,
      Left,
      Center,
      Right,
      BottomLeft,
      Bottom,
      BottomRight
    };

    void alignToDockingPoint( const QPointF& dockingPoint,
                              DockingPointType dockingPointType,
                              Qt::Alignment alignmentFlags);

    void setDockingPointPadding( int horizontalPadding,
                                 int verticalPadding,
                                 bool fixed );
    void setText( const QString& text );
    QString text() const;

    void setTextAttributes( const TextAttributes &a );
    TextAttributes textAttributes() const;

    void setFrameAttributes( const FrameAttributes &a );
    FrameAttributes frameAttributes() const;

    void setBackgroundAttributes( const BackgroundAttributes &a );
    BackgroundAttributes backgroundAttributes() const;

    static void paintBackground( QPainter* painter,
                                 BackgroundAttributes attributes,
                                 const QRectF& rectangle );

signals:
    void positionChanged( AbstractArea * );


}; // End of class AbstractArea

}
#endif // KDCHARTAREA_H
