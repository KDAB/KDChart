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
#ifndef KDCHARTCARTESIANAXIS_H
#define KDCHARTCARTESIANAXIS_H

#include <QList>

#include "KDChartAbstractAxis.h"

namespace KDChart {

    class AbstractCartesianDiagram;

    class KDCHART_EXPORT CartesianAxis : public AbstractAxis
    {
        Q_OBJECT

        Q_DISABLE_COPY( CartesianAxis )
        KDCHART_DECLARE_PRIVATE_DERIVED_PARENT( CartesianAxis, AbstractCartesianDiagram* )

    public:
        enum Position {
            Bottom,
            Top,
            Right,
            Left
        };

        CartesianAxis ( AbstractCartesianDiagram* parent = 0 );
        ~CartesianAxis();

        void paint( PaintContext* ) const;
	void paintEvent( QPaintEvent* event );
        QSize sizeHint() const;
        QSize minimumSizeHint() const { return sizeHint(); }
	QSizePolicy sizePolicy() const;
        void setGeometry( const QRectF& rect );
        QRectF geometry() const;


        QDomDocumentFragment toXML() const;

        virtual void setPosition ( Position p );
        virtual const Position position () const;
    };

    class CartesianAxisList : public QList<CartesianAxis*> {};
}

#endif
