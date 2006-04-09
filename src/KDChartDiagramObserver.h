/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2003 Klarävdalens Datakonsult AB.  All rights reserved.
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
#ifndef __KDCHARTDIAGRAMOBSERVER_H__
#define __KDCHARTDIAGRAMOBSERVER_H_

#include <QObject>

namespace KDChart {
    class AbstractDiagram;

    class DiagramObserver : public QObject
    {
        Q_OBJECT
    public:
       /**
         * Constructur.
         * Constructs a new observer observing the given diagram.
         * The observer is made a child of the diagram, so it will
         * be removed automagically.
         */
        DiagramObserver( AbstractDiagram* diagram, QObject* parent = NULL );

    signals:
        /** This signal is emitted immediately before the diagram is 
	 * being destroyed. */
        void diagramDestroyed( AbstractDiagram* diagram );
	/** This signal is emitted whenever the data of the diagram changes. */
        void diagramDataChanged( AbstractDiagram* diagram );

    private slots:
        void destroyed();
        void dataChanged();

    private:
        AbstractDiagram* diagram;
    };
}

#endif // KDChartDiagramObserver_H
