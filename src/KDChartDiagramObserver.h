/* -*- Mode: C++ -*-
   KDChart - a multi-platform charting engine
   */

/****************************************************************************
 ** Copyright (C) 2001-2006 Klarälvdalens Datakonsult AB.  All rights reserved.
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
#ifndef __KDCHARTDIAGRAMOBSERVER_H__
#define __KDCHARTDIAGRAMOBSERVER_H_

#include <QObject>
#include <QAbstractItemModel>
#include <QPointer>

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
	/** This signal is emitted whenever the attributes of the diagram change. */
        void diagramAttributesChanged( AbstractDiagram* diagram );

    private slots:
        void destroyed();
        void dataChanged();
        void attributesChanged();
	void modelsChanged();

    private:
	void init();

        AbstractDiagram* m_diagram;
	QPointer<QAbstractItemModel> m_model;
	QPointer<QAbstractItemModel> m_attributesmodel;	
   };
}

#endif // KDChartDiagramObserver_H
