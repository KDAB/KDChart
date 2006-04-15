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

#ifndef KDCHARTAXIS_H
#define KDCHARTAXIS_H

#include <QObject>
#include <QRectF>
#include <QtXml/QDomDocumentFragment>
#include <QWidget>

#include "kdchart_export.h"
#include "KDChartGlobal.h"
#include "KDChartAbstractArea.h"

class QPainter;
class QSizeF;
class QRectF;


namespace KDChart {

    class TextAttributes;
    class Area;
    class AbstractCoordinatePlane;
    class PaintContext;
    class AbstractDiagram;

    class KDCHART_EXPORT AbstractAxis : public AbstractArea
    // : public AbstractArea FIXME maybe reintroduce later
    {
        Q_OBJECT

        Q_DISABLE_COPY( AbstractAxis )
        KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( AbstractAxis, AbstractDiagram* )
    public:
        explicit AbstractAxis( AbstractDiagram* parent = 0 );
        virtual ~AbstractAxis();

        // FIXME implement when code os ready for it:
        // virtual Area* clone() const = 0;

        // FIXME (Mirko) readd when needed
        // void copyRelevantDetailsFrom( const KDChartAxis* axis );

    /*    virtual void paint( PaintContext* ) const = 0;
        virtual QSize sizeHint() const = 0;*/
	//virtual void paintEvent( QPaintEvent* event) = 0;
        virtual void setGeometry( const QRectF& rect ) = 0;
        virtual QRectF geometry() const = 0;

        virtual QDomDocumentFragment toXML() const = 0;
    };
}

#endif // KDCHARTAXIS_H
