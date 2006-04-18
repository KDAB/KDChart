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

#include <KDChartAbstractDiagram.h>
#include <KDChartDiagramObserver.h>

#include <QDebug>

namespace KDChart {
    DiagramObserver::DiagramObserver( AbstractDiagram* diagram, QObject* parent )
    : QObject( parent ), diagram(diagram)
    {
       connect( diagram, SIGNAL(destroyed( QObject* )), SLOT(destroyed()));
       connect( diagram->model(), SIGNAL(dataChanged( const QModelIndex&, const QModelIndex&)), SLOT(dataChanged()));
       connect( diagram->model(), SIGNAL(attributesChanged( const QModelIndex&, const QModelIndex&)), SLOT(attributesChanged()));
       connect( diagram->model(), SIGNAL(headerDataChanged( Qt::Orientation, int, int)), SLOT(dataChanged()));
    }

    void DiagramObserver::destroyed()
    {
        emit diagramDestroyed( diagram );
    }

    void DiagramObserver::dataChanged()
    {
        emit diagramDataChanged( diagram );
    }

    void DiagramObserver::attributesChanged()
    {
        emit diagramAttributesChanged( diagram );
    }
}
